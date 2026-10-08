# HW1: Bit Manipulation

This folder has my bit-manipulation functions (bits.c / bits.h),
a thermostat status decoder (status.c / status.h), 
and a test program (tests/test_bits.c).

## Building and testing

From inside hw01:

    make         # compiles each .c file into a .o file
    make test    # builds tests/test_bits and runs it
    make clean   # deletes everything the build made

The test program prints PASS or FAIL for each test, then a summary.
It exits with 1 if any test fails, and 0 if they all pass.

## Valid inputs

Bits are numbered from 0 on the right.

- width has to be 1 to 32
- pos has to be 0 to 31
- pos + width can't be more than 32 (the field has to fit inside the word)

## What happens with bad input

**print_binary** prints just a blank line if width is 0 or negative,
since the loop never runs. If width is over 32, it gets cut down to 32,
because a uint32_t only has 32 bits to show.

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

## Edge cases

**Width 32 in get_field and set_field.** The mask is normally
(1u << width) - 1, which gives "width" ones in a row. But shifting a
32-bit number by 32 is undefined in C. On most PCs, 1u << 32 actually
turns into 1u << 0, so the mask would come out as 0. So when width is
32, I skip the formula and use 0xFFFFFFFF (all ones) instead.

**Value too big for the field in set_field.** If value has more bits
than width, the extra bits would spill into the neighboring bits when
you shift it into place. So after shifting, I AND it with the field
mask, which chops off anything outside the field. 

**How sign_extend works.** In two's complement, the leftmost bit counts
as negative. When C reads the bits as a normal unsigned number, that bit
counts as positive instead, so the number comes out too big by exactly
2^width. So if the sign bit is 1, I subtract 2^width. 

**Width 32 in sign_extend.** 2^32 doesn't fit in 32 bits, so I do the
subtraction with 64-bit numbers (int64_t) and then convert back to
int32_t. The answer always fits, even the most negative one
(sign_extend(0x80000000, 32) = -2147483648).

## Thermostat status word

status_unpack takes the thermostat's 16-bit status word and splits it
into a status_t struct, with one member per field:

| Bits  | Field    | Member in status_t | Type    |
|-------|----------|--------------------|---------|
| 0     | HEAT     | heat               | bool    |
| 1     | COOL     | cool               | bool    |
| 2     | FAN      | fan                | bool    |
| 3     | FAULT    | fault              | bool    |
| 6-4   | MODE     | mode               | uint8_t |
| 7     | reserved | reserved           | bool    |
| 15-8  | SETPOINT | setpoint           | int8_t  |

Every position and width is a named constant in status.h, so there are
no magic numbers in status.c. Each field is pulled out with get_field.
The set point also goes through sign_extend, because it's a signed
8-bit number and can be negative.

**Invalid modes (5, 6, 7).** status_unpack doesn't fix or hide a bad
mode. It just stores the raw number in mode. If the thermostat sends
mode 6, you get mode = 6, so you can see exactly what was sent, which
helps when debugging a broken thermostat. To check whether a mode is
valid, compare it to the named constant MODE_LAST_VALID:

    if (s.mode > MODE_LAST_VALID) { /* invalid */ }

I didn't replace bad modes with OFF, because then a broken thermostat
would look like it was just turned off. Deciding what to do about a bad
mode (like keeping the last good mode) is up to the program that uses
the decoder, since status_unpack only sees one word at a time and
doesn't know what the previous mode was.

The reserved bit is also stored as-is, so you can tell if it's ever
set when it shouldn't be.