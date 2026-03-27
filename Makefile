CC := clang
CSTD := -std=c23
BUILDFLAGS := -O2 -Wall -Wextra -Iinclude
LIBFLAGS := -std=gnu11 -Og -g -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wimplicit-fallthrough -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -fstack-protector-strong -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=undefined,address -D_FORTIFY_SOURCE=2 -pipe
TESTFLAGS := -std=gnu11 -Og -g -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wimplicit-fallthrough -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -fstack-protector-strong -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=undefined,address -D_FORTIFY_SOURCE=2 -pipe
FUZZCFLAGS := -fsanitize=fuzzer -fno-omit-frame-pointer -fno-sanitize-recover=all -g -O1 -Iinclude

.PHONY: all clean

all: bin libclib.o
dev: bin libclib_dev.o

libclib.o: clib.c clib.h | bin
	${CC} $(CSTD) $(BUILDFLAGS) -c clib.c -o bin/libclib.o

bin:
	mkdir -p $@

libclib_dev.o: clib.c clib.h | bin
	$(CC) $(CSTD) $(LIBFLAGS) -c clib.c -o bin/libclib_dev.o

test: test_clib.c clib.h | bin
	$(CC) $(CSTD) $(TESTFLAGS) test_clib.c bin/libclib_dev.o -o bin/test
	./bin/test

fuzz: fuzz.c clib.h | bin
	$(CC) $(CSTD) $(FUZZCFLAGS) clib.c fuzz.c -o bin/fuzz

clean:
	rm -rf bin
