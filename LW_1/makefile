CC=gcc
CFLAGS=-c -std=c11 -Werror -pedantic -Wall -Wextra -o3
SOURCES=LW_1.c radix_sort.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=LW_1

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -frd *.o $(EXECUTABLE)