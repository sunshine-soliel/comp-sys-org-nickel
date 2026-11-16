#include <stdio.h>
#include <time.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

#define NUM_SWITCHES 4
#define NUM_LEDS     4

// static: a scenario_t is about 16 KB, too big to be comfortable on the stack
static scenario_t scen;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    int bad = scenario_load(argv[1], &scen);
    if (bad != 0) {
        fprintf(stderr, "%s: error on line %d\n", argv[1], bad);
        return 1;
    }

    int rc;
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_LEDS, &rc);
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }
    iom361_set_display(false);   // keep the LED display out of the trace

    uint32_t before = 0;         // the switches on the previous tick
    int last = scenario_last_tick(&scen);

    for (int tick = 0; tick <= last; tick++) {
        // 1. Set the "world": only main does this, never sensor.c or panel.c
        const scenario_line_t *L = scenario_at(&scen, tick);
        if (L != NULL) {
            _iom361_set_sensor1(L->temp_c, L->rh_pct);
            _iom361_set_switches(L->switches);
        }

        // 2. The clock
        time_t now = scen.start + tick;

        // 3. Read the sensor and the switches through the registers
        reading_t r;
        if (sensor_read(base, now, &r) != 0) {
            fprintf(stderr, "sensor_read failed at tick %d\n", tick);
            return 1;
        }
        uint32_t sw = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc != 0) {
            fprintf(stderr, "switch read failed at tick %d\n", tick);
            return 1;
        }

        // 4. A press of switch k toggles LED k
        uint32_t pressed = panel_pressed(sw, before);
        for (int k = 0; k < NUM_SWITCHES; k++) {
            if (pressed & (1u << k)) {
                panel_toggle(base, k);
            }
        }
        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

        // 5. Print one trace line, time in UTC
        char when[32];
        strftime(when, sizeof when, "%Y-%m-%d %H:%M:%S", gmtime(&r.t));
        printf("%s temp=%.1f rh=%.1f switches=0x%X pressed=0x%X leds=0x%X\n",
               when, r.temp_c, r.rh_pct, (unsigned)sw, (unsigned)pressed, (unsigned)leds);

        before = sw;
    }
    return 0;
}
