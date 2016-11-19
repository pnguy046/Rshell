#include <iostream>
#include <stdio.h>
#include <unistd.h> //used for fork(), execvp()
#include <sys/types.h> //used for wait
#include <sys/wait.h> //used for wait
#include <sys/stat.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include "command.h"
using namespace std;

int main(){
    string user_input = "";
    // bool execStatus = true;
    bool allSpace = true;
    // vector<string> argsList;
    
    
    while (user_input != "exit") {
        Parse p;
        p.print();
        getline(cin, user_input);
        //test for exit
        if(user_input == "exit") {
            exit(0);
        }
        //test for empty string
        else if(user_input.size() == 0) {
            break;
        }
        //test for only white spaces
        else {
            // argsList.clear();
            for(unsigned i = 0; i < user_input.size(); i++) {
                if(user_input.at(i) != ' ') {
                    allSpace = false;
                    break;
                }
            }
            //the actual shell
            if(!allSpace) { 
                p.sepParenthesis(user_input);
                // e.exec(argsList, execStatus);
            }
            else{
                break;
            }
        }
    }
    return 0;
}