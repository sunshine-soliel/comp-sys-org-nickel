CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra

# make memcheck adds these to CFLAGS
SANITIZE = -g -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer

all: balance

balance: balance.o istack.o
	$(CC) $(CFLAGS) balance.o istack.o -o balance

balance.o: balance.c istack.h
	$(CC) $(CFLAGS) -c balance.c -o balance.o

istack.o: istack.c istack.h
	$(CC) $(CFLAGS) -c istack.c -o istack.o

tests/test_istack.o: tests/test_istack.c istack.h
	$(CC) $(CFLAGS) -c tests/test_istack.c -o tests/test_istack.o

tests/test_istack: tests/test_istack.o istack.o
	$(CC) $(CFLAGS) tests/test_istack.o istack.o -o tests/test_istack

# Both tests: the unit tests, then the regression test
test: balance tests/test_istack
	./tests/test_istack
	sh regress.sh

# Rebuild everything with the sanitizers and run both tests.
# The clean first matters: without it, make would reuse the .o files
# built without -fsanitize. The clean after keeps those sanitizer .o
# files from mixing into a later plain make.
memcheck:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(SANITIZE)"
	$(MAKE) clean

clean:
	rm -f balance *.o tests/test_istack.o tests/test_istack

.PHONY: all test memcheck clean
