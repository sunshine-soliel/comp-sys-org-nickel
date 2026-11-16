#include <math.h>
#include "sensor.h"
#include "iom361_r4.h"

// The sensor values are 20 bits: bits 19..0
#define SENSOR_MASK  0xFFFFFu        // twenty 1s, same as get_field(raw, 0, 20)
#define TWO_TO_20    1048576.0f      // 2^20, a float so the division is a float division

// Rounds x to the nearest 0.1: 90.99998 -> 910 -> 91.0
static float round_tenth(float x)
{
    return roundf(x * 10.0f) / 10.0f;
}

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc = 0;

    // Read both registers into local variables first.
    // If either read fails, return before touching *r, so it stays unchanged.
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc;
    }
    uint32_t raw_rh = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    // Keep bits 19..0 only
    uint32_t st  = raw_temp & SENSOR_MASK;
    uint32_t srh = raw_rh & SENSOR_MASK;

    // Formulas from iom361_r4.h
    float temp = (st / TWO_TO_20) * 200.0f - 50.0f;
    float rh   = (srh / TWO_TO_20) * 100.0f;

    // Both reads worked: now fill in *r
    r->t = now;
    r->temp_c = round_tenth(temp);
    r->rh_pct = round_tenth(rh);
    return 0;
}
