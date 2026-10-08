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

**set_field** gives back the word unchanged if the field doesn't fit.
Returning 0 would wipe out every other bit in the word, which is a big
punishment for a small mistake. If the field doesn't exist, there's
nothing to set, so it just doesn't change anything.

**sign_extend** returns 0 if width isn't 1 to 32, since there's no
sensible number to read.

**print_binary** prints just a blank line if width is 0 or negative,
since the loop never runs. If width is over 32, it gets cut down to 32,
because a uint32_t only has 32 bits to show.

## Edge cases

**Width 32 in get_field and set_field.** The mask is normally
(1u << width) - 1, which gives "width" ones in a row. But shifting a
32-bit number by 32 is undefined in C. On most PCs, 1u << 32 actually
turns into 1u << 0, so the mask would come out as 0. So when width is
32, I skip the formula and use 0xFFFFFFFF (all ones) instead.

**Value too big for the field in set_field.** If value has more bits
than width, the extra bits would spill into the neighboring bits when
you shift it into place. So after shifting, I AND it with the field
mask, which chops off anything outside the field. For example,
set_field(0xB6, 4, 4, 0x13) gives 0x36, not 0x136.

**How sign_extend works.** In two's complement, the leftmost bit counts
as negative. When C reads the bits as a normal unsigned number, that bit
counts as positive instead, so the number comes out too big by exactly
2^width. So if the sign bit is 1, I subtract 2^width. For example, 0xF8
read as 8 bits is 248, and 248 - 256 = -8.

**Width 32 in sign_extend.** 2^32 doesn't fit in 32 bits, so I do the
subtraction with 64-bit numbers (int64_t) and then convert back to
int32_t. The answer always fits, even the most negative one
(sign_extend(0x80000000, 32) = -2147483648).