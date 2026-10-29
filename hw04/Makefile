all: events

events: events.o tally.o
	gcc -std=c11 -Wall -Wextra events.o tally.o -o events

events.o: events.c tally.h
	gcc -std=c11 -Wall -Wextra -c events.c -o events.o

tally.o: tally.c tally.h
	gcc -std=c11 -Wall -Wextra -c tally.c -o tally.o

tests/test_tally.o: tests/test_tally.c tally.h
	gcc -std=c11 -Wall -Wextra -c tests/test_tally.c -o tests/test_tally.o

tests/test_tally: tests/test_tally.o tally.o
	gcc -std=c11 -Wall -Wextra tests/test_tally.o tally.o -o tests/test_tally

test: tests/test_tally
	./tests/test_tally

clean:
	rm -f events events.o tally.o tests/test_tally.o tests/test_tally

.PHONY: all test clean
