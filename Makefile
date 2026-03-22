CC := clang
CFLAGS := -O2 -Wall -Wextra -Iinclude
TESTFLAGS := -std=gnu11 -Og -g -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wimplicit-fallthrough -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -fstack-protector-strong -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=undefined,address -D_FORTIFY_SOURCE=2 -pipe
FUZZCFLAGS := -fsanitize=fuzzer -fno-omit-frame-pointer -fno-sanitize-recover=all -g -O1 -Iinclude

.PHONY: all clean

all: bin libclib.o

bin:
	mkdir -p $@

libclib.o: clib.c clib.h | bin
	$(CC) $(CFLAGS) -c clib.c -o bin/libclib.o

test: test_clib.c clib.h | bin
	$(CC) $(TESTFLAGS) test_clib.c bin/libclib.o -o bin/test
	./bin/test

fuzz: fuzz.c clib.h | bin
	$(CC) $(FUZZCFLAGS) clib.c fuzz.c -o bin/fuzz

clean:
	rm -rf bin
