# HW5: Combination Lock (Table-Driven State Machine)

This folder has a combination lock written as a finite state machine.
All of the lock's behavior lives in one table (lock_table in lock.c),
and lock_step does a single table lookup, with no if or switch on the
state.

- lock.h / lock.c: the types, the table, lock_step, lock_event, and
  the name functions
- main.c: reads characters from standard input and prints one line
  per event
- data/codes.txt: sample input
- tests/test_lock.c: the test program

## Building and testing

From inside hw05:

    make                      # builds the lock program
    ./lock < data/codes.txt   # runs it on the sample input
    make test                 # builds tests/test_lock and runs it
    make clean                # deletes everything the build made

The test program prints PASS or FAIL for each test, then a summary.
It exits with 1 if any test fails, and 0 if they all pass.

## The lock

The code is 3-1-4. Input characters:

- a digit: E_RIGHT if it's the next digit of the code, otherwise E_WRONG
- `c`: the door closes (E_CLOSE)
- `t`: the 5-second timer runs out (E_TIMEOUT)
- anything else (spaces, newlines, other letters): not an event, skipped

## State diagram

```
          right(3)          right(1)          right(4) / UNLOCK
 LOCKED ----------> GOT1 ----------> GOT2 -----------------> OPEN
   ^                 |                 |                       |
   |   wrong / BEEP  |  wrong / BEEP   |                       |
   |   timeout       |  timeout        |                       |
   +-----------------+-----------------+                       |
   |                                                           |
   +-------------- close / LOCK,  timeout / LOCK --------------+

 Stays in the same state:
   LOCKED : wrong / BEEP, close, timeout
   GOT1   : close
   GOT2   : close
   OPEN   : any digit

 (no action written means NONE)
```

## Transition table

Each cell is `next state / action`.

| State      | E_RIGHT       | E_WRONG       | E_CLOSE       | E_TIMEOUT     |
|------------|---------------|---------------|---------------|---------------|
| **LOCKED** | GOT1 / NONE   | LOCKED / BEEP | LOCKED / NONE | LOCKED / NONE |
| **GOT1**   | GOT2 / NONE   | LOCKED / BEEP | GOT1 / NONE   | LOCKED / NONE |
| **GOT2**   | OPEN / UNLOCK | LOCKED / BEEP | GOT2 / NONE   | LOCKED / NONE |
| **OPEN**   | OPEN / NONE   | OPEN / NONE   | LOCKED / LOCK | LOCKED / LOCK |

The patterns:

- **E_RIGHT moves forward one step.** Only the last step opens the
  door, so [GOT2][E_RIGHT] is the only UNLOCK.
- **E_WRONG goes back to LOCKED with a BEEP**, even from GOT2, because
  a wrong digit starts over from the beginning. In OPEN, digits do
  nothing.
- **E_TIMEOUT while typing goes back to LOCKED with NONE**, because the
  reset is silent. In LOCKED the timer isn't running, but the table
  still needs the cell, so it does nothing.
- **E_CLOSE does nothing unless the door is open.** In OPEN, closing
  or the timer running out locks it again.

## Design notes

**Why A_UNDEFINED is 0.** C fills any table cell I don't write with 0.
Since A_UNDEFINED is 0, a forgotten cell shows up as "undefined"
instead of looking like a real do-nothing cell. If A_NONE were 0, a
missing cell would be impossible to tell apart from a correct one.

**How lock_event knows the next digit.** The code is stored as the
string "314". The state number is also the index of the digit it's
waiting for: LOCKED (0) wants code[0] = '3', GOT1 (1) wants
code[1] = '1', and GOT2 (2) wants code[2] = '4'. In OPEN there is no
next digit, so every digit is E_WRONG, and the table turns that into
"do nothing."

**Why lock_event returns int.** It needs to return -1 for characters
that aren't events, and -1 isn't one of the event_t values.

**The name functions** check the range before indexing the array and
return "?" if the value is out of range, so they never read outside
the array.

**Why 3 3 1 4 does not open.** The first 3 moves to GOT1. The second 3
is E_WRONG in GOT1 (it wants a 1), so the lock goes back to LOCKED and
throws away the first 3 too. Then 1 and 4 are both wrong in LOCKED.
Changing [GOT1][E_WRONG] to GOT1 / BEEP would make 3 3 1 4 open, but it
would also make 3 7 1 4 open, because the table only sees "wrong" and
doesn't know which digit it was.

**Newlines don't reset the lock.** main.c skips characters that aren't
events, so the state carries over from one line of input to the next.
