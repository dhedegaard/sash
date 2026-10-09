# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

sash is a simple Unix shell written in C89 (circa 2010). It supports built-in commands (help, quit/exit, environ, echo, ls/dir, cd, clr/clear), executes everything else via fork/execvp, and handles I/O redirection (`<`, `>`) and background execution (`&`).

## Build

CMake out-of-source build (this is also what CI runs, see .github/workflows/cmake.yml):

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/sash
```

On UNIX the build compiles with `-Wall -Wextra -pedantic`; keep the code warning-clean. The code targets ANSI C (C89-style declarations at top of block, `/* */` comments) — match that style.

There is no test suite. parser.c contains a commented-out block of assert-based tests for `parsetoargs()` that can be adapted for manual verification.

## Architecture

Flow: `main.c` runs the REPL (prompt → fgets → parse) → `parser.c` tokenizes input into a `struct arg_t` (defined in command.h: cmd, argv/argc, inputfile, outputfile, background) and dispatches built-ins by string-matching the command → `command.c` provides the `cmd_*` wrappers → unrecognized commands go to `exec.c`, which forks, applies redirection via freopen, and execvp's.

Each .c file is built as its own static library in CMakeLists.txt and linked into the `sash` executable — a new module needs an `add_library` entry and addition to `target_link_libraries(sash ...)`.

Supporting modules: `utility.c` (string helpers like trim/startswith, path helpers like getstartwd/getrelativepath), `environ.c` (environment variable storage/printing), `queue.c`/`stack.c` (generic containers used by parser and ls), `prompt.c` (prints prompt showing current directory), `ls.c`/`cd.c` (built-in implementations).

Note: `cmd_help` reads the `readme` file from the shell's start directory at runtime, so the readme doubles as the help text — keep it updated when commands change. `todo` tracks the release roadmap and known bugs.
