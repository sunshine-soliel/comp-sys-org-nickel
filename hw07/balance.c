#include <stdio.h>
#include "istack.h"

// ./balance < file
// Reads standard input one character at a time. Pushes each ( [ { and pops
// for each ) ] }. Stops at the first error and prints exactly one line.
// Exit status 0 if balanced, 1 on a bracket error, 2 if memory runs out.

// The closing bracket that matches an opening one
static char closer_for(int open)
{
    if (open == '(') {
        return ')';
    }
    if (open == '[') {
        return ']';
    }
    return '}';
}

int main(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        fprintf(stderr, "balance: out of memory\n");
        return 2;
    }

    int line = 1;          // lines and columns count from 1
    int col = 0;           // column of the character just read
    int pairs = 0;         // matched pairs so far
    size_t deepest = 0;    // most brackets open at once
    int status = 0;        // what main returns
    int c;

    // One exit path: every error breaks out of the loop, so the stack is
    // destroyed below no matter how we leave.
    while ((c = getchar()) != EOF) {
        col = col + 1;

        if (c == '(' || c == '[' || c == '{') {
            if (istack_push(s, c) != 0) {
                fprintf(stderr, "balance: out of memory\n");
                status = 2;
                break;
            }
            if (istack_count(s) > deepest) {
                deepest = istack_count(s);
            }
        } else if (c == ')' || c == ']' || c == '}') {
            int open;
            if (istack_pop(s, &open) != 0) {
                printf("line %d, column %d: found '%c', nothing open\n", line, col, c);
                status = 1;
                break;
            }
            if (closer_for(open) != c) {
                printf("line %d, column %d: found '%c', expected '%c'\n",
                       line, col, c, closer_for(open));
                status = 1;
                break;
            }
            pairs = pairs + 1;
        }

        // The newline itself is the last column of its line
        if (c == '\n') {
            line = line + 1;
            col = 0;
        }
    }

    // Reached the end of the input with no error so far
    if (status == 0) {
        if (istack_is_empty(s)) {
            printf("balanced: %d pairs, deepest %zu\n", pairs, deepest);
        } else {
            int innermost = 0;
            istack_peek(s, &innermost);
            printf("end of input: %zu still open, innermost '%c'\n",
                   istack_count(s), innermost);
            status = 1;
        }
    }

    istack_destroy(s);
    return status;
}
