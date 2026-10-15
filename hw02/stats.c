#include "stats.h"

/* private helper: only used in this file, so it is static */
static float larger(float x, float y)
{
    return x > y ? x : y;
}

float stats_min(const float a[], int n)
{
    float m = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] < m)
            m = a[i];
    return m;
}

float stats_max(const float a[], int n)
{
    if (n == 1)                              /* base case: one value */
        return a[0];
    return larger(a[n - 1], stats_max(a, n - 1));   /* last value vs. max of the rest */
}

float stats_mean(const float a[], int n)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return sum / n;
}

stats_run_t stats_longest_run_above(const float a[], int n, float threshold)
{
    stats_run_t best = {0, -1};
    int run = 0, start = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > threshold) {
            if (run == 0)
                start = i;
            run++;
            if (run > best.length) {
                best.length = run;
                best.start = start;
            }
        } else {
            run = 0;
        }
    }
    return best;
}