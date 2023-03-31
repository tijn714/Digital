CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

ifdef DEBUG
	CFLAGS += -g
endif

LDFLAGS = -framework CoreFoundation -framework CoreGraphics

all: main

main: src/main.c
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f main main.o

linux: src/main.c
	$(CC) $(LDFLAGS) $^ -o $@
