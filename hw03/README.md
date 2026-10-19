# HW3: Pointers

This folder has my pointer library (ptr.c / ptr.h) and a test
program (tests/test_ptr.c).

## Building and testing

From inside hw03:

    make         # builds ptr.o and tests/test_ptr
    make test    # builds tests/test_ptr and runs it
    make clean   # deletes everything the build made

The test program prints PASS or FAIL for each test, then a summary.
It exits with 1 if any test fails, and 0 if they all pass.

## The functions

**swap_float** gets the addresses of two floats, so it can change the
caller's variables directly. Without the `*`, it would only get copies,
swap the copies, and the caller would see no change.

**minmax** has to give back two answers, but a function can only return
one value. So the caller passes the addresses of min and max, and
minmax stores the answers there. The return value is just 0 (worked) or
-1 (bad input). On bad input it returns before writing anything, so the
caller's variables stay exactly how they were. There's no float value
that could mean "no answer", which is why the error is a separate
return code instead of something stored in *min.

**find_above** returns a pointer into the array, so the caller knows
where the value is, not just what it is. It returns NULL if nothing is
strictly greater than the threshold. Equal doesn't count.

**reverse** starts one pointer at each end, swaps them with swap_float,
then moves them toward each other until they meet. With an odd length
the middle element is left alone, since it's already in place.

## const

minmax and find_above take `const float *a` because they only read
the array. The compiler will stop me if I accidentally write to it,
and the caller knows their array is safe. find_above returns a
`const float *` for the same reason: it points into a read-only array.
reverse and swap_float change the data, so they don't use const.

## Pointer arithmetic

ptr.c never uses `a[i]`. Loops walk a pointer `p` from `a` up to
(but not including) `a + n`, and read the value with `*p`.
