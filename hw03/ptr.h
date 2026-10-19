#ifndef PTR_H
#define PTR_H

/* Swaps the two floats that a and b point to.
Takes: two pointers to floats in the caller's memory
Returns: nothing (the caller's variables are changed directly)
Assumes: a and b are not NULL (does nothing if either is NULL) */
void swap_float(float *a, float *b);

/* Finds the smallest and largest of a[0..n-1].
Takes: an array a (read only) of length n, and two pointers
       where the answers get stored
Returns: 0 and stores the answers in *min and *max,
         or -1 and changes nothing if n < 1 or any pointer is NULL
Assumes: a really has at least n floats */
int minmax(const float *a, int n, float *min, float *max);

/* Finds the first value in a[0..n-1] that is greater than threshold.
Takes: an array a (read only) of length n, and the threshold
Returns: a pointer to that value inside a,
         or NULL if no value is greater (or a is NULL, or n < 1)
Assumes: a really has at least n floats.
Equal to threshold does NOT count, it has to be strictly greater. */
const float *find_above(const float *a, int n, float threshold);

/* Reverses a[0..n-1] in place.
Takes: an array a of length n
Returns: nothing (the array itself is changed)
Assumes: a really has at least n floats.
Does nothing if a is NULL or n < 2. */
void reverse(float *a, int n);

#endif
