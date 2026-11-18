# HW7: A Growing Stack and a Bracket Checker

This folder has a stack of `int` that grows on the heap (malloc, realloc,
free), a bracket checker that uses it, and the bugs I made on purpose for
the sanitizers to catch.

- istack.h / istack.c: the istack ADT (istack_t is opaque)
- balance.c: `./balance < file`, the bracket checker
- tests/test_istack.c: the unit tests
- inputs/ and expected/: the regression inputs, and the output line and
  exit status each one should give
- regress.sh: runs ./balance on every input and compares with expected/

## Building and testing

From inside hw07:

    make                         # builds balance, no warnings
    ./balance < inputs/deep.txt  # checks one file
    make test                    # unit tests, then sh regress.sh
    make memcheck                # rebuilds with -fsanitize=address,undefined, runs make test
    make clean                   # deletes everything the build made

The unit tests print PASS or FAIL per test and a summary, and exit
nonzero if any fails. `make test` fails if any unit test or any
regression input fails. `make memcheck` cleans first, so that every
file is rebuilt with the sanitizers, and cleans again after. It finds
leaks on Linux only. On macOS, use
`leaks --atExit -- ./tests/test_istack` after `make test`.

## balance output

    balanced: 8 pairs, deepest 4                   exit status 0
    line 2, column 10: found ')', expected ']'     exit status 1
    line 1, column 12: found ')', nothing open     exit status 1
    end of input: 3 still open, innermost '('      exit status 1

Every bracket counts, also in comments and strings. Lines and columns
count from 1, and it stops at the first error. If memory runs out it
prints to stderr and exits with 2. Every way out of the loop goes
through one `istack_destroy`, so the stack is always freed.

## Design notes

**One array on the heap.** `struct istack` holds `data`, `count`, and
`cap`, and is completed only in istack.c. `data[0]` is the bottom, so
the top is `data[count - 1]`, and `istack_to_array` is one `memcpy`.

**The room starts at 8 and doubles.** When `count == cap`, push calls
realloc for `cap * 2`. Popping never shrinks it.

**realloc into a second pointer.** `int *bigger = realloc(s->data, ...)`.
If realloc fails, it returns NULL but the old array is still there.
Writing `s->data = realloc(s->data, ...)` would overwrite the only
pointer to the old array with NULL: every value is lost and the old
array leaks. With `bigger`, a failure returns -1 and the stack is
unchanged.

**create frees what it already got.** If the second malloc (the array)
fails, it frees the struct before returning NULL, so nothing leaks.

**destroy frees the inside first.** `free(s->data)` then `free(s)`.
In the other order, `s->data` would be read from memory that was
already freed.

**to_array is a copy the caller owns.** It is a new malloc, so changing
it does not change the stack, and the caller must free it.

## Part 4: breaking it on purpose

Each bug was made in turn, run under `make memcheck` on Linux, and then
undone. The code in this folder passes `make memcheck` cleanly.

**(1) Destroy the same stack twice.** I added a second
`istack_destroy(x);` in tests/test_istack.c.

    ==454==ERROR: AddressSanitizer: heap-use-after-free on address 0x5030000001c0 at pc 0x56474b4d2c16 bp 0x7fff12dee520 sp 0x7fff12dee510

The first destroy freed the struct, and the second one read
`s->data` out of that freed struct (istack.c line 38), so ASan stopped
it there, before the second `free` could even happen.

**(2) Push onto a stack after destroying it.** I added
`istack_push(x, 1);` right after `istack_destroy(x);`.

    ==480==ERROR: AddressSanitizer: heap-use-after-free on address 0x5030000001c8 at pc 0x56414cf5ecd0 bp 0x7ffcced92a90 sp 0x7ffcced92a80

Push read `s->count` (8 bytes into the 24-byte struct) from memory that
destroy had already given back to the heap.

**(3) Leave out istack_destroy in balance.c.**

    ==511==ERROR: LeakSanitizer: detected memory leaks

When balance exited, the struct (24 bytes, from istack_create) and its
array were still allocated with nothing pointing to them anymore.
LeakSanitizer reported them and changed the exit status, so every
regression input failed.
