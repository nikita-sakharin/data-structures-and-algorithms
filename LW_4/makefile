CC=g++
CFLAGS=-c -std=c++14 -Werror -pedantic -Wall -Wextra -O3
LDFLAGS=
SOURCES=LW_4.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=LW_4

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -frd $(OBJECTS) $(EXECUTABLE)