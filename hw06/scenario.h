#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

// One line of a scenario file: tick temp rh switches
typedef struct {
    int      tick;       // seconds since the start
    float    temp_c;     // temperature in degrees C
    float    rh_pct;     // relative humidity in %
    uint32_t switches;   // switch register, written in hex in the file
} scenario_line_t;

// A whole scenario: the start time and up to 1000 lines, no malloc
typedef struct {
    time_t          start;
    int             count;                      // how many lines are used
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

/* Loads the scenario file at path into *s.
   Format: lines starting with # are comments, blank lines are skipped,
   the first other line is "start <unix timestamp>", and every line after
   it is "tick temp rh switches" with switches in hex and ticks increasing.
   Returns 0, or the line number (counting from 1) of the first wrong
   line. If the file cannot be opened, or has no start line, returns 1. */
int scenario_load(const char *path, scenario_t *s);

/* Returns the line in force at tick t: the last line whose tick is <= t,
   since the values hold between lines. Returns NULL if there is none
   (no lines, or t is before the first line). */
const scenario_line_t *scenario_at(const scenario_t *s, int t);

/* The last tick in the scenario, or -1 if it has no lines. */
int scenario_last_tick(const scenario_t *s);

#endif
