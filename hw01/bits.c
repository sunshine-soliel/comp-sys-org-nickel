#include <stdio.h>
#include "bits.h"

void print_binary(uint32_t x, int width)
{
    if (width > 32) {
        width = 32;
    }
    for (int i = width - 1; i >= 0; i--) {
        if ((x >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

