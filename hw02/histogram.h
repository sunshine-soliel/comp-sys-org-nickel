#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/* Prints a histogram of a[0..n-1] in 5 C bins from 0 to 100 C.
   Values below 0 go in the first bin, values of 100 or more in the last.
   Empty bins are not printed. Assumes n >= 0. */
void histogram_print(const float a[], int n);

#endif