#include <stdio.h>
#include <string.h>
#include "scenario.h"

// True if the line has nothing on it but spaces, or is a comment
static int is_blank_or_comment(const char *line)
{
    // Skip spaces and tabs at the front
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    return *line == '\0' || *line == '\n' || *line == '\r' || *line == '#';
}

int scenario_load(const char *path, scenario_t *s)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return 1;
    }

    s->start = 0;
    s->count = 0;

    char line[256];
    int line_no = 0;      // counts every line in the file, comments too
    int have_start = 0;

    while (fgets(line, sizeof line, f) != NULL) {
        line_no++;
        if (is_blank_or_comment(line)) {
            continue;
        }

        // The first real line must be the start line
        if (!have_start) {
            long long start;
            if (sscanf(line, " start %lld", &start) != 1) {
                fclose(f);
                return line_no;
            }
            s->start = (time_t)start;
            have_start = 1;
            continue;
        }

        // Every other line: tick temp rh switches (4 fields)
        scenario_line_t L;
        if (sscanf(line, "%d %f %f %x", &L.tick, &L.temp_c, &L.rh_pct, &L.switches) != 4) {
            fclose(f);
            return line_no;            // a field is missing or wrong
        }
        if (L.tick < 0) {
            fclose(f);
            return line_no;
        }
        if (s->count > 0 && L.tick <= s->lines[s->count - 1].tick) {
            fclose(f);
            return line_no;            // ticks must increase
        }
        if (s->count == SCENARIO_MAX_LINES) {
            fclose(f);
            return line_no;            // too many lines
        }
        s->lines[s->count] = L;
        s->count++;
    }
    fclose(f);

    if (!have_start) {
        // No start line anywhere: report the line after the last one
        return line_no + 1;
    }
    return 0;
}

const scenario_line_t *scenario_at(const scenario_t *s, int t)
{
    const scenario_line_t *found = NULL;
    for (int i = 0; i < s->count; i++) {
        if (s->lines[i].tick <= t) {
            found = &s->lines[i];      // this line has started
        } else {
            break;                     // ticks increase, so no later line can match
        }
    }
    return found;
}

int scenario_last_tick(const scenario_t *s)
{
    if (s->count == 0) {
        return -1;
    }
    return s->lines[s->count - 1].tick;
}
