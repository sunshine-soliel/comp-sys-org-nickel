#include <stdio.h>
#include <stdbool.h>
#include "../lock.h"

// Running totals, shared by every test
int pass_count = 0;
int fail_count = 0;

// Prints PASS or FAIL for one test and adds to the totals.
void check(bool result, const char *name)
{
    if (result == true) {
        printf("PASS  %s\n", name);
        pass_count = pass_count + 1;
    } else {
        printf("FAIL  %s\n", name);
        fail_count = fail_count + 1;
    }
}

// Checks one table entry through lock_step: right next state AND right action
void check_step(state_t s, event_t e, state_t want_next, action_t want_action)
{
    char name[64];
    snprintf(name, sizeof name, "table %s + %s -> %s %s",
             state_name(s), event_name(e), state_name(want_next), action_name(want_action));

    action_t a = A_UNDEFINED;
    state_t next = lock_step(s, e, &a);
    check(next == want_next && a == want_action, name);
}

// Feeds a string through the lock, starting in LOCKED, like main.c does.
// Returns the final state.
state_t run(const char *input)
{
    state_t s = LOCKED;
    for (int i = 0; input[i] != '\0'; i++) {
        int e = lock_event(s, input[i]);
        if (e < 0) {
            continue;
        }
        action_t a;
        s = lock_step(s, (event_t)e, &a);
    }
    return s;
}

int main(void)
{
    // ----- all 16 table entries -----
    check_step(LOCKED, E_RIGHT,   GOT1,   A_NONE);
    check_step(LOCKED, E_WRONG,   LOCKED, A_BEEP);
    check_step(LOCKED, E_CLOSE,   LOCKED, A_NONE);
    check_step(LOCKED, E_TIMEOUT, LOCKED, A_NONE);

    check_step(GOT1,   E_RIGHT,   GOT2,   A_NONE);
    check_step(GOT1,   E_WRONG,   LOCKED, A_BEEP);
    check_step(GOT1,   E_CLOSE,   GOT1,   A_NONE);
    check_step(GOT1,   E_TIMEOUT, LOCKED, A_NONE);

    check_step(GOT2,   E_RIGHT,   OPEN,   A_UNLOCK);
    check_step(GOT2,   E_WRONG,   LOCKED, A_BEEP);
    check_step(GOT2,   E_CLOSE,   GOT2,   A_NONE);
    check_step(GOT2,   E_TIMEOUT, LOCKED, A_NONE);

    check_step(OPEN,   E_RIGHT,   OPEN,   A_NONE);
    check_step(OPEN,   E_WRONG,   OPEN,   A_NONE);
    check_step(OPEN,   E_CLOSE,   LOCKED, A_LOCK);
    check_step(OPEN,   E_TIMEOUT, LOCKED, A_LOCK);

    // ----- lock_event: right and wrong digit in each state -----
    check(lock_event(LOCKED, '3') == E_RIGHT, "event LOCKED '3' is RIGHT");
    check(lock_event(LOCKED, '1') == E_WRONG, "event LOCKED '1' is WRONG");
    check(lock_event(GOT1,   '1') == E_RIGHT, "event GOT1 '1' is RIGHT");
    check(lock_event(GOT1,   '3') == E_WRONG, "event GOT1 '3' is WRONG");
    check(lock_event(GOT2,   '4') == E_RIGHT, "event GOT2 '4' is RIGHT");
    check(lock_event(GOT2,   '9') == E_WRONG, "event GOT2 '9' is WRONG");

    // ----- lock_event: while open, every digit is WRONG (and does nothing) -----
    check(lock_event(OPEN, '3') == E_WRONG,   "event OPEN '3' is WRONG");
    check(lock_event(OPEN, '4') == E_WRONG,   "event OPEN '4' is WRONG");

    // ----- lock_event: c, t, and other characters -----
    check(lock_event(LOCKED, 'c') == E_CLOSE,   "event 'c' is CLOSE");
    check(lock_event(OPEN,   'c') == E_CLOSE,   "event 'c' is CLOSE while open");
    check(lock_event(GOT1,   't') == E_TIMEOUT, "event 't' is TIMEOUT");
    check(lock_event(OPEN,   't') == E_TIMEOUT, "event 't' is TIMEOUT while open");
    check(lock_event(LOCKED, 'x') == -1,        "event 'x' is -1");
    check(lock_event(LOCKED, ' ') == -1,        "event space is -1");
    check(lock_event(LOCKED, '\n') == -1,       "event newline is -1");

    // ----- whole sequences -----
    check(run("314") == OPEN,      "sequence 3 1 4 opens");
    check(run("3314") == LOCKED,   "sequence 3 3 1 4 does not open");
    check(run("31t4") == LOCKED,   "sequence 3 1 t 4 does not open");
    check(run("314c") == LOCKED,   "sequence 3 1 4 c locks again");

    // ----- names, including out of range -----
    check(state_name(GOT2)[0] == 'G',                  "state_name GOT2");
    check(state_name((state_t)N_STATES)[0] == '?',     "state_name out of range");
    check(event_name((event_t)N_EVENTS)[0] == '?',     "event_name out of range");
    check(action_name((action_t)N_ACTIONS)[0] == '?',  "action_name out of range");

    // ----- Summary -----
    printf("\n%d passed, %d failed\n", pass_count, fail_count);

    // Exit code: 0 means everything passed, 1 means something failed
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}
