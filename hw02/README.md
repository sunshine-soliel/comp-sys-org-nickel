# HW2: Splitting a Program into Modules

This folder has the HW2 starter program (readings) split into modules:
reading, stats, histogram, and main. It prints exactly the same output
as the original readings.c, byte for byte.

## Building and testing

From inside hw02:

    make         # builds ./readings, one .o file per .c file
    make test    # builds and runs the stats unit tests, then runs regress.sh
    make clean   # deletes everything the build made

Run the program like this:

    ./readings < data/normal.txt
    ./readings 25 < data/edge.txt      # threshold of 25 C instead of 30

tests/test_stats prints PASS or FAIL for each test and a summary, and
exits with 1 if any test fails. regress.sh compares the program's output
with the files in expected/ and fails on any difference.

## Module diagram

Each arrow means "calls". Only main calls the other modules, and none of
them call each other.

                 +--------+
                 |  main  |
                 +--------+
                /    |     \
               v     v      v
       +---------+ +-------+ +-----------+
       | reading | | stats | | histogram |
       +---------+ +-------+ +-----------+

| Module    | Files                      | Responsible for                                                    |
|-----------|----------------------------|--------------------------------------------------------------------|
| reading   | reading.h, reading.c       | reading input lines, skipping comments, blank lines, and bad lines |
| stats     | stats.h, stats.c           | min, max (recursive), mean, longest run above a threshold          |
| histogram | histogram.h, histogram.c   | printing the temperature histogram                                 |
| main      | main.c                     | the command line, calling the modules, printing the results        |

## Design choices

**No globals.** The arrays live in main and get passed to the functions.
When you pass an array, C passes its address, so reading_read fills in
main's arrays directly.

**Returning two results.** reading_read needs to return count and
skipped, and stats_longest_run_above needs to return the length and
where the run starts. A function only returns one thing, so each one
returns a small struct (reading_result_t and stats_run_t).

**stats returns indices, not ticks.** stats_longest_run_above only works
on the temperature array, so it returns the index where the run starts.
main turns that into tick numbers. That way stats doesn't need to know
what a tick is.

**One recursive max.** stats_max(a, n) returns the larger of the last
value and stats_max(a, n-1), and stops at n == 1. It's used for both
temperature and humidity. Every call stays on the stack until the one
below it returns, so it uses one stack frame per value. A huge array
(hundreds of thousands of values) will overflow the stack.

**Private helpers are static.** is_ignored (reading.c), larger (stats.c),
and bin_index (histogram.c) are only used in their own file, so they're
static and don't show up in any header. LINE_LEN, NUM_BINS, and
BIN_WIDTH are also kept inside their .c files. READING_MAX is in
reading.h because main needs it to size the arrays.

**Same output.** The printf format strings are copied exactly from the
starter, and the mean still adds up floats left to right like the
original, so the output matches expected/ exactly.