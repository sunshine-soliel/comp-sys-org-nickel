#ifndef BITS_H
#define BITS_H

#include <stdint.h>

/* Prints the lowest width bits of x in binary, 
most significant bit first, 
groups of 4 bits separated by spaces */
void print_binary(uint32_t x, int width);

/*Returns bits pos to pos+width-1 of word, 
shifted down to bit 0*/
uint32_t get_field(uint32_t word, int pos, int width);

/*Returns word with bits pos to pos+width-1
replaced by the lowest width bits of value
All other bits remain unchanged */
uint32_t set_field(uint32_t word, int pos, int width, uint32_t value);

#endif