Script started on Fri 18 Nov 2016 11:33:35 PM PST
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ make
g++ -c -Wall -ansi command.cpp -o command.o
command.cpp: In member function ‘bool Execute::execute(std::vector<std::basic_string<char> >)’:
command.cpp:60:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
g++ command.o main.o -o test.out
mkdir -p bin src/rshell.cpp bin/rshell
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ tes[K[K[K.a[K/a.out
$ exit
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ .a[K/a.out
$ Echo    (echo a && exit) || echo c
a
c
$ (echo a &&  || exit) || ec xit
a
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ a[K./a.out
$ (echo a && echo b) || echo c
a
c
$ make
$ exit
$ exit
]0;ubuntu@patrickn96-ourrepo-3961800: ~/workspace/CS100/Assn2/Working[01;32mpatrickn96[00m:[01;34m~/workspace/CS100/Assn2/Working[00m (master) $ exit
exit

Script done on Fri 18 Nov 2016 11:36:10 PM PST
