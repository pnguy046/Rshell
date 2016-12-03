Script started on Fri 18 Nov 2016 11:12:26 PM PST
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ g[Kmake[K[K[K[Kmake
g++ -c -Wall -ansi command.cpp -o command.o
command.cpp: In member function ‘bool Execute::execute(std::vector<std::basic_string<char> >)’:
command.cpp:60:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
g++ -c -Wall -ansi main.cpp -o main.o
g++ command.o main.o -o test.out
mkdir -p bin src/rshell.cpp bin/rshell
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ make
g++ -c -Wall -ansi command.cpp -o command.o
g++ command.o main.o -o test.out
mkdir -p bin src/rshell.cpp bin/rshell
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ make clean
rm -f test.out command.o main.o
rm -rf bin
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ make
g++ -c -Wall -ansi command.cpp -o command.o
g++ -c -Wall -ansi main.cpp -o main.o
g++ command.o main.o -o test.out
mkdir -p bin src/rshell.cpp bin/rshell
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ .a/[K[K/a.out
$ echo hello #[testing       -  -e o working       uo  oiu  r urrepo ] 
hello
$ echo ls    e test #[ - -e   d edf   d ourrepo ] 
test
$ echo anohter       "ano other test" $ #[ -f our  repo ]
another test
$ echo      )echo     (echo a && echo b) ##  #  || echo       %%  && echo    (echo c && echo b f)
a
$ (echo a && echo b) #echo c
a
$ echo      (echo a || echo b)
a
b
$ exit
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ exit
exit

Script done on Fri 18 Nov 2016 11:19:04 PM PST
