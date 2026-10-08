# AI Usage

## Tool
Claude (claude.ai)

## How I used it
- At first Claude generated a complete solution. I deleted all of it
  and started over, because I wanted to understand how it works, not
  just turn in working code.
- After that, Claude walked me through each function step by step. I
  worked out examples by hand (get_field, set_field, sign_extend, and
  decoding 0x1631 and 0xF82E) before writing any code, and I typed the
  functions in myself.
- Claude wrote the test file (tests/test_bits.c) and drafted the
  README. I proofread and edited both.
- Claude explained the Makefile one rule at a time, and I built it up
  and tested make, make test, and make clean.


## Something the AI got wrong or I had to fix
- Claude first suggested adding a mode_valid member to the struct.
  After I read the assignment again, that would have broken the "one
  member per field" rule, so I came up with my own solution that made
  sense (MODE_LAST_VALID).