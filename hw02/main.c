#include <stdio.h>
#include <stdlib.h>
#include "reading.h"
#include "stats.h"
#include "histogram.h"

int main(int argc, char *argv[])
{
    /* the readings live here now, not in globals */
    int   ticks[READING_MAX];
    float temps[READING_MAX];
    float hums[READING_MAX];
    float threshold = 30.0f;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }

    reading_result_t r = reading_read(ticks, temps, hums, READING_MAX);
    printf("readings: %d\n", r.count);
    printf("skipped:  %d\n", r.skipped);
    if (r.count == 0) {
        printf("no readings, no summary\n");
        return 0;
    }

    /* one function per statistic, each called twice */
    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n",
           stats_min(temps, r.count), stats_max(temps, r.count), stats_mean(temps, r.count));
    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n",
           stats_min(hums, r.count), stats_max(hums, r.count), stats_mean(hums, r.count));

    stats_run_t run = stats_longest_run_above(temps, r.count, threshold);
    if (run.length == 0)
        printf("above %.1f C: never\n", threshold);
    else
        printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
               threshold, run.length, ticks[run.start], ticks[run.start + run.length - 1]);

    histogram_print(temps, r.count);
    return 0;
}