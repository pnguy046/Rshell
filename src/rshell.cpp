#include <iostream>
#include <cstring>
#include <vector>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <errno.h> //Needed for exit status
#include <stdio.h>
#include <sys/wait.h> //Needed for waitpid
#include <stdlib.h>  // Declaration for exit()


using namespace std;
/*
	In this, we have our rshell working
	and our function declarations here
*/
void print();
void exec(vector<string>&, bool&);
void execute(vector<string>, bool&);
void parseString(string, vector<string>&);
bool isComment(string);
bool hasSemi(string s);
bool hasHash(string s);
bool isComparator(string s);



int main(){
    string user_input = "";
    bool execStatus = true;
    bool allSpace = true;
    vector<string> argsList;
    
    while (user_input != "exit") {
        print();
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
            argsList.clear();
            for(unsigned i = 0; i < user_input.size(); i++) {
                if(user_input.at(i) != ' ') {
                    allSpace = false;
                    break;
                }
            }
            //the actual shell
            if(!allSpace) { 
                parseString(user_input, argsList);
                exec(argsList, execStatus);
            }
            else{
                break;
            }
        }
    }
    return 0;
}
//getting the command prompt to show       
void print() {
    char* user = getlogin();
    char host[256]; 
	gethostname(host, 128);
	if (user == NULL) { 
		cout << "$ ";
	}
	else {
		cout << user << '@' << host << "$ ";
	}
}

//here is where the forking and executables while be handled
//passed in command
void execute(vector<string> cmds, bool &execStatus) {

    if(cmds.size() > 1){
    	//fixes quotation marks for echo
		if (cmds.at(1).at(0) == '\"'||cmds.at(1).at(0) ==  '\'') { 
			cmds.at(1).erase(cmds.at(1).begin());
			cmds.at(cmds.size() - 1).erase(cmds.at(cmds.size() - 1).begin() + cmds.at(cmds.size() - 1).size() - 1);
		}
	}
	
    int i;
    char* argv[cmds.size() + 1];
    for (i = 0; i < static_cast<int>(cmds.size()); i++) { //convert the vector into a char* array for execvp
		argv[i] = (char*)cmds.at(i).c_str();	
	}
	argv[cmds.size()] = NULL;
	// starts at bin
	char path[20] = "/bin/"; 	
	// add the first command to the file path
	strcat(path, cmds[0].c_str()); 
    pid_t pid = fork();
    //fork succeeded
    if(pid >= 0){ 
    	//child process
        if(pid == 0) { 
        	/* execute the command  */
            if (execvp(path, argv) < 0) {     
              execStatus = false;
              //error execvp failed
              perror("execvp failed.\n");
              exit(-1);
          }
          else {
              execStatus = true;
          }
        }
        //parent process
        else { 
            int status;
            waitpid(pid, &status, 0);
        }
    }
    //fork failed
    else { 
        execStatus = false;
        perror("fork failed\n");
        exit(-1);
    }
}

void exec(vector<string> &argsList, bool &execStatus) {
    vector<string> command;
    
	//this will start when there are 2 commands
    for (unsigned i = 0; i < argsList.size(); i++) { 
		command.push_back(argsList.at(i));
		//if there's more cmds and the next command is a comparator

		if(isComparator(argsList.at(i)) && command.size() > 1) { 
			//handles comments after comparator
			if(command.at(1) == "#") { 
					break;
			}
			
			//include next comparator
			if (command.at(0) == "&&") {
				i--; 
				//remove comparator from command
				command.erase(command.begin()); 
				//remove comparator at the end
				command.pop_back(); 
				if (execStatus == true) {
					execute(command, execStatus);
				}
				command.clear();
			}

			else if(command.at(0)== ";") {
				i--;
				//remove comparator from command
				command.erase(command.begin()); 
				command.pop_back(); 
				execute(command, execStatus);
				command.clear();
			}
			else if(command.at(0) == "||") {
				 //include next comparator
				i--;
				//delete the comparator
				command.erase(command.begin()); 
				//remove comparator at end of command
				command.pop_back(); 
				//if the previous command did not execute, run this command
				if (execStatus == false) { 
					execute(command, execStatus);
				}
				command.clear();
			}
			else if(command.at(0) == "#" && command.size() == 1) {

				break; 
			}
			else if(!isComparator(command.at(0))){
				i--;
				//remove the comparator
				command.pop_back(); 
				execute(command, execStatus);
				command.clear();
			}
		}
		//last command
		else if(i == argsList.size()-1) { 
			if(isComparator(command.at(command.size()-1))) { 
				command.pop_back();
				break;
			}
			//delete comparator
			if (command.size() != 0 && command.at(0) == "&&") {
				command.erase(command.begin()); 
				if (execStatus == true) {
					execute(command, execStatus);
				}
			}
			else if (command.size() != 0 && command.at(0) == "||") {
				command.erase(command.begin());
				if (execStatus == false) {
					execute(command, execStatus);
				}
			}
			else if (command.size() != 0 && command.at(0) == ";") {
				command.erase(command.begin());
				execute(command, execStatus);
				break;
			}
			else if (command.size() != 0 && command.at(0) == "#") {
				command.erase(command.begin());
				return;
			}
			else {
				execute(command, execStatus);
			}
		}
	}
}

bool isComparator(string s) {
	if (s == "||" || s== "&&" || s == ";" || s == "#") {
		return true;
	}
	return false;	
}
//checks for semicolon attached to a word
bool hasSemiColon(string s){ 
	for(unsigned i = 0; i < s.size(); ++i){
	    //return position of semi
		if(s.at(i) == ';'){
				return true; 
		}
	}
	return false; // if not found return -1
}

//checks for hashtag
bool isComment(string s){
	for(unsigned i = 0; i < s.size(); ++i){
		if(s.at(i) == '#') {
				return true;
		}
	}
	return false;
}

void parseString(string userInput, vector<string>& argsList) {
	
	//parsed using the strtok command method bellow is what was seen on the c++
	//reference site
	//split command into separate strings
	char* token; 
	char* messagePointer = new char[userInput.length() + 1];
	//converts string to char*
	strcpy(messagePointer, userInput.c_str());
	//tokenize first part of string
	token = strtok(messagePointer, " "); 

	/*
		This is where the parsing and check takes place for the rest
		of this code. Using a for loop, it will go and place every word parsed
		with a space and then test the special cases for the semicolon and 
		the comments
	
	*/

	for (int i = 0; token != NULL; i++) {
	    //break out of for loop if there's an empty token

		if (token == NULL) { 
			break;
		}
		string parsedString = string(token);
		//if it a just a comparator without any words attached  go straight to push_back
		if(!isComparator(parsedString)) { 
		    //if there is a semicolon attached enter loop
			if (hasSemiColon(parsedString)) {
				string semicolon = ";";
				//remove the semicolon at the end of word
				parsedString.erase(parsedString.begin() + parsedString.size() -1 ); 
				 // push back word without semicolon
				argsList.push_back(parsedString);
				// push back semi colon as its own cell
				argsList.push_back(semicolon);
			}
			//if there is a comment enter loop
			else if(isComment(parsedString)) { 
				string hashtag = "#";
				// remove hashtag located at front of string
				parsedString.erase(parsedString.begin()); 
				// push back has as its own cell
				argsList.push_back(hashtag); 
				// push back word 
				argsList.push_back(parsedString); 
			}
			else{ 
			    // if it does not contain semi or hash push back word
				argsList.push_back(parsedString); 
			}
		}
		else{	
			argsList.push_back(parsedString);
			}
		token = strtok(NULL, " ");
	}

	while(isComparator(argsList.at(0))) {
		argsList.erase(argsList.begin());
	}
	while(isComparator(argsList.at(argsList.size()-1))) {
		argsList.pop_back();
	}

}  
