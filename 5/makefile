CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS :=

TARGETS := generator handler

all: $(TARGETS)

generator: generator.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

handler: handler.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run-c-handler: handler
	./handler

run-c-generator: generator
	./generator

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean run-c-handler run-c-generator
