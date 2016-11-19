CC=g++
CC_FLAGS=-Wall -ansi
EXEC=test.out
SOURCES=$(wildcard *.cpp)
BIN = mkdir -p bin src/rshell.cpp bin/rshell
OBJECTS=$(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)
	$(BIN)

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
	rm -rf bin
