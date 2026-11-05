lock: main.o lock.o
	gcc -std=c11 -Wall -Wextra main.o lock.o -o lock

main.o: main.c lock.h
	gcc -std=c11 -Wall -Wextra -c main.c -o main.o

lock.o: lock.c lock.h
	gcc -std=c11 -Wall -Wextra -c lock.c -o lock.o

tests/test_lock.o: tests/test_lock.c lock.h
	gcc -std=c11 -Wall -Wextra -c tests/test_lock.c -o tests/test_lock.o

tests/test_lock: tests/test_lock.o lock.o
	gcc -std=c11 -Wall -Wextra tests/test_lock.o lock.o -o tests/test_lock

test: tests/test_lock
	./tests/test_lock

clean:
	rm -f lock main.o lock.o tests/test_lock.o tests/test_lock

.PHONY: test clean
