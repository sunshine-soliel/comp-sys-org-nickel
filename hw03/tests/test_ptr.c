#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "../ptr.h"

// Running totals, shared by every test
int pass_count = 0;
int fail_count = 0;

// Prints PASS or FAIL for one test and adds to the totals.
void check(bool result, const char *name)
{
    if (result == true) {
        printf("PASS  %s\n", name);
        pass_count = pass_count + 1;
    } else {
        printf("FAIL  %s\n", name);
        fail_count = fail_count + 1;
    }
}

int main(void)
{
    // ----- swap_float -----
    float x = 1.5f;
    float y = -2.0f;
    swap_float(&x, &y);
    check(x == -2.0f && y == 1.5f, "swap_float basic");

    // ----- minmax -----
    float vals[] = {3.0f, -1.5f, 7.25f, 0.0f, 2.0f};
    float lo = 0.0f;
    float hi = 0.0f;
    int r = minmax(vals, 5, &lo, &hi);
    check(r == 0 && lo == -1.5f && hi == 7.25f, "minmax several values");

    float one[] = {4.5f};
    r = minmax(one, 1, &lo, &hi);
    check(r == 0 && lo == 4.5f && hi == 4.5f, "minmax one value");

    // Outputs start at 99 so we can tell they weren't touched
    lo = 99.0f;
    hi = 99.0f;
    r = minmax(vals, 0, &lo, &hi);
    check(r == -1 && lo == 99.0f && hi == 99.0f, "minmax n = 0 (unchanged)");

    r = minmax(NULL, 5, &lo, &hi);
    check(r == -1 && lo == 99.0f && hi == 99.0f, "minmax NULL array (unchanged)");

    r = minmax(vals, 5, NULL, &hi);
    check(r == -1 && hi == 99.0f, "minmax NULL min (unchanged)");

    r = minmax(vals, 5, &lo, NULL);
    check(r == -1 && lo == 99.0f, "minmax NULL max (unchanged)");

    // ----- find_above -----
    float data[] = {1.0f, 5.0f, 2.0f, 8.0f};
    const float *found = find_above(data, 4, 4.0f);
    check(found == &data[1], "find_above two matches returns first");

    found = find_above(data, 4, 10.0f);
    check(found == NULL, "find_above no match");

    // 5.0 equals the threshold, so it must be skipped; 8.0 is next
    found = find_above(data, 4, 5.0f);
    check(found == &data[3], "find_above equal is not above");

    found = find_above(NULL, 4, 0.0f);
    check(found == NULL, "find_above NULL array");

    // ----- reverse -----
    float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
    reverse(even, 4);
    check(even[0] == 4.0f && even[1] == 3.0f &&
          even[2] == 2.0f && even[3] == 1.0f, "reverse even length");

    float odd[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    reverse(odd, 5);
    check(odd[0] == 5.0f && odd[1] == 4.0f && odd[2] == 3.0f &&
          odd[3] == 2.0f && odd[4] == 1.0f, "reverse odd length");

    float single[] = {7.0f};
    reverse(single, 1);
    check(single[0] == 7.0f, "reverse n = 1 does nothing");

    reverse(NULL, 4);
    check(true, "reverse NULL doesn't crash");

    // ----- Summary -----
    printf("\n%d passed, %d failed\n", pass_count, fail_count);

    // Exit code: 0 means everything passed, 1 means something failed
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}
