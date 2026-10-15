#include <stdio.h>
#include <stdbool.h>
#include "../stats.h"

#define BIG_N 10000

/* Prints PASS or FAIL for one test.
   Returns 1 if the test failed, 0 if it passed, so main can count failures. */
static int check(bool ok, const char *name)
{
    printf("%s  %s\n", ok ? "PASS" : "FAIL", name);
    return ok ? 0 : 1;
}

int main(void)
{
    int failed = 0;
    int total = 0;

    /* 1. one value: min, max and mean are all that value */
    float one[] = {21.5f};
    failed += check(stats_min(one, 1) == 21.5f,  "one value: min");   total++;
    failed += check(stats_max(one, 1) == 21.5f,  "one value: max");   total++;
    failed += check(stats_mean(one, 1) == 21.5f, "one value: mean");  total++;

    /* 2. maximum at the start and at the end */
    float first[] = {9.0f, 1.0f, 2.0f, 3.0f};
    float last[]  = {1.0f, 2.0f, 3.0f, 9.0f};
    failed += check(stats_max(first, 4) == 9.0f, "max at the start"); total++;
    failed += check(stats_max(last, 4) == 9.0f,  "max at the end");   total++;

    /* 3. a value equal to the threshold ends a run:
          31 32 | 30 | 31 32 33  -> longest run is 3, starting at index 3 */
    float temps[] = {31.0f, 32.0f, 30.0f, 31.0f, 32.0f, 33.0f};
    stats_run_t run = stats_longest_run_above(temps, 6, 30.0f);
    failed += check(run.length == 3 && run.start == 3, "value equal to threshold ends a run"); total++;

    /* no value above the threshold: length 0 */
    run = stats_longest_run_above(temps, 6, 50.0f);
    failed += check(run.length == 0, "no run above threshold"); total++;

    /* 4. recursive max on 10,000 values, the largest one in the middle */
    static float big[BIG_N];   /* static: too big to put on the stack comfortably */
    for (int i = 0; i < BIG_N; i++)
        big[i] = (float) (i % 100);
    big[BIG_N / 2] = 500.0f;
    failed += check(stats_max(big, BIG_N) == 500.0f, "recursive max on 10,000 values"); total++;

    printf("%d of %d tests passed\n", total - failed, total);
    return failed == 0 ? 0 : 1;
}