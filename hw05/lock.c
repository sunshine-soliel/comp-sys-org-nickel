#include "lock.h"

// The whole lock: one row per state, one cell per event.
// Each cell is { next state, action }.
const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE   },
        [E_WRONG]   = { LOCKED, A_BEEP   },
        [E_CLOSE]   = { LOCKED, A_NONE   },
        [E_TIMEOUT] = { LOCKED, A_NONE   },
    },
    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE   },
        [E_WRONG]   = { LOCKED, A_BEEP   },
        [E_CLOSE]   = { GOT1,   A_NONE   },
        [E_TIMEOUT] = { LOCKED, A_NONE   },
    },
    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },
        [E_WRONG]   = { LOCKED, A_BEEP   },
        [E_CLOSE]   = { GOT2,   A_NONE   },
        [E_TIMEOUT] = { LOCKED, A_NONE   },
    },
    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE   },
        [E_WRONG]   = { OPEN,   A_NONE   },
        [E_CLOSE]   = { LOCKED, A_LOCK   },
        [E_TIMEOUT] = { LOCKED, A_LOCK   },
    },
};

state_t lock_step(state_t s, event_t e, action_t *action)
{
    // One lookup: row s, column e
    transition_t t = lock_table[s][e];
    *action = t.action;
    return t.next;
}

// The code, as characters. code[s] is the digit expected in state s.
static const char code[] = "314";

int lock_event(state_t s, char c)
{
    if (c == 'c') {
        return E_CLOSE;
    }
    if (c == 't') {
        return E_TIMEOUT;
    }
    if (c >= '0' && c <= '9') {
        // Right only if we're still typing and it's the next digit
        if (s < OPEN && c == code[s]) {
            return E_RIGHT;
        }
        return E_WRONG;
    }
    // Not an event (spaces, newlines, letters...)
    return -1;
}

// Names indexed by the enum, so state_names[GOT1] is "GOT1"
static const char *const state_names[N_STATES] = {
    [LOCKED] = "LOCKED", [GOT1] = "GOT1", [GOT2] = "GOT2", [OPEN] = "OPEN",
};
static const char *const event_names[N_EVENTS] = {
    [E_RIGHT] = "RIGHT", [E_WRONG] = "WRONG",
    [E_CLOSE] = "CLOSE", [E_TIMEOUT] = "TIMEOUT",
};
static const char *const action_names[N_ACTIONS] = {
    [A_UNDEFINED] = "UNDEFINED", [A_NONE] = "NONE", [A_UNLOCK] = "UNLOCK",
    [A_LOCK] = "LOCK", [A_BEEP] = "BEEP",
};

const char *state_name(state_t s)
{
    if ((int)s < 0 || s >= N_STATES) {
        return "?";
    }
    return state_names[s];
}

const char *event_name(event_t e)
{
    if ((int)e < 0 || e >= N_EVENTS) {
        return "?";
    }
    return event_names[e];
}

const char *action_name(action_t a)
{
    if ((int)a < 0 || a >= N_ACTIONS) {
        return "?";
    }
    return action_names[a];
}