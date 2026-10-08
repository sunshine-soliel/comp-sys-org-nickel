#include <stdio.h>
#include "bits.h"

void print_binary(uint32_t x, int width)
{
    //check that the width isn't more than 32 bits
    if (width > 32) {
        width = 32;
    }
    //checks what each bit should be and prints it one at a time
    for (int i = width - 1; i >= 0; i--) {
        if ((x >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
        //adds a space every 4 bits
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

uint32_t get_field(uint32_t word, int pos, int width)
{
    /* Check for bad input and return 0 if bad
    every way the input can be bad:
    width is less than 1
    width is more than 32
    pos is less than 0 (negative positions don't exist)
    pos is more than 31
    the field hangs off the edge: pos + width is more than 32 */
    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32) {
        return 0;
    }

    // Mask of `width` ones. 1u << 32 is undefined, so width 32 is special.
    uint32_t mask;
    if (width == 32) {
        mask = 0xFFFFFFFF;
    } else {
        mask = (1u << width) - 1;
    }

    // Slide the field down to position 0, then erase everything above it
    return (word >> pos) & mask;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    // Bad input: the field doesn't exist, so change nothing and return word
    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32) {
        return word;
    }

    // Ones at the bottom, exact same as get_field
    uint32_t mask;
    if (width == 32) {
        mask = 0xFFFFFFFF;
    } else {
        mask = (1u << width) - 1;
    }

    // Move the ones up so they sit on the field
    mask = mask << pos;

    // Erase the field, then write the trimmed value into it
    return (word & ~mask) | ((value << pos) & mask);
}

int32_t sign_extend(uint32_t value, int width)
{
    // Width must be 1-32
    if (width < 1 || width > 32) {
        return 0;
    }

    // Keep only the bottom `width` bits
    uint32_t v = get_field(value, 0, width);

    // Sign bit is the leftmost bit we're reading (position width - 1)
    int sign_bit = (v >> (width - 1)) & 1;

    // Negative: real value is v - 2^width
    if (sign_bit == 1) {
        // Use 64 bits, since 2^32 doesn't fit in 32
        int64_t big_v = (int64_t)v;
        int64_t two_to_the_width = (int64_t)1 << width;
        int64_t result = big_v - two_to_the_width;

        // Answer always fits back in 32 bits
        return (int32_t)result;
    }

    // Positive: already correct
    return (int32_t)v;
}