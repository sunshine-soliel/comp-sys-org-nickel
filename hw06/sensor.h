#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

// One reading: when it was taken, and what the sensor said
typedef struct {
    time_t t;        // timestamp, seconds since 1 Jan 1970 UTC
    float  temp_c;   // temperature in degrees C, rounded to 0.1
    float  rh_pct;   // relative humidity in %, rounded to 0.1
} reading_t;

/* Reads TEMP_REG and HUMID_REG, keeps bits 19..0 of each, converts them
   to degrees C and %RH, rounds to 0.1, and stores them with the
   timestamp now in *r.
   Returns 0, or the iom361 return code of the first read that failed.
   On failure *r is not changed. */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
