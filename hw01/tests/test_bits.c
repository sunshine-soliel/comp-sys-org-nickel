#include "../bits.h"
#include <stdio.h>

int main(void)
{
    print_binary(5, 4);
    print_binary(5, 2);
    print_binary(0xB6, 8);
    print_binary(0xFFFFFFFF, 40); // should print only the lowest 32 bits due to width limit

    printf("%u\n", get_field(0xB6, 2, 3));
    printf("%u\n", get_field(0xFFFFFFFF, 0, 32));
    printf("%u\n", get_field(0xFFFFFFFF, 31, 2));
    printf("%u\n", get_field(0xFFFFFFFF, 0, 33)); // should return 0 due to bad input
    printf("%u\n", get_field(0xFFFFFFFF, -1, 2)); // should return 0 due to bad input
    printf("%u\n", get_field(0xFFFFFFFF, 0, 0)); // should return 0 due to bad input

    printf("%X\n", set_field(0xB6, 2, 3, 2)); 
    printf("%X\n", set_field(0xB6, 4, 4, 0x3)); 
    printf("%X\n", set_field(0xB6, 4, 4, 0x13)); 
    printf("%X\n", set_field(0xB6, 30, 4, 0xF)); 

    printf("%d\n", sign_extend(0xF8, 8));
    printf("%d\n", sign_extend(0x16, 8));
    printf("%d\n", sign_extend(0xFFFFFF05, 4));
    printf("%d\n", sign_extend(0x0A, 4));
    printf("%d\n", sign_extend(0x80, 8));
    printf("%d\n", sign_extend(0x80000000, 32));

    return 0;
}