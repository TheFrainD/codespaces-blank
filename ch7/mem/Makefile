CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -Werror

SRC=$(wildcard *.c)
HDRS=$(wildcard *.h)
OBJ=$(SRC:.c=.o)

BIN=mem

.PHONY: all run clean

all: $(BIN)

run: all
	./$(BIN)

$(BIN): $(OBJ) $(HDRS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(BIN) $(OBJ)
