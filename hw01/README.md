# HW1: Bit Manipulation

This folder has my bit-manipulation functions (bits.c / bits.h)
and a test file (tests/test_bits.c).

## Building and testing

From inside hw01:

    gcc -std=c11 -Wall -Wextra bits.c tests/test_bits.c -o tests/test_bits
    ./tests/test_bits

## Valid inputs

Bits are numbered from 0 on the right.

- width has to be 1 to 32
- pos has to be 0 to 31
- pos + width can't be more than 32 (the field has to fit inside the word)

## What happens with bad input

**get_field** returns 0 if any of the rules above are broken. There are
no real bits outside the 32-bit word, so 0 is the honest answer, and it
never reads anything that doesn't exist. The downside: a valid field
that's all zeros also returns 0, so you can't tell the two apart just
from the result.

**print_binary** prints just a blank line if width is 0 or negative,
since the loop never runs. If width is over 32, it gets cut down to 32,
because a uint32_t only has 32 bits to show.

## Edge cases

**Width 32 in get_field.** The mask is normally (1u << width) - 1, which
gives "width" ones in a row. But shifting a 32-bit number by 32 is
undefined in C. On most PCs, 1u << 32 actually turns into 1u << 0, so
the mask would come out as 0 and you'd get 0 back. So when width is 32,
I skip the formula and use 0xFFFFFFFF (all ones) instead.S