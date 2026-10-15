#include <stdio.h>
#include "reading.h"

#define LINE_LEN 128   /* private: no other file needs it */

/* private helper: 1 if the line is blank or a comment, 0 otherwise */
static int is_ignored(const char line[])
{
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    return line[i] == '\n' || line[i] == '\0' || line[i] == '#';
}

reading_result_t reading_read(int ticks[], float temps[], float hums[], int max)
{
    reading_result_t r = {0, 0};
    char line[LINE_LEN];

    while (fgets(line, sizeof line, stdin) != NULL) {
        if (is_ignored(line))
            continue;
        if (r.count == max) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", max);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[r.count], &temps[r.count], &hums[r.count]) == 3)
            r.count++;
        else
            r.skipped++;
    }
    return r;
}