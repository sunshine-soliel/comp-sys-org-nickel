# HW4: The tally ADT

This folder has a tally abstract data type (tally.h / tally.c) that
counts how often each name shows up, a program that tallies lines from
standard input (events.c), a sample input file (data/door.txt), and a
test program (tests/test_tally.c).

## Building and testing

From inside hw04:

    make                        # builds the events program
    make test                   # builds tests/test_tally and runs it
    make clean                  # deletes everything the build made
    ./events < data/door.txt    # tally the door events

The test program prints PASS or FAIL for each test, then a summary.
It exits with 1 if any test fails, and 0 if they all pass.

events skips blank lines and lines starting with #. At the end it prints
each name with its count, then `names: N, lines skipped: S`, where S is
how many lines tally_add refused (for example, names longer than 15
characters). For data/door.txt that is `names: 6, lines skipped: 1`.

## How the tally works

**No malloc.** All the tallies live in a `static` array of TALLY_MAX (4)
inside tally.c. Each one has an `in_use` flag. tally_create hands out the
first free one and empties it; tally_destroy just marks it free again.

**Kept sorted.** Each tally has an array of entries (a name and a count),
always in strcmp order. tally_add walks the array to the first entry
that is >= the new name. If it's the same name, it adds 1 to the count.
If it's a new name, every entry from there to the end moves up one slot,
starting from the last one, so nothing gets overwritten before it has
moved. Then the new name goes in the open slot. Because the array is
always sorted, tally_print just prints it in order.

**Copied safely.** tally_add checks the length with strlen first, and
refuses empty names and names longer than TALLY_LEN. Only then does it
copy len + 1 bytes (the name plus its '\0') with memcpy. Each name slot
is TALLY_LEN + 1 bytes, so the copy always fits.

**Full tally.** A full tally refuses a new name, but an existing name
still counts, since it doesn't need a new slot. The check for "already
there" happens before the check for "full."

## Part 3: What the opaque type prevents

I added this line to a file that includes only tally.h:

    tally_t *t = tally_create();
    return t->n;

and gcc said:

    peek.c: In function 'main':
    peek.c:6:13: error: invalid use of incomplete typedef 'tally_t' {aka 'struct tally'}
        6 |     return t->n;
          |             ^~

tally.h only says that `struct tally` exists, not what's inside it, so
any file other than tally.c can hold a `tally_t *` but can't read or
change its members, or even know its size. That means outside code
can't break the rules the tally depends on: it can't change `n` without
moving entries, put names in out of order, write a name without a '\0',
or mark a tally free while someone is still using it. Every change has
to go through tally_add and the other functions, which check their input
and keep the array sorted. It also means I can change the inside later
(like swapping the sorted array for a tree) without changing or even
recompiling events.c, as long as the functions in tally.h stay the same.
I removed the line afterward so the code compiles.
