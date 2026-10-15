#include <stdio.h>
#include "iom361_r4.h"

#define NUM_SWITCHES 3   /* ADD_30, START, DOOR_OPEN */
#define NUM_LEDS     2   /* MAGNETRON, LAMP */

int main(void)
{
    int rc;
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_LEDS, &rc);
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }
    printf("Microwave oven\n");
    return 0;
}