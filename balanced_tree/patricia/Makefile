CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic --pedantic-errors -std=c23 -O3 -flto -s
LDFLAGS=-O3 -flto -s
SOURCES=patricia_tree.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -frd $(OBJECTS) $(EXECUTABLE)
