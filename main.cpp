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
    vector<string> argsList;
    
    while (user_input != "exit") {
        getline(cin, user_input);
        if(user_input == "exit") {
            exit(0);
        }
        else {
            argsList.clear();
            parseString(user_input, argsList);
            exec(argsList, execStatus);
        }
    }
    return 0;
}
       

void execute(vector<string> commands, bool &execStatus) { //passed in command
	cout << "enter executive: " << endl; //test
	for(unsigned i = 0; i < commands.size(); i++) {
		cout << "<" << commands.at(i) << "> ";
	}
	cout << endl;
	
    if(commands.size() > 1){
    	cout << "enter edit: " << endl;
		if (commands.at(1).at(0) == '\"'||commands.at(1).at(0) ==  '\'') { //fixes quotation marks for echo
			cout << "echo change" << endl;
			commands.at(1).erase(commands.at(1).begin());
			commands.at(commands.size()-1).erase(commands.at(commands.size()-1).begin() + commands.at(commands.size()-1).size()-1);
		}
	}
	
	cout << "edited echo:" << endl; //test
	for(unsigned i = 0; i < commands.size(); i++) {
		cout << "<" << commands.at(i) << "> ";
	}
	cout << endl;
	
    int i;
    char* argv[commands.size() + 1];
    for (i = 0; i < static_cast<int>(commands.size()); i++) { //convert the vector into a char* array for execvp
		argv[i] = (char*)commands.at(i).c_str();	
	}
	argv[commands.size()] = NULL;
	char path[20] = "/bin/"; // starts at bin	
	strcat(path, commands[0].c_str()); // add the first command to the file path
    pid_t pid = fork();
    if(pid >= 0){ //fork succeeded
        if(pid == 0) { //child process
            if (execvp(path, argv) < 0) {     /* execute the command  */
              execStatus = false; //error execvp failed
              perror("execvp failed.\n");
              exit(-1);
          }
          else {
              execStatus = true;
          }
        }
        else { //parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else { //fork failed
        execStatus = false;
        perror("fork failed\n");
        exit(-1); //exit(errno)?
    }
    cout << "exit execute" << endl;
}

void exec(vector<string> &argsList, bool &execStatus) {
    vector<string> currCommand;
    
    cout << "enter exec" << endl; //test
    for(unsigned i = 0; i < argsList.size(); i++) {
		cout << "<" << argsList.at(i) << "> ";
	}
	cout << endl;
	
    for(unsigned i = 0; i+1 < argsList.size(); i++) {
        currCommand.push_back(argsList.at(i));
        if(isComparator(currCommand.at(i)) && currCommand.size() > 1) {
        	cout << "num i: " << i << endl;
            if(currCommand.at(1) == "#" || currCommand.at(0) == "#") {
            	cout << "1" << endl;
                break;
            }
            else if(currCommand.at(0) == ";") {
                i--;     // to include next connector
                cout << "2" << endl;
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				execute(currCommand, execStatus);
				currCommand.clear();
            }
            else if(currCommand.at(0) == "&&") {
                i--;     // to include next connector
                cout << "3" << endl;
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				if(execStatus == true) {
				    execute(currCommand, execStatus);
				}
				currCommand.clear();
            }
            else if(currCommand.at(0) == "||") {
                i--;     // to include next connector
                cout << "4" << endl;
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				if(execStatus == false) {
				    execute(currCommand, execStatus);
				}
				currCommand.clear();
            }
            else if(!isComparator(currCommand.at(0))){  //first command
            	cout << "5" << endl;
                i--;     // to include next connector
				currCommand.pop_back(); //remove connector at end of command
				execute(currCommand, execStatus);
				cout << "return from execute" << endl;
				currCommand.clear();
				// cout << "cleared" << endl;
				cout << "num i after: " << i << endl;
            }
        }
        else if(i+1 == argsList.size()) { //if this is the last command
        	cout << "single/last command" << endl;
        	cout << "num i: " << i << endl;
			if(isComparator(currCommand.at(currCommand.size()-1))) { //if there is a dangling connector at the end remove it
				currCommand.pop_back();
				break;
			}
			else if(isComment(currCommand.at(1)) || isComment(currCommand.at(0))) {
                break;
            }
            else if(currCommand.at(0) == ";") {
                i--;     // to include next connector
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				execute(currCommand, execStatus);
				currCommand.clear();
            }
            else if(currCommand.at(0) == "&&") {
                i--;     // to include next connector
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				if(execStatus == true) {
				    execute(currCommand, execStatus);
				}
				currCommand.clear();
            }
            else if(currCommand.at(0) == "||") {
                i--;     // to include next connector
				currCommand.erase(currCommand.begin());  //delete the connector
				currCommand.pop_back(); //remove connector at end of command
				if(execStatus == false) {
				    execute(currCommand, execStatus);
				}
				currCommand.clear();
            }
            else if(!isComparator(currCommand.at(0))){  //single command
            	cout << "10" << endl;
                i--;     // to include next connector
				// currCommand.pop_back(); //remove connector at end of command
				execute(currCommand, execStatus);
				currCommand.clear();
				cout << "num i after: " << i << endl;
            }
            else if(currCommand.at(0) == "#" && currCommand.size() == 1) {
				break; //don't do anything if command has comments
			}
    	}
	}
	cout << "exit exec" << endl; //test
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
	strcpy(messagePointer, userInput.c_str()); //converts string to char*
	token = strtok(messagePointer, " "); //tokenize first part of string

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
		//if it a just a connector without any words attached  go straight to push_back
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
	// cout << "before: "; //test remove begin connector 
	// for(unsigned i = 0; i < argsList.size(); i++) {
	// 	cout << "<" << argsList.at(i) << "> ";
	// }
	// cout << endl;
	while(isComparator(argsList.at(0))) {
		argsList.erase(argsList.begin());
	}
	while(isComparator(argsList.at(argsList.size()-1))) {
		argsList.pop_back();
	}
	// cout << "after: ";
	// for(unsigned i = 0; i < argsList.size(); i++) {
	// 	cout << "<" << argsList.at(i) << "> ";
	// }
	// cout << endl;
}  
