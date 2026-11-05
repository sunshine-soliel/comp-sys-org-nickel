#ifndef LOCK_H
#define LOCK_H

// States: how much of the code 3-1-4 has been typed so far
typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t;

// Events: the four things that can happen in any state
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t;

// Actions: what the lock does on a transition.
// A_UNDEFINED is 0, so a table entry I forget to fill in shows up as undefined.
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t;

// One cell of the table: where to go, and what to do
typedef struct { state_t next; action_t action; } transition_t;

// The transition table itself, defined in lock.c
extern const transition_t lock_table[N_STATES][N_EVENTS];

/* Looks up event e in state s in the table.
   Stores the action in *action and returns the next state. */
state_t lock_step(state_t s, event_t e, action_t *action);

/* Turns a character into an event for state s.
   The next digit of the code is E_RIGHT, any other digit is E_WRONG,
   'c' is E_CLOSE, 't' is E_TIMEOUT, and anything else is -1. */
int lock_event(state_t s, char c);

/* Return the name of a state, event, or action as a string,
   or "?" if the value is out of range. */
const char *state_name(state_t s);
const char *event_name(event_t e);
const char *action_name(action_t a);

#endif