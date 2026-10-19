#include <stddef.h>
#include "ptr.h"

void swap_float(float *a, float *b)
{
    // Nothing to swap if either pointer is missing
    if (a == NULL || b == NULL) {
        return;
    }

    // Save *a, copy *b into it, then put the saved value in *b
    float temp = *a;
    *a = *b;
    *b = temp;
}

int minmax(const float *a, int n, float *min, float *max)
{
    // Bad input: return -1 BEFORE touching *min or *max
    if (a == NULL || min == NULL || max == NULL || n < 1) {
        return -1;
    }

    // Start both answers at the first value
    float smallest = *a;
    float largest = *a;

    // Walk the rest of the array with a pointer
    for (const float *p = a + 1; p < a + n; p++) {
        if (*p < smallest) {
            smallest = *p;
        }
        if (*p > largest) {
            largest = *p;
        }
    }

    // Only write to the caller's variables once we're done
    *min = smallest;
    *max = largest;
    return 0;
}

const float *find_above(const float *a, int n, float threshold)
{
    if (a == NULL || n < 1) {
        return NULL;
    }

    // Return as soon as we find one, so it's always the first match
    for (const float *p = a; p < a + n; p++) {
        if (*p > threshold) {
            return p;
        }
    }

    // Got to the end without finding one
    return NULL;
}

void reverse(float *a, int n)
{
    if (a == NULL || n < 2) {
        return;
    }

    // One pointer at each end
    float *left = a;
    float *right = a + n - 1;

    // Swap and move toward each other until they meet in the middle
    while (left < right) {
        swap_float(left, right);
        left++;
        right--;
    }
}
