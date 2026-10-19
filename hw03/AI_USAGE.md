# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- I wrote ptr.h and ptr.c with Claude's help, using pointer
  arithmetic instead of a[i].
- Claude wrote the test file (tests/test_ptr.c), the Makefile, and
  drafted the README. I read through them and checked that make,
  make test, and make clean all work.

## Something the AI got wrong or I had to fix
- When Claude first wrote ptr.h and ptr.c, it saved them in the wrong
  folder (my read-only uploads folder instead of hw03/), so they had
  to be copied into hw03/ before the Makefile could find them.
