all: bits.o status.o tests/test_bits.o

bits.o: bits.c bits.h
	gcc -std=c11 -Wall -Wextra -c bits.c -o bits.o

status.o: status.c status.h bits.h
	gcc -std=c11 -Wall -Wextra -c status.c -o status.o

tests/test_bits.o: tests/test_bits.c bits.h status.h
	gcc -std=c11 -Wall -Wextra -c tests/test_bits.c -o tests/test_bits.o

tests/test_bits: tests/test_bits.o bits.o status.o
	gcc -std=c11 -Wall -Wextra tests/test_bits.o bits.o status.o -o tests/test_bits

test: tests/test_bits
	./tests/test_bits

clean:
	rm -f bits.o status.o tests/test_bits.o tests/test_bits

.PHONY: all test clean