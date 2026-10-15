#ifndef READING_H
#define READING_H

#define READING_MAX 1000   /* most readings the program keeps */

/* Result of reading_read: how many readings were stored, and how many lines were skipped. */
typedef struct {
    int count;     /* readings stored in the arrays */
    int skipped;   /* lines that did not hold three numbers */
} reading_result_t;

/* Reads lines of "tick temperature humidity" from standard input into
   ticks[], temps[] and hums[], storing at most max readings.
   Blank lines and lines starting with '#' are ignored; other bad lines are skipped.
   Returns the number stored and the number skipped.
   Assumes each array has room for at least max values. */
reading_result_t reading_read(int ticks[], float temps[], float hums[], int max);

#endif