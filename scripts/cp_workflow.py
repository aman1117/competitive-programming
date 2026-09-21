"""Standard-library-only tools for trusted, local C++ practice (Python 3.10+)."""

from __future__ import annotations

import argparse
from contextlib import nullcontext
from dataclasses import dataclass
from datetime import datetime, timezone
import difflib
import hashlib
import json
import math
import os
from pathlib import Path
import re
import shutil
import signal
import sqlite3
import subprocess
import sys
import tempfile
import time
from typing import Sequence

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / ".build"
EXE = ".exe" if os.name == "nt" else ""
OUTPUT_LIMIT = 8 * 1024 * 1024


class WorkflowError(Exception):
    """An actionable workflow failure, printed without an internal traceback."""


@dataclass
class Result:
    status: str
    code: int
    elapsed: float
    stdout: bytes
    stderr: bytes


def stop_process(process: subprocess.Popen) -> None:
    """Terminate only this invocation's process tree, never a process name."""
    if process.poll() is not None:
        return
    if os.name == "nt":
        stopped = subprocess.run(
            ["taskkill", "/PID", str(process.pid), "/T", "/F"],
            capture_output=True, timeout=10, check=False,
        )
        if stopped.returncode and process.poll() is None:
            raise WorkflowError(f"Cannot terminate child PID {process.pid}: {stopped.stderr.decode(errors='replace')}")
    else:
        try:
            os.killpg(process.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass  # The child exited between poll and signal.
    process.wait(timeout=10)


def execute(command: Sequence[str], *, timeout: float, data: bytes = b"",
            cwd: Path | None = None, limit: int = OUTPUT_LIMIT) -> Result:
    """File-backed capture avoids pipe deadlocks; bound time and captured output."""
    started = time.monotonic()
    with tempfile.TemporaryFile() as incoming, tempfile.TemporaryFile() as out, tempfile.TemporaryFile() as err:
        incoming.write(data)
        incoming.seek(0)
        process = subprocess.Popen(
            list(command), stdin=incoming, stdout=out, stderr=err, cwd=cwd,
            start_new_session=os.name != "nt",
        )
        status = "OK"
        try:
            while process.poll() is None:
                if time.monotonic() - started >= timeout:
                    status = "TIMEOUT"
                    stop_process(process)
                    break
                if os.fstat(out.fileno()).st_size + os.fstat(err.fileno()).st_size > limit:
                    status = "OUTPUT_LIMIT"
                    stop_process(process)
                    break
                time.sleep(0.01)
        finally:
            if process.poll() is None:
                stop_process(process)
        if os.fstat(out.fileno()).st_size + os.fstat(err.fileno()).st_size > limit:
            status = "OUTPUT_LIMIT"
        if status == "OK" and process.returncode:
            status = "RUNTIME_ERROR"
        out.seek(0)
        err.seek(0)
        return Result(status, process.returncode, time.monotonic() - started, out.read(limit), err.read(limit))


def compiler_path(name: str | None = None) -> str:
    compiler = name or os.environ.get("CXX", "g++")
    resolved = shutil.which(compiler)
    if not resolved:
        raise WorkflowError(f"Compiler not found: {compiler}. Use --compiler with a GCC/Clang executable path.")
    return resolved


def build_flags(mode: str, compiler: str) -> list[str]:
    flags = ["-std=c++17", "-Wall", "-Wextra", "-Wshadow", "-I", str(ROOT / "include")]
    if mode == "release":
        return flags + ["-O2"]
    flags += ["-O0", "-g", "-DLOCAL"]
    if mode in ("diagnostic", "sanitize"):
        flags += [
            "-D_GLIBCXX_ASSERTIONS",
            "-D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG",
            "-fno-omit-frame-pointer",
        ]
    if mode == "diagnostic":
        identity = execute([compiler, "--version"], timeout=10)
        require_success(identity, "Reading compiler version")
        trap = "-fsanitize-trap=undefined" if b"clang" in identity.stdout.lower() else "-fsanitize-undefined-trap-on-error"
        flags += ["-fsanitize=undefined", trap]
    elif mode == "sanitize":
        if os.name == "nt":
            raise WorkflowError("Full ASan/UBSan mode is configured for Linux/macOS, not native Windows. Use --mode diagnostic here.")
        flags += ["-fsanitize=address,undefined", "-fno-sanitize-recover=all"]
    return flags


def require_success(result: Result, operation: str) -> None:
    if result.status != "OK":
        output = (result.stdout + result.stderr).decode("utf-8", errors="replace")
        raise WorkflowError(f"{operation}: {result.status} (exit {result.code})\n{output[:12000]}")


def source_file(value: str | Path) -> Path:
    source = Path(value).resolve()
    if not source.is_file() or source.suffix.lower() != ".cpp":
        raise WorkflowError(f"Expected an existing .cpp file: {source}")
    return source


def compile_cpp(source: Path, output: Path, compiler: str, mode: str,
                *, standalone: bool = False) -> None:
    flags = build_flags(mode, compiler)
    if standalone:
        include_index = flags.index("-I")
        del flags[include_index:include_index + 2]
    result = execute([compiler, *flags, str(source), "-o", str(output)], timeout=120)
    require_success(result, f"Compiling {source.name}")
    if result.stderr:
        sys.stderr.write(result.stderr.decode(errors="replace"))
    if not output.is_file():
        raise WorkflowError(f"Compiler returned success but did not create {output}")


def work_directory():
    BUILD.mkdir(exist_ok=True)
    return tempfile.TemporaryDirectory(prefix="cp-", dir=BUILD)


def compare(actual: bytes, expected: bytes, mode: str = "tokens",
            absolute: float = 1e-6, relative: float = 1e-6) -> tuple[bool, str]:
    if mode == "exact":
        matches = actual == expected
    else:
        left, right = actual.split(), expected.split()
        matches = len(left) == len(right)
        if matches:
            for a, b in zip(left, right):
                if mode != "float":
                    if a != b:
                        matches = False
                        break
                    continue
                try:
                    x, y = float(a), float(b)
                except ValueError:
                    if a != b:
                        matches = False
                        break
                    continue
                if not math.isfinite(x) or not math.isfinite(y) or not math.isclose(x, y, rel_tol=relative, abs_tol=absolute):
                    matches = False
                    break
    if matches:
        return True, ""
    diff = "".join(difflib.unified_diff(
        expected.decode(errors="replace").splitlines(keepends=True),
        actual.decode(errors="replace").splitlines(keepends=True),
        fromfile="expected", tofile="actual",
    ))
    return False, (diff[:6000] or f"Expected bytes {expected[:200]!r}; actual bytes {actual[:200]!r}")


def run_command(args) -> int:
    source = source_file(args.source)
    incoming = Path(args.input).resolve() if args.input else None
    if incoming and not incoming.is_file():
        raise WorkflowError(f"Input file does not exist: {incoming}")
    compiler = compiler_path(args.compiler)
    with work_directory() as folder:
        executable = Path(folder) / ("program" + EXE)
        compile_cpp(source, executable, compiler, args.mode)
        with incoming.open("rb") if incoming else nullcontext(None) as stream:
            process = subprocess.Popen([str(executable)], stdin=stream, start_new_session=os.name != "nt")
            try:
                try:
                    code = process.wait(timeout=args.timeout or None)
                except subprocess.TimeoutExpired:
                    stop_process(process)
                    raise WorkflowError(f"Program exceeded {args.timeout:g} seconds.") from None
            finally:
                if process.poll() is None:
                    stop_process(process)
            if code:
                raise WorkflowError(f"Program exited with code {code}.")
    return 0


def build_command(args) -> int:
    source = source_file(args.source)
    destination = Path(args.output).resolve()
    if destination.suffix.lower() in (".cpp", ".hpp", ".h", ".py", ".ps1", ".md", ".json"):
        raise WorkflowError("Build output must be a binary path, not a source/configuration file.")
    if destination.exists() and destination.parent != (BUILD / "debug").resolve() and not args.force:
        raise WorkflowError(f"Output exists: {destination}. Use --force to replace it.")
    destination.parent.mkdir(parents=True, exist_ok=True)
    with work_directory() as folder:
        executable = Path(folder) / ("program" + EXE)
        compile_cpp(source, executable, compiler_path(args.compiler), args.mode)
        # Replace the debugger binary only after a successful build.
        os.replace(executable, destination)
    print(destination)
    return 0


def judge_command(args) -> int:
    source = source_file(args.source)
    cases = Path(args.cases).resolve() if args.cases else source.parent / "cases"
    if not cases.is_dir():
        raise WorkflowError(f"Case directory not found: {cases}")
    inputs = sorted(cases.glob("*.in"))
    if not inputs:
        raise WorkflowError(f"No *.in files in {cases}")
    for incoming in inputs:
        if not incoming.with_suffix(".out").is_file():
            raise WorkflowError(f"Missing expected output: {incoming.with_suffix('.out')}")
    failures = 0
    with work_directory() as folder:
        executable = Path(folder) / ("solution" + EXE)
        compile_cpp(source, executable, compiler_path(args.compiler), args.mode)
        for incoming in inputs:
            result = execute([str(executable)], timeout=args.timeout, data=incoming.read_bytes())
            matched, diff = compare(result.stdout, incoming.with_suffix(".out").read_bytes(),
                                    args.compare, args.absolute, args.relative)
            status = result.status if result.status != "OK" else ("PASS" if matched else "WRONG_ANSWER")
            print(f"{status:14} {incoming.name} ({result.elapsed * 1000:.0f} ms)")
            if status != "PASS":
                failures += 1
                if diff:
                    print(diff)
                if result.stderr:
                    print(result.stderr.decode(errors="replace")[:6000], file=sys.stderr)
    print(f"{len(inputs) - failures}/{len(inputs)} cases passed.")
    return int(failures != 0)


def save_failure(directory: Path, seed: int, sources: dict[str, Path],
                 results: dict[str, Result], data: bytes, options: dict) -> Path:
    directory.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    folder = Path(tempfile.mkdtemp(prefix=f"seed-{seed}-{stamp}-", dir=directory))
    (folder / "input.in").write_bytes(data)
    shutil.copytree(ROOT / "include", folder / "include")
    metadata = {"seed": seed, "created_utc": stamp, "options": options, "programs": {}}
    for name, source in sources.items():
        shutil.copyfile(source, folder / f"{name}.cpp")
        item = {"source": str(source), "sha256": hashlib.sha256(source.read_bytes()).hexdigest()}
        result = results.get(name)
        if result:
            item.update(status=result.status, exit_code=result.code, seconds=result.elapsed)
            (folder / f"{name}.out").write_bytes(result.stdout)
            (folder / f"{name}.err").write_bytes(result.stderr)
        metadata["programs"][name] = item
    (folder / "failure.json").write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")
    return folder


def stress_command(args) -> int:
    sources = {key: source_file(getattr(args, key)) for key in ("solution", "brute", "generator")}
    compiler = compiler_path(args.compiler)
    with work_directory() as folder:
        executables = {key: Path(folder) / (key + EXE) for key in sources}
        for key in sources:
            compile_cpp(sources[key], executables[key], compiler, args.mode)
        for seed in range(args.seed, args.seed + args.iterations):
            results = {"generator": execute([str(executables["generator"]), str(seed)], timeout=args.timeout)}
            data = results["generator"].stdout
            failed = results["generator"].status != "OK"
            diff = ""
            if not failed:
                for key in ("brute", "solution"):
                    results[key] = execute([str(executables[key])], timeout=args.timeout, data=data)
                matched, diff = compare(results["solution"].stdout, results["brute"].stdout,
                                        args.compare, args.absolute, args.relative)
                failed = not matched or any(result.status != "OK" for result in results.values())
            if failed:
                saved = save_failure(Path(args.failures).resolve(), seed, sources, results, data, {
                    "compiler": compiler, "mode": args.mode, "timeout": args.timeout,
                    "compare": args.compare, "absolute": args.absolute, "relative": args.relative,
                })
                print(f"FAIL seed={seed}: " + ", ".join(f"{key}={value.status}" for key, value in results.items()))
                if diff:
                    print(diff)
                print(f"Counterexample and source snapshots: {saved}")
                return 1
            if (seed - args.seed + 1) % 100 == 0:
                print(f"Passed {seed - args.seed + 1} seeds...")
    print(f"Passed {args.iterations} seeds starting at {args.seed}.")
    return 0


def code_only(text: str) -> str:
    """Mask comments/literals but retain newlines to locate real directives."""
    tokens = re.compile(
        r'//[^\r\n]*|/\*[\s\S]*?\*/'
        r'|(?:u8|u|U|L)?R"(?P<delimiter>[^ ()\\\t\r\n]{0,16})\([\s\S]*?\)(?P=delimiter)"'
        r'|"(?:\\[\s\S]|[^"\\])*"'
        r"|'(?:\\[^\r\n]|[^'\\\r\n])*'"
    )
    return tokens.sub(lambda match: re.sub(r"[^\r\n]", " ", match.group()), text)


def flatten(source: Path) -> str:
    """Expand only the known pragma-once toolkit; reject ambiguous preprocessing."""
    library = (ROOT / "include").resolve()
    emitted: set[Path] = set()
    active: set[Path] = set()
    include = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*(?://.*)?$')

    def expand(file: Path, *, header: bool = False) -> str:
        file = file.resolve()
        if file in active:
            raise WorkflowError(f"Cyclic include while expanding {file}")
        if header and file in emitted:
            return ""
        text = file.read_text(encoding="utf-8-sig")
        if re.search(r"\\\r?\n", text):
            raise WorkflowError(f"Escaped line continuations require manual export: {file}")
        visible = code_only(text)
        if header and not re.search(r"(?m)^\s*#\s*pragma\s+once\s*$", visible):
            raise WorkflowError(f"Only pragma-once toolkit headers can be expanded: {file}")
        active.add(file)
        if header:
            emitted.add(file)
        result = []
        conditions = 0
        for line, code in zip(text.splitlines(), visible.splitlines()):
            if not re.match(r"^\s*#", code):
                result.append(line)
                continue
            if re.match(r"^\s*#\s*(if|ifdef|ifndef)\b", code):
                conditions += 1
            elif re.match(r"^\s*#\s*endif\b", code):
                conditions -= 1
                if conditions < 0:
                    raise WorkflowError(f"Unbalanced preprocessor conditions: {file}")
            if header and re.match(r"^\s*#\s*pragma\s+once\s*$", code):
                continue
            match = include.match(line)
            if not match:
                if re.match(r"^\s*#\s*include\b", code):
                    raise WorkflowError(f"Unsupported include form in {file.name}: {line}")
                result.append(line)
                continue
            delimiter, name = match.groups()
            if delimiter == "<" and not name.startswith("cp/"):
                result.append(line)
                continue
            candidates = [file.parent / name, library / name]
            dependency = next((candidate.resolve() for candidate in candidates if candidate.is_file()), None)
            if dependency is None or not dependency.is_relative_to(library):
                raise WorkflowError(f"Local include is not a toolkit header: {name}. Inline it manually.")
            if conditions:
                raise WorkflowError(f"Conditional toolkit includes are not supported: {name}")
            result.append(expand(dependency, header=True))
        active.remove(file)
        if conditions:
            raise WorkflowError(f"Unbalanced preprocessor conditions: {file}")
        return "\n".join(result) + "\n"

    return "// Standalone C++17 submission generated by cptool export.\n" + expand(source)


def export_command(args) -> int:
    source = source_file(args.source)
    destination = Path(args.output).resolve()
    if destination == source:
        raise WorkflowError("Export must not overwrite its source.")
    if destination.suffix.lower() != ".cpp":
        raise WorkflowError("Submission output must have a .cpp suffix.")
    if destination.exists() and not args.force:
        raise WorkflowError(f"Output exists: {destination}. Use --force only if you intend to replace it.")
    content = flatten(source)
    if re.search(r"(?m)^\s*#\s*define\s+LOCAL\b", code_only(content)):
        raise WorkflowError("Source defines LOCAL itself; remove that definition before export.")
    with work_directory() as folder:
        staged = Path(folder) / "submission.cpp"
        staged.write_text(content, encoding="utf-8")
        compile_cpp(staged, Path(folder) / ("submission" + EXE),
                    compiler_path(args.compiler), "release", standalone=True)
        destination.parent.mkdir(parents=True, exist_ok=True)
        if args.force:
            os.replace(staged, destination)
        else:
            with destination.open("x", encoding="utf-8", newline="\n") as output:
                output.write(content)
    print(f"Standalone file: {destination}")
    print("LOCAL was not defined. Confirm t/no-t, limits, and sample answers before submitting.")
    return 0


def new_command(args) -> int:
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_-]*", args.name):
        raise WorkflowError("Problem name must contain only letters, digits, underscores, and hyphens.")
    destination = ROOT / "practice" / args.name
    destination.mkdir(parents=True, exist_ok=False)
    (destination / "cases").mkdir()
    shutil.copyfile(ROOT / "templates" / "codeforces.cpp", destination / "main.cpp")
    print(destination)
    print("Add cases/sample1.in and cases/sample1.out, then use cptool judge.")
    return 0


def format_command(args) -> int:
    source = source_file(args.source)
    formatter = shutil.which("clang-format")
    if not formatter:
        raise WorkflowError("clang-format not found on PATH.")
    style = f"-style=file:{ROOT / '.clang-format'}"
    flags = ["--dry-run", "--Werror"] if args.check else ["-i"]
    result = execute([formatter, style, *flags, str(source)], timeout=30)
    require_success(result, "Formatting")
    print(f"{'Checked' if args.check else 'Formatted'} {source}")
    return 0


def check_command(args) -> int:
    compiler = compiler_path(args.compiler)
    flags = build_flags(args.mode, compiler) + ["-Werror"]
    for header in sorted((ROOT / "include" / "cp").glob("*.hpp")):
        require_success(execute(
            [compiler, *flags, "-x", "c++", "-fsyntax-only", "-include", str(header), "-"],
            timeout=60, data=b"\n"), f"Header {header.name}")
    starters = [ROOT / "main.cpp", *sorted((ROOT / "templates").glob("*.cpp"))]
    for local in ([], ["-DLOCAL"]):
        require_success(execute([compiler, *flags, *local, "-fsyntax-only", *map(str, starters)],
                                timeout=60), "Submission starters")
    with work_directory() as folder:
        executable = Path(folder) / ("tests" + EXE)
        for test in sorted((ROOT / "tests").glob("*_test.cpp")):
            compile_cpp(test, executable, compiler, args.mode)
            result = execute([str(executable)], timeout=90)
            require_success(result, test.name)
            print(result.stdout.decode(errors="replace").strip())
    return 0


def positive(value: str) -> float:
    result = float(value)
    if not math.isfinite(result) or result <= 0:
        raise argparse.ArgumentTypeError("Must be a finite positive number.")
    return result


def nonnegative(value: str) -> float:
    result = float(value)
    if not math.isfinite(result) or result < 0:
        raise argparse.ArgumentTypeError("Must be a finite nonnegative number.")
    return result


def natural(value: str) -> int:
    result = int(value)
    if result < 1:
        raise argparse.ArgumentTypeError("Must be a positive integer.")
    return result


def compilation(parser, default="release"):
    parser.add_argument("--compiler", help="GCC/Clang executable (default: CXX environment variable, then g++).")
    parser.add_argument("--mode", choices=["release", "debug", "diagnostic", "sanitize"], default=default)


def comparison(parser):
    parser.add_argument("--compare", choices=["tokens", "exact", "float"], default="tokens")
    parser.add_argument("--absolute", type=nonnegative, default=1e-6, help="Absolute tolerance for float comparison.")
    parser.add_argument("--relative", type=nonnegative, default=1e-6, help="Relative tolerance for float comparison.")
    parser.add_argument("--timeout", type=positive, default=2.0, help="Wall-time limit per program, seconds.")


def parser() -> argparse.ArgumentParser:
    cli = argparse.ArgumentParser(description=__doc__)
    commands = cli.add_subparsers(dest="command", required=True)
    run = commands.add_parser("run", help="Compile and run with terminal I/O.")
    run.add_argument("source", nargs="?", default="main.cpp")
    run.add_argument("--input")
    run.add_argument("--timeout", type=nonnegative, default=0, help="0 means no interactive timeout.")
    compilation(run)
    run.set_defaults(handler=run_command)
    build = commands.add_parser("build", help="Build a persistent debugger executable.")
    build.add_argument("source")
    build.add_argument("--output", default=str(BUILD / "debug" / ("program" + EXE)))
    build.add_argument("--force", action="store_true")
    compilation(build, "debug")
    build.set_defaults(handler=build_command)
    judge = commands.add_parser("judge", help="Run paired *.in/*.out cases.")
    judge.add_argument("source")
    judge.add_argument("--cases", help="Default: cases directory beside the source.")
    compilation(judge)
    comparison(judge)
    judge.set_defaults(handler=judge_command)
    stress = commands.add_parser("stress", help="Compare a solution to a brute-force reference.")
    for name in ("solution", "brute", "generator"):
        stress.add_argument(name)
    stress.add_argument("--iterations", type=natural, default=100)
    stress.add_argument("--seed", type=int, default=1)
    stress.add_argument("--failures", default=str(ROOT / ".practice" / "failures"))
    compilation(stress)
    comparison(stress)
    stress.set_defaults(handler=stress_command)
    export = commands.add_parser("export", help="Expand cp headers and compile without LOCAL or local include paths.")
    export.add_argument("source")
    export.add_argument("--output", required=True)
    export.add_argument("--compiler")
    export.add_argument("--force", action="store_true")
    export.set_defaults(handler=export_command)
    new = commands.add_parser("new", help="Create a local/Codeforces problem folder; never overwrite an existing one.")
    new.add_argument("name")
    new.set_defaults(handler=new_command)
    fmt = commands.add_parser("format", help="Format only the selected C++ file.")
    fmt.add_argument("source")
    fmt.add_argument("--check", action="store_true")
    fmt.set_defaults(handler=format_command)
    check = commands.add_parser("check", help="Portable header, starter and C++ regression runner.")
    compilation(check)
    check.set_defaults(handler=check_command)
    from revision import register_commands
    register_commands(commands, ROOT)
    return cli


def main(argv=None) -> int:
    try:
        args = parser().parse_args(argv)
        return args.handler(args)
    except (WorkflowError, OSError, ValueError, sqlite3.Error) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("Cancelled.", file=sys.stderr)
        return 130


if __name__ == "__main__":
    sys.exit(main())
