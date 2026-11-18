# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- Claude wrote the code (istack.h, istack.c, balance.c), the test file, the Makefile, regress.sh, the inputs and expected files, and a draft of the README, in the same style as my HW6.
- Claude made each Part 4 bug in a copy, ran make memcheck, and copied the first line of each report into the README.
- I read through each file, and ran make, make test, make memcheck, and make clean myself. <!-- FILL IN: only keep this line if you did it, and add something specific you checked, e.g. traced the 9th push by hand -->

## Something the AI got wrong or I had to fix
- Claude expected bug (1), destroying a stack twice, to be reported as a double free. ASan actually reported heap-use-after-free, because the second istack_destroy reads s->data from the struct that was already freed before it ever reaches the second free.
