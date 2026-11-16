#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/* The switches that went from off (0) to on (1): one bit per switch.
   A bit is set if it is 1 now and was 0 before. */
uint32_t panel_pressed(uint32_t now, uint32_t before);

/* The switches that went from on (1) to off (0). */
uint32_t panel_released(uint32_t now, uint32_t before);

/* Turns LED n on (on != 0) or off (on == 0) by read-modify-write,
   so every other LED keeps its state.
   Returns 0, -1 if n is not 0 to 31, or the iom361 return code if a
   register read or write fails. */
int panel_led(uint32_t *base, int n, int on);

/* Toggles LED n the same way. Same return values as panel_led. */
int panel_toggle(uint32_t *base, int n);

#endif
