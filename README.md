# C Programming Exam — Spring 2026

My exam submission for PG3401, Spring 2026, written in C and originally developed in a Debian Linux environment.

This repository preserves the submitted source code, Makefiles, supporting data, and screenshots.

## Project Structure

- `task2_300/` — Task 2 source code and supporting files.
- `task3_300/` — Task 3 source code and supporting files.
- `task4_300/` — Task 4 source code and supporting files.
- `task5_300/` — Task 5 source code and supporting files.
- `logdata/` — Logs and data from the exam environment.

Each task includes its own Makefile and header files in an `include/` directory.

## Build and Run

The intended environment is Debian Linux with GCC and Make installed.

To build and run a task, enter its directory and create the object directory before running Make. For example:

```bash
cd task2_300
mkdir -p obj
make
./task2
```

To remove compiled files:

```bash
make clean
```

Some tasks require additional input files or components from the original exam environment.

## EWA Exam Software

The EWA program, created by the lecturer/examiner, is not included in this repository. It is needed to fully run and understand some of the tasks.

The software may be reused by its author in future exams. It has therefore been excluded from this public repository to prevent future exam candidates from accessing it in advance.

## Compatibility

The code was developed for Linux. Native macOS compatibility has not been verified.

Compiled executables and object files are excluded from version control.

## Notes

This is an archived exam submission rather than a maintained software project. It may contain incomplete functionality, bugs, or limitations.
