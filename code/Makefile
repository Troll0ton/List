CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp list.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rlist

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@