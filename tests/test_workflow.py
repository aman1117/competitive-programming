"""Run with Python's built-in unittest; no pytest or third-party packages."""

import argparse
from contextlib import closing, redirect_stdout, redirect_stderr
from datetime import date
import importlib
import io
import json
import os
from pathlib import Path
import sqlite3
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
cp = importlib.import_module("cp_workflow")
revision = importlib.import_module("revision")


class WorkflowTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="cp workflow tests ")
        self.folder = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def cli(self, *args):
        output, errors = io.StringIO(), io.StringIO()
        with redirect_stdout(output), redirect_stderr(errors):
            code = cp.main(list(args))
        return code, output.getvalue(), errors.getvalue()

    def write(self, name, contents):
        filename = self.folder / name
        filename.parent.mkdir(parents=True, exist_ok=True)
        filename.write_text(contents, encoding="utf-8")
        return filename

    def test_comparison_modes(self):
        self.assertEqual(cp.compare(b"1 2\n", b"1\n2\r\n")[0], True)
        self.assertEqual(cp.compare(b"1 2\n", b"1 2", "exact")[0], False)
        self.assertEqual(cp.compare(b"0.3333333 yes", b"0.333333 yes", "float")[0], True)
        self.assertEqual(cp.compare(b"NaN", b"1", "float")[0], False)
        self.assertEqual(cp.compare(b"inf", b"1", "float")[0], False)
        self.assertEqual(cp.compare(b"nan", b"nan", "float")[0], False)
        self.assertEqual(cp.compare(b"inf", b"inf", "float")[0], False)
        self.assertEqual(cp.compare(b"answer", b"other", "float")[0], False)
        self.assertEqual(cp.compare(b"1", b"1 2")[0], False)
        self.assertIn("expected", cp.compare(b"bad\n", b"good\n")[1])

    def test_timeout_exit_and_output_limit(self):
        command = [sys.executable, "-c", "import time; time.sleep(30)"]
        result = cp.execute(command, timeout=0.15)
        self.assertEqual(result.status, "TIMEOUT")
        self.assertLess(result.elapsed, 5)
        result = cp.execute([sys.executable, "-c", "raise SystemExit(7)"], timeout=5)
        self.assertEqual((result.status, result.code), ("RUNTIME_ERROR", 7))
        result = cp.execute([sys.executable, "-c", "print('x'*10000)"], timeout=5, limit=100)
        self.assertEqual(result.status, "OUTPUT_LIMIT")
        self.assertLessEqual(len(result.stdout), 100)

    def test_input_and_stderr_capture(self):
        result = cp.execute([sys.executable, "-c",
                             "import sys; sys.stdout.buffer.write(sys.stdin.buffer.read()); print('err',file=sys.stderr)"],
                            timeout=5, data=b"\xff\r\n42")
        self.assertEqual(result.stdout, b"\xff\r\n42")
        self.assertIn(b"err", result.stderr)

    def test_no_case_or_missing_expected_output(self):
        source = self.write("main.cpp", "int main() {}")
        cases = self.folder / "cases"
        cases.mkdir()
        self.assertEqual(self.cli("judge", str(source))[0], 1)
        self.write("cases/a.in", "1")
        code, _, error = self.cli("judge", str(source))
        self.assertEqual(code, 1)
        self.assertIn("Missing expected output", error)

    def test_compile_failure_does_not_run_stale_program(self):
        source = self.write("bad.cpp", "int main() { missing_symbol; }")
        output = self.write("existing.exe", "untouched")
        code, _, error = self.cli("build", str(source), "--output", str(output), "--force")
        self.assertEqual(code, 1)
        self.assertIn("Compiling", error)
        self.assertEqual(output.read_text(), "untouched")

    def test_judge_pass_and_wrong_answer(self):
        source = self.write("program with spaces.cpp",
                            "#include <iostream>\nint main(){long long a,b; std::cin>>a>>b; std::cout<<a+b<<'\\n';}")
        self.write("cases/one.in", "1 2")
        expected = self.write("cases/one.out", "3\n")
        self.write("cases/two.in", "-2 2")
        self.write("cases/two.out", "0\n")
        code, output, error = self.cli("judge", str(source))
        self.assertEqual(code, 0, error)
        self.assertIn("2/2 cases passed", output)
        expected.write_text("4\n")
        code, output, _ = self.cli("judge", str(source))
        self.assertEqual(code, 1)
        self.assertIn("WRONG_ANSWER", output)
        self.assertIn("-4", output)

    def test_judge_timeout_and_runtime_error(self):
        source = self.write("loop.cpp", "int main(){for(;;){}}")
        self.write("cases/a.in", "")
        self.write("cases/a.out", "")
        code, output, _ = self.cli("judge", str(source), "--timeout", "0.1", "--mode", "debug")
        self.assertEqual(code, 1)
        self.assertIn("TIMEOUT", output)
        source.write_text("int main(){return 5;}")
        code, output, _ = self.cli("judge", str(source))
        self.assertEqual(code, 1)
        self.assertIn("RUNTIME_ERROR", output)

    def test_export_expands_once_and_runs_without_includes(self):
        source = self.write("source.cpp", '#include "cp/arrays.hpp"\n#include "cp/arrays.hpp"\n'
                            '#include <iostream>\nint main(){std::cout<<*cp::max_subarray_sum({-1,3,4})<<"\\n";}')
        exported = self.folder / "submission.cpp"
        code, _, error = self.cli("export", str(source), "--output", str(exported))
        self.assertEqual(code, 0, error)
        content = exported.read_text()
        self.assertNotIn('#include "cp/', content)
        self.assertNotIn("#pragma once", content)
        self.assertEqual(content.count("inline std::optional<i64> max_subarray_sum"), 1)
        compiler = cp.compiler_path()
        executable = self.folder / ("submission" + cp.EXE)
        cp.compile_cpp(exported, executable, compiler, "release", standalone=True)
        result = cp.execute([str(executable)], timeout=5)
        self.assertEqual(result.stdout.strip(), b"7")
        self.assertEqual(self.cli("export", str(source), "--output", str(source))[0], 1)
        self.assertEqual(self.cli("export", str(source), "--output", str(exported))[0], 1)

    def test_export_rejects_unsupported_forms(self):
        for contents in (
            '#ifdef LOCAL\n#include "cp/arrays.hpp"\n#endif\nint main(){}',
            '#define HEADER "cp/arrays.hpp"\n#include HEADER\nint main(){}',
            '#include "nonexistent.hpp"\nint main(){}',
            '#define LOCAL\nint main(){}',
            '// escaped comment \\\n#include "cp/arrays.hpp"\nint main(){}',
        ):
            source = self.write("bad_export.cpp", contents)
            destination = self.folder / "out.cpp"
            self.assertEqual(self.cli("export", str(source), "--output", str(destination))[0], 1)
            self.assertFalse(destination.exists())

    def test_export_does_not_rewrite_comments_or_raw_strings(self):
        literal = 'R"tag(\n#include "not-a-real-header.hpp"\n#define LOCAL\n#pragma once\n)tag"'
        comment = '/*\n#include "also-not-real.hpp"\n#ifdef SOMETHING\n*/'
        source = self.write("literal.cpp",
                            f'#include <iostream>\n{comment}\nint main(){{std::cout<<{literal};}}\n')
        flattened = cp.flatten(source)
        self.assertIn(literal, flattened)
        self.assertIn(comment, flattened)
        code, _, error = self.cli("export", str(source), "--output", str(self.folder / "literal-output.cpp"))
        self.assertEqual(code, 0, error)

    def test_stress_saves_failing_seed_input_and_sources(self):
        solution = self.write("solution.cpp", "#include <iostream>\nint main(){std::cout<<0;}")
        brute = self.write("brute.cpp", "#include <iostream>\nint main(){int n;std::cin>>n;std::cout<<n;}")
        generator = self.write("generator.cpp", "#include <iostream>\nint main(int,char** a){std::cout<<a[1];}")
        failures = self.folder / "failures"
        code, output, error = self.cli("stress", str(solution), str(brute), str(generator),
                                       "--seed", "41", "--iterations", "3", "--failures", str(failures))
        self.assertEqual(code, 1, error)
        self.assertIn("seed=41", output)
        snapshots = list(failures.iterdir())
        self.assertEqual(len(snapshots), 1)
        saved = snapshots[0]
        self.assertEqual((saved / "input.in").read_text(), "41")
        self.assertEqual(json.loads((saved / "failure.json").read_text())["seed"], 41)
        self.assertEqual((saved / "solution.cpp").read_text(), solution.read_text())
        self.assertTrue((saved / "include" / "cp" / "arrays.hpp").is_file())
        solution.write_text(brute.read_text())
        code, output, error = self.cli("stress", str(solution), str(brute), str(generator),
                                       "--seed", "41", "--iterations", "4", "--failures", str(failures))
        self.assertEqual(code, 0, error)
        self.assertIn("Passed 4 seeds", output)

    def test_new_problem_never_overwrites(self):
        with patch.object(cp, "ROOT", self.folder):
            (self.folder / "templates").mkdir()
            self.write("templates/codeforces.cpp", "int main() {}")
            self.assertEqual(self.cli("new", "cf-123A")[0], 0)
            self.assertTrue((self.folder / "practice" / "cf-123A" / "cases").is_dir())
            self.assertEqual(self.cli("new", "cf-123A")[0], 1)
            self.assertEqual(self.cli("new", "..")[0], 1)

    def test_revision_cli_uses_explicit_private_database(self):
        filename = self.folder / "my history.sqlite3"
        code, output, error = self.cli(
            "log", "239", "--outcome", "hinted", "--title", "Sliding Window Maximum",
            "--mistake", "implementation", "--note", "Expire old indices",
            "--date", "2026-09-22", "--db", str(filename),
        )
        self.assertEqual(code, 0, error)
        self.assertIn("2026-09-24", output)
        code, output, error = self.cli("due", "--date", "2026-09-24", "--db", str(filename), "--json")
        self.assertEqual(code, 0, error)
        self.assertEqual(json.loads(output)[0]["problem"], "239")
        code, output, error = self.cli("history", "239", "--db", str(filename))
        self.assertEqual(code, 0, error)
        self.assertEqual(json.loads(output)[0]["note"], "Expire old indices")
        code, output, error = self.cli("stats", "--db", str(filename))
        self.assertEqual(code, 0, error)
        self.assertIn("hinted: 1", output)

    def test_diagnostic_mode_detects_undefined_behavior(self):
        source = self.write("overflow.cpp",
                            "#include <limits>\nint main(){volatile int x=std::numeric_limits<int>::max();return x+1;}")
        executable = self.folder / ("overflow" + cp.EXE)
        cp.compile_cpp(source, executable, cp.compiler_path(), "diagnostic")
        self.assertEqual(cp.execute([str(executable)], timeout=5).status, "RUNTIME_ERROR")

    @unittest.skipUnless(os.name == "nt", "PowerShell compatibility wrapper is Windows-specific")
    def test_powershell_cprun_wrapper(self):
        source = self.write("wrapper sample.cpp", "#include <iostream>\nint main(){int x;std::cin>>x;std::cout<<x+1;}")
        incoming = self.write("input data.txt", "41")
        result = cp.execute(["pwsh", "-NoProfile", "-File", str(ROOT / "scripts" / "run-cpp.ps1"),
                             str(source), "-InputFile", str(incoming), "-Diagnostic", "-TimeoutSeconds", "5"],
                            timeout=30)
        self.assertEqual(result.status, "OK", result.stderr.decode(errors="replace"))
        self.assertEqual(result.stdout.strip(), b"42")


class RevisionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.db = Path(self.temp.name) / "revision.sqlite3"

    def tearDown(self):
        self.temp.cleanup()

    def record(self, when, outcome="independent", **kwargs):
        values = dict(platform="lc", problem="239", title="Sliding Window Maximum", topic="deque",
                      attempted=date.fromisoformat(when), outcome=outcome, minutes=30,
                      mistake="none", note="")
        values.update(kwargs)
        return revision.record_attempt(self.db, **values)

    def test_spaced_cycle_and_early_repeats(self):
        self.assertEqual(self.record("2026-09-22"), ("2026-09-24", 0))
        self.assertEqual(self.record("2026-09-22"), ("2026-09-24", 0))
        self.assertEqual(self.record("2026-09-23"), ("2026-09-24", 0))
        self.assertEqual(self.record("2026-09-24"), ("2026-10-01", 1))
        self.assertEqual(self.record("2026-10-01"), ("2026-10-22", 2))
        self.assertEqual(self.record("2026-10-22"), (None, 3))

    def test_failure_resets_schedule_and_keeps_history(self):
        self.record("2026-09-22")
        self.record("2026-09-24")
        self.assertEqual(self.record("2026-09-26", "hinted", note="Forgot expiration rule"),
                         ("2026-09-28", 0))
        with revision.database(self.db) as connection:
            self.assertEqual(connection.execute("SELECT COUNT(*) FROM attempts").fetchone()[0], 3)
            self.assertEqual(connection.execute("SELECT note FROM attempts ORDER BY id DESC").fetchone()[0],
                             "Forgot expiration rule")

    def test_backdates_rejected_and_transactions_rollback(self):
        self.record("2026-09-22")
        with self.assertRaises(ValueError):
            self.record("2026-09-21")
        with revision.database(self.db) as connection:
            self.assertEqual(connection.execute("SELECT COUNT(*) FROM attempts").fetchone()[0], 1)

    def test_due_query_and_problem_identity(self):
        self.record("2026-09-22")
        self.record("2026-09-22", platform="cf")
        output = io.StringIO()
        with redirect_stdout(output):
            revision.due_command(argparse.Namespace(db=str(self.db), date=date(2026, 9, 24), json=True))
        rows = json.loads(output.getvalue())
        self.assertEqual(len(rows), 2)
        self.assertEqual({row["platform"] for row in rows}, {"lc", "cf"})

    def test_quotes_are_data_not_sql(self):
        self.record("2026-09-22", title="don't drop; ' tables", note="' OR 1=1 --")
        with revision.database(self.db) as connection:
            self.assertEqual(connection.execute("SELECT note FROM attempts").fetchone()[0], "' OR 1=1 --")

    def test_missing_history_is_not_fabricated(self):
        output = io.StringIO()
        with redirect_stdout(output):
            revision.due_command(argparse.Namespace(db=str(self.db), date=date(2026, 9, 24), json=False))
        self.assertIn("No practice history", output.getvalue())
        self.assertFalse(self.db.exists())

    def test_unrelated_database_is_not_modified(self):
        with closing(sqlite3.connect(self.db)) as connection:
            connection.execute("CREATE TABLE unrelated(value TEXT)")
        with self.assertRaisesRegex(ValueError, "unrelated"):
            self.record("2026-09-22")
        with closing(sqlite3.connect(self.db)) as connection:
            names = [row[0] for row in connection.execute("SELECT name FROM sqlite_master WHERE type='table'")]
            self.assertEqual(names, ["unrelated"])


if __name__ == "__main__":
    unittest.main()
