MAKE = g++ -Wall -Werror -ansi -pedantic main.cpp

all:
	$(MAKE)
rshell:
	$(MAKE)
clean:
	rm bin/rshell
	rm -rf bin
