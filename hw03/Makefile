all: ptr.o tests/test_ptr

ptr.o: ptr.c ptr.h
	gcc -std=c11 -Wall -Wextra -c ptr.c -o ptr.o

tests/test_ptr.o: tests/test_ptr.c ptr.h
	gcc -std=c11 -Wall -Wextra -c tests/test_ptr.c -o tests/test_ptr.o

tests/test_ptr: tests/test_ptr.o ptr.o
	gcc -std=c11 -Wall -Wextra tests/test_ptr.o ptr.o -o tests/test_ptr

test: tests/test_ptr
	./tests/test_ptr

clean:
	rm -f ptr.o tests/test_ptr.o tests/test_ptr

.PHONY: all test clean
