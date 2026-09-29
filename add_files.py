#!/usr/bin/env python3
"""Add two files line by line via ASCII values.

Reads lines from two input files and, for each pair of corresponding
lines (line i of FILE1 and line i of FILE2), writes a single number to
the output: the sum of the ASCII values of every character in both
lines.

Usage:
    python3 add_files.py FILE1 FILE2 [-o OUTPUT]

Example:
    FILE1 line "ab"  ->  97 + 98            = 195
    FILE2 line "cd"  ->  99 + 100           = 199
    output line      ->  195 + 199          = 394

Notes:
    * The trailing newline of each line is not counted; every other
      character (including spaces) contributes its ASCII value.
    * If the files have different numbers of lines, only the lines
      present in both files are added and a warning is printed.
"""

import argparse
import sys


def read_lines(path):
    """Return the list of lines (without trailing newlines) of `path`."""
    try:
        with open(path, "r", encoding="ascii") as f:
            return f.read().splitlines()
    except UnicodeDecodeError as exc:
        sys.exit(f"error: {path!r} contains non-ASCII data: {exc}")
    except OSError as exc:
        sys.exit(f"error: cannot read {path!r}: {exc}")


def ascii_sum(line):
    """Return the sum of the ASCII values of the characters in `line`."""
    return sum(map(ord, line))


def main():
    parser = argparse.ArgumentParser(
        description=(
            "Add two files line by line: each output line is the sum of "
            "the ASCII values of the corresponding lines of the two inputs."
        )
    )
    parser.add_argument("file1", help="first input file")
    parser.add_argument("file2", help="second input file")
    parser.add_argument(
        "-o", "--output",
        default="output.txt",
        help="output file (default: %(default)s)",
    )
    args = parser.parse_args()

    lines1 = read_lines(args.file1)
    lines2 = read_lines(args.file2)

    if len(lines1) != len(lines2):
        shorter = min(len(lines1), len(lines2))
        print(
            f"warning: files have different line counts "
            f"({len(lines1)} vs {len(lines2)}); "
            f"only the first {shorter} lines are added",
            file=sys.stderr,
        )

    results = [
        ascii_sum(a) + ascii_sum(b) for a, b in zip(lines1, lines2)
    ]

    try:
        with open(args.output, "w", encoding="ascii") as f:
            f.writelines(f"{r}\n" for r in results)
    except OSError as exc:
        sys.exit(f"error: cannot write {args.output!r}: {exc}")

    print(f"wrote {len(results)} sums to {args.output!r}")


if __name__ == "__main__":
    main()
