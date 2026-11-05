#include <stdio.h>
#include "lock.h"

int main(void)
{
    state_t s = LOCKED;
    int ch;

    // Read one character at a time until the input runs out
    while ((ch = getchar()) != EOF) {
        // Turn the character into an event; skip it if it isn't one
        int e = lock_event(s, (char)ch);
        if (e < 0) {
            continue;
        }

        // One step of the machine
        action_t a;
        state_t next = lock_step(s, (event_t)e, &a);

        // e.g. "GOT2 4 RIGHT -> OPEN UNLOCK"
        printf("%s %c %s -> %s %s\n",
               state_name(s), ch, event_name((event_t)e),
               state_name(next), action_name(a));

        s = next;
    }

    printf("final: %s\n", state_name(s));
    return 0;
}