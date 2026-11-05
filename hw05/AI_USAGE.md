# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- Claude walked me through the assignment one step at a time: the
  state diagram and transition table first, then lock.h, lock.c,
  main.c, and data/codes.txt. Claude explained each piece and gave me
  the code, and I typed lock.h, lock.c, and main.c in myself.
- Claude wrote the Makefile and the test file (tests/test_lock.c).
  I ran make, make test, and make clean to check that they work.
- Claude drafted the README. I proofread and edited it.

## Something the AI got wrong or I had to fix
- Claude's build commands for main.c assumed lock.o already existed,
  but I hadn't built it yet, so the linker said "cannot find lock.o."
  I had to compile lock.c first.
