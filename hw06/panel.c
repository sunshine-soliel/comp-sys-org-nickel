#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    // on now AND off before
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before)
{
    // off now AND on before
    return ~now & before;
}

int panel_led(uint32_t *base, int n, int on)
{
    if (n < 0 || n > 31) {
        return -1;
    }
    int rc = 0;

    // Read: what the LEDs show right now
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    // Modify: change bit n only
    uint32_t bit = 1u << n;
    if (on) {
        leds = leds | bit;     // set bit n
    } else {
        leds = leds & ~bit;    // clear bit n
    }

    // Write: put it back, with every other bit as it was
    iom361_writeReg(base, LEDS_REG, leds, &rc);
    return rc;
}

int panel_toggle(uint32_t *base, int n)
{
    if (n < 0 || n > 31) {
        return -1;
    }
    int rc = 0;

    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    leds = leds ^ (1u << n);   // XOR flips bit n: 0 -> 1, 1 -> 0

    iom361_writeReg(base, LEDS_REG, leds, &rc);
    return rc;
}
