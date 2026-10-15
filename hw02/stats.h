#ifndef STATS_H
#define STATS_H

/* Result of stats_longest_run_above: how long the run is, and the index where it starts. */
typedef struct {
    int length;   /* 0 if no value is above the threshold */
    int start;    /* index of the first value in the run, -1 if length is 0 */
} stats_run_t;

/* Returns the smallest of a[0..n-1]. Assumes n >= 1. */
float stats_min(const float a[], int n);

/* Returns the largest of a[0..n-1], recursively. Assumes n >= 1.
   Uses one stack frame per element, so a very large n can overflow the stack. */
float stats_max(const float a[], int n);

/* Returns the mean of a[0..n-1]. Assumes n >= 1. */
float stats_mean(const float a[], int n);

/* Returns the longest run of consecutive values strictly above threshold.
   A value equal to the threshold ends a run. Assumes n >= 0. */
stats_run_t stats_longest_run_above(const float a[], int n, float threshold);

#endif