CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: replay

replay: main.o sensor.o panel.o scenario.o iom361_r4.o
	$(CC) $(CFLAGS) main.o sensor.o panel.o scenario.o iom361_r4.o -o replay -lm

main.o: main.c iom361_r4.h sensor.h panel.h scenario.h
	$(CC) $(CFLAGS) -c main.c -o main.o

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c -o sensor.o

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c -o panel.o

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c -o scenario.o

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c -o iom361_r4.o

tests/test_hw06.o: tests/test_hw06.c iom361_r4.h sensor.h panel.h scenario.h
	$(CC) $(CFLAGS) -c tests/test_hw06.c -o tests/test_hw06.o

tests/test_hw06: tests/test_hw06.o sensor.o panel.o scenario.o iom361_r4.o
	$(CC) $(CFLAGS) tests/test_hw06.o sensor.o panel.o scenario.o iom361_r4.o -o tests/test_hw06 -lm

test: replay tests/test_hw06
	./tests/test_hw06
	sh regress.sh

clean:
	rm -f replay *.o tests/test_hw06.o tests/test_hw06

.PHONY: all test clean
