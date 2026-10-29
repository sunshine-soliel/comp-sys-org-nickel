#include <stdio.h>
#include <string.h>
#include "tally.h"

#define LINE_SIZE 256

int main(void)
{
    char line[LINE_SIZE];
    int skipped = 0;

    tally_t *t = tally_create();
    if (t == NULL) {
        fprintf(stderr, "could not create a tally\n");
        return 1;
    }

    while (fgets(line, sizeof line, stdin) != NULL) {
        // If there's no '\n', the line was longer than the buffer.
        // Throw away the rest of it so it doesn't come back as a new line.
        // (It's way longer than TALLY_LEN, so tally_add will refuse it anyway.)
        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
        }

        // Remove the newline (and a '\r' from Windows files)
        line[strcspn(line, "\r\n")] = '\0';

        // Skip blank lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        // Count lines the tally refused
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    tally_print(t, stdout);
    printf("names: %d, lines skipped: %d\n", tally_size(t), skipped);

    tally_destroy(t);
    return 0;
}
