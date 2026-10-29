# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- Ran short on time this week, so I had Claude generate the code
  (tally.h, tally.c, events.c, tests/test_tally.c, Makefile, and 
  data/door.txt).
- I built and tested it myself on my machine with make, make test,
  and make clean, and ran ./events on the sample data.
- I reviewed the code before pushing, focusing on tally_add (finding
  the insert spot, shifting entries from the end so nothing gets
  overwritten, and checking the length before memcpy) and on why
  the opaque type blocks t->n outside tally.c.

## Something the AI got wrong or I had to fix
- When Claude ran the Part 3 experiment to get the compiler error,
  its first attempt wrote the test file to the wrong directory, so
  gcc failed with "peek.c: No such file or directory" instead of the
  opaque-type error. It had to recreate the file inside hw04 and
  compile again to get the real message that's in the README.