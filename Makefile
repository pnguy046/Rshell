MAKE = g++ -Wall -Werror -ansi -pedantic main.cpp
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