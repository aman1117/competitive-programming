# Productive LeetCode and local C++ practice

[Project home](../../README.md) | [Algorithm handbook](../README.md)

**LeetCode is solved on LeetCode.** Use its editor, custom test cases, Run, and Submit. This toolkit does not log in, scrape questions, read cookies, automate submissions, or require a local LeetCode harness. Local commands support revision and optional Codeforces/general C++ practice.

## 1. Start a session

Open a new PowerShell 7 terminal, or load the added shell function once:

```powershell
. $PROFILE.CurrentUserAllHosts
cd "$HOME\competitive-programming"
cptool due
```

`cptool` invokes `scripts\cp_workflow.py`. The implementation needs **Python 3.10+**, using only the standard library, and the already-installed GCC/Clang-compatible compiler for C++ commands. The revision commands do not need a compiler.

Without the shell function:

```powershell
python .\scripts\cp_workflow.py --help
python .\scripts\cp_workflow.py due
```

All source/input/output paths are relative to your terminal unless absolute. The default revision database, failure snapshots, and build area are anchored to this toolkit, even when `cptool` is called elsewhere.

## 2. The LeetCode practice loop

1. Choose one due revision or an untagged problem near your current ability. Do not open its topic tags or editorial immediately.
2. State the brute-force solution, constraints, candidate invariant/recurrence, and expected TC/SC before coding.
3. Attempt it in LeetCode's editor. Use custom cases for empty/singleton inputs where allowed, repeated values, boundary values, and impossible states.
4. After a deliberate attempt window, take the smallest useful hint. If you read the editorial, close it and implement the reasoning yourself.
5. Submit on LeetCode. Record whether you solved independently, needed a hint/editorial, or did not finish. An accepted submission after copying an editorial is not an independent solve.
6. Re-solve due problems later without looking at the previous solution. Mix topic practice with timed contests and untagged sets.

Timeboxes such as 25-40 minutes are starting points, not a rule to abandon every deeper problem. The tracker measures self-reported practice, not your LeetCode rating.

### Record one real attempt

The following is a **usage example**; log it only after actually attempting that problem:

```powershell
cptool log 239 --title "Sliding Window Maximum" --topic monotonic-deque `
  --outcome hinted --minutes 35 --mistake implementation `
  --note "Store indices; expire the front before reading the window maximum."

cptool due
cptool history 239
cptool stats
```

| Option | Values / meaning |
|---|---|
| `--platform` | `lc` (default) or `cf`; the same numeric ID on different platforms is distinct |
| `--outcome` | `independent`, `hinted`, `editorial`, `failed` |
| `--mistake` | `none`, `recognition`, `proof`, `implementation`, `complexity`, `edge-case`, `overflow`, `time` |
| `--note` | One corrective lesson: what assumption failed, what invariant fixes it |
| `--date` | Optional ISO date (`YYYY-MM-DD`), default local date; backdated attempts are rejected |
| `--db` | Optional alternate database, useful for a separate practice history |

No sample attempts are pre-populated. Your history starts empty. Later logs can omit title/topic to retain the existing values.

### Revision schedule

The first logged attempt schedules a review **2 days later**. An independent solve on or after that due date advances to **7 days after that solve**, then **21 days after the next independent due solve**. Completing the last due solve finishes this spaced cycle; the problem can still appear in mixed practice.

Hinted, editorial-assisted, or failed attempts reset the cycle to two days. Early independent repeats do not consume stages or postpone the existing due date. The intervals are a practical heuristic, not a scientific guarantee of retention.

Data is stored in `.practice\revision.sqlite3`, ignored by Git. `history` prints the last 100 matching attempts; `stats` summarizes all attempts. To back up the full history, copy the SQLite file when no tracker command is running. There is no email, background scheduling, network sync, or automatic account access.

## 3. Local/Codeforces problem folders

Create a fresh problem without overwriting your current `main.cpp`:

```powershell
cptool new cf-123A
```

It creates:

```text
practice\cf-123A\
  main.cpp       Standalone C++17 starter copied from templates\codeforces.cpp
  cases\         Add sample1.in, sample1.out, edge1.in, edge1.out, ...
```

The starter reads a leading `t`; set `multiple_test_cases=false` for a single-test problem. Existing folders are never overwritten. Your root `main.cpp` is left untouched.

## 4. Run or judge samples

Interactive and input-file execution keep the original `cprun` syntax:

```powershell
cprun .\practice\cf-123A\main.cpp
cprun .\practice\cf-123A\main.cpp -InputFile .\input.txt
cprun .\practice\cf-123A\main.cpp -Local
cprun .\practice\cf-123A\main.cpp -Diagnostic -InputFile .\input.txt -TimeoutSeconds 5
```

To compare all cases automatically:

```powershell
cptool judge .\practice\cf-123A\main.cpp

# Existing runnable example:
cptool judge .\examples\range_sum.cpp --cases .\examples --mode diagnostic
```

`judge` compiles once and runs sorted `*.in` files. Every input must have a same-stem `.out`. Missing expected output, compile failure, runtime error, timeout, excessive output, or a mismatch produce a nonzero exit code.

| Comparison | Command | Semantics |
|---|---|---|
| Default | `--compare tokens` | Ignore whitespace differences; all tokens must match exactly |
| Strict | `--compare exact` | Compare bytes, including line endings and final newline |
| Numeric tolerance | `--compare float --absolute 1e-6 --relative 1e-6` | Finite numeric tokens use tolerance; nonnumeric tokens must match exactly |

Float mode rejects NaN/infinity. Use the actual problem's tolerance, not blindly the defaults. Problems accepting multiple different valid constructions require a problem-specific checker; this runner does not pretend token comparison can judge them.

Default judge/stress timeout is **2 wall-clock seconds per program**. Change it with `--timeout`. Captured stdout plus stderr has an **8 MiB monitored limit**; polling can allow a small overshoot. Timing includes local process startup and is not a judge-equivalent CPU benchmark.

Interactive `cprun` defaults to no timeout so it can wait for keyboard input. Compile steps have a 120-second limit. Failed compilation never runs a previous binary. Temporary executables are removed; persistent debug builds are the deliberate exception.

## 5. Differential stress testing

Use three standalone C++ programs:

| Program | Contract |
|---|---|
| Solution | Read one problem instance from stdin and print its result |
| Brute force | Solve the same instance independently, simply and correctly |
| Generator | Read integer seed from `argv[1]`; print one valid, small instance |

```powershell
cptool stress .\examples\stress\solution.cpp .\examples\stress\brute.cpp `
  .\examples\stress\generator.cpp --iterations 100 --seed 1 --mode diagnostic
```

On the first mismatch or execution failure, a uniquely named directory under `.practice\failures` stores the input, stdout/stderr, seed, statuses, options, source snapshots, hashes, and toolkit headers. Nothing is silently overwritten. Replay the exact saved `input.in`; regenerating from a seed additionally depends on the generator and toolchain.

To replay a snapshot with its saved headers, compile from that failure directory using its `include` directory:

```powershell
# Run inside the particular saved failure directory.
g++ -std=c++17 -I .\include .\solution.cpp -o .\replay.exe
# Feed input.in using your shell's native redirection, or inspect it manually.
```

`cptool run` uses the current toolkit headers, so it is convenient for diagnosing your current solution but is not an exact historical-header replay. External libraries or custom headers outside the toolkit are not snapshotted. Failure directories may contain large outputs and should be curated.

Stress testing is optional for local CP work; do not move your ordinary LC routine off the website just to use it.

## 6. Build modes and runtime diagnostics

| Mode | Behavior |
|---|---|
| `release` | C++17, `-O2`, warnings; no `LOCAL` macro |
| `debug` / `cprun -Local` | `-O0 -g -DLOCAL`; suitable for breakpoints |
| `diagnostic` / `cprun -Diagnostic` | Debug flags + undefined-behavior traps + libstdc++ assertions/libc++ debug hardening |
| `sanitize` | Linux/macOS ASan + UBSan with debug flags; native Windows explicitly rejects this mode |

The native ARM64 LLVM-MinGW diagnostic mode detects supported UB such as signed overflow and enabled library assertions such as `vector[]` bounds errors. A trap may report only a nonzero exit/exception: use the debugger to locate it.

**It is not full memory-safety coverage.** It does not replace ASan for use-after-free and arbitrary memory corruption; library checks also depend on the library/version. No flag can establish algorithm correctness. Unsupported compiler flags fail explicitly; there is no silent fallback to an unchecked mode.

Example portable commands:

```powershell
cptool check --mode diagnostic
cptool check --compiler clang++ --mode diagnostic
```

Run `python3 scripts/cp_workflow.py check --compiler g++ --mode sanitize` on Linux for GNU GCC sanitizer coverage. Your native `g++` is an LLVM-MinGW/Clang driver, not proof of GNU GCC compatibility.

## 7. Editor shortcuts and debugging

clangd remains the only C++ language server. **LLDB DAP** is a debugger adapter, not a competing IntelliSense engine.

| Action | How |
|---|---|
| Build active `.cpp` for debugging | **Ctrl+Shift+B** |
| Debug with keyboard input | **F5**, choose `C++: debug active file (keyboard input)` |
| Debug with root `input.txt` | **F5**, choose `C++: debug active file (input.txt)` |
| Run all sample pairs next to a source | Tasks: `CP: judge active C++ samples` |
| Run with runtime checks | Tasks: `CP: run active C++ (diagnostic)` |
| Show due revisions | Tasks: `LC: show due revisions` |
| Format the active file | **Shift+Alt+F** (clangd) |

Keep the desired `.cpp` active before starting a build/debug task; a header or Markdown file is not an executable. The build task creates `.build\debug\program.exe` and replaces it only after successful compilation. Breakpoints, stepping, stack traces, and watch expressions use debug symbols.

The input-file configuration uses LLDB's `target.input-path` setting, compatible with the installed native adapter. It leaves stdout/stderr separate and does not truncate the input file. For keyboard input, type into the integrated terminal, not the Debug Console. If VS Code was open during debugger installation, reload the window.

The editor settings contain machine-specific ARM64 compiler/debugger paths. Update them if the toolchain moves. These VS Code launch paths are Windows-specific; portable CLI/CI commands remain usable on Linux.

## 8. Formatting and standalone submissions

`.clang-format` defines four-space indentation, a 100-column target, attached braces, and preserved include order. Format-on-save is deliberately off: existing educational templates are not reformatted wholesale.

```powershell
cptool format .\practice\cf-123A\main.cpp
cptool format .\practice\cf-123A\main.cpp --check
```

For a **local C++/Codeforces** submission that uses `cp/...` headers:

```powershell
cptool export .\practice\cf-123A\main.cpp --output .\submission.cpp
```

Export recursively expands known pragma-once toolkit headers, removes their `#pragma once`, preserves standard includes, and compiles the result without the toolkit include path and without defining `LOCAL`. It does not change the source or submit anything. Existing output requires explicit `--force`.

It deliberately rejects conditional toolkit includes, macro-generated includes, escaped line continuations, and quoted headers outside the toolkit instead of producing a potentially broken file. It preserves include-looking text inside comments and string literals. It is a convenience flattener for this library, not a full C++ preprocessor. Guarded `#ifdef LOCAL` code may remain in the file but is not compiled on the judge.

Always confirm the input format, numeric bounds, sample answers, and judge standard yourself. For LeetCode, continue submitting directly in the website editor; do not use this executable-oriented exporter for a class-only LC submission.

## 9. Portable checks and CI

```powershell
.\scripts\test.ps1
python -m unittest discover -s .\tests -p "test_*.py"
```

The PowerShell runner covers standalone headers, starter modes, C++ algorithm suites, sorting, and workflow/revision tests. Python's built-in `unittest` requires no additional package.

`.github\workflows\cpp.yml` defines Ubuntu jobs for both **GNU GCC and Clang**, with ASan/UBSan algorithm checks, workflow tests, and the seeded stress example. Those remote jobs run after pushing to GitHub or manually dispatching the workflow; merely adding the workflow file does not mean a remote run has occurred.

All runners execute **trusted local programs**, not sandboxed submissions. Programs have your user permissions. Time/output limits are convenience safeguards, not security boundaries or memory limits; do not use these tools to execute untrusted downloads.
