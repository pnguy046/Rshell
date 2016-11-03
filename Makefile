MAKE = g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
BIN = mkdir -p bin

all:
	$(BIN)
	$(MAKE)
rshell:
	$(BIN)
	$(MAKE)
clean:
	rm bin/rshell
	rm -rf bin
