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

bool Execute::execute(vector<string>cmds) {
	if (cmds.at(0) == "exit") {
		exit(0);
	}
	if(cmds.size() > 1){
		if (cmds.at(1).at(0) == '\"'||cmds.at(1).at(0) ==  '\'') { //fixes quotation marks when executing echo
			cmds.at(1).erase(cmds.at(1).begin());
			cmds.at(cmds.size()-1).erase(cmds.at(cmds.size()-1).begin() + cmds.at(cmds.size()-1).size()-1);
		}
	}
	char* cmdArr[cmds.size() + 1];
	int i = 0;
	for (i = 0; i < static_cast<int>(cmds.size()); i++) {//convert the vector into a char* array for execvp
		cmdArr[i] = (char*)cmds.at(i).c_str();	
	}
	cmdArr[cmds.size()] = NULL; //set last value to NULL for execvp
	int status = 0;		
	char fullpath[20] = "/bin/"; //full file path, starts at bin	
	strcat(fullpath, cmds[0].c_str()); //add the first command to the file path
	pid_t pid = fork();//split the processes into parent and child
	if(pid==0) { //if the process is a child
		int exec = execvp(fullpath,cmdArr); // to check if execvp passes or fails	
		if (exec == -1) { //execvp failed 
			return false;
			perror("exec");
			exit(-1);
		}
	}
	else { //Otherwise, the process is a parent
		waitpid(pid, &status, 0); //wait for the child process to finish
	}	
	return true;
}
bool Execute::execTest(vector<string>cmds){
	struct stat file;
	bool hasFlag;
	for(unsigned i = 0; i < cmds.size(); i++) {
		cout << "<" << cmds.at(i) << ">";
	}
	cout << endl;
	if( cmds.at(1) != "-e" && cmds.at(1) != "-f" && cmds.at(1) != "-d"){
			hasFlag = false;
	}
	else {
			hasFlag = true;
	}
	
	
	
	if( cmds.at(1) == "-e" || !hasFlag ){ //does test -e or test with no specifications
		// cout << "-e or none" << endl;
		if( hasFlag && stat(cmds.at(2).c_str(), &file) == 0){
			// cout << "-e" << endl;
			cout << "(True)" << endl;
			return true;
		}
		else if (!hasFlag && stat(cmds.at(1).c_str(), &file) == 0){
			// cout << "none" << endl;
			cout << "(True)" << endl;
			return true;
		}
		else {
			cout << "(False)" << endl;
			return false;
		}
	}

	else if(cmds.at(1) == "-f"){	//does test witht -f, so checks if it is a file
		if(stat(cmds.at(2).c_str(), &file) == 0){
			if(S_ISREG(file.st_mode)){
				cout << "(True)" << endl;
				return true;
			}
			else{
				cout << "(False)" << endl;
				return false;
			}
		}
		else{
			cout << "(False)" << endl;
			return false;
		}
	}

	else if(cmds.at(1) == "-d"){ // does test with -d, so checks if it a directory
		if(stat(cmds.at(2).c_str(), &file) == 0) {
			if(S_ISDIR(file.st_mode)){
				cout << "(True)" << endl;
				return true;
			}
			else{
				cout << "(False)" << endl;
				return false;
			}
		}
		else{
			cout << "(False)" << endl;
			return false;
		}
	}
	return false;
}

void Execute::execMain(string ss, bool &execStatus) {
	vector<string>argsList; //holds the whole command line (parsed)
	vector<string>command; //holds individual commands to run in execute() function
	
	if (ss == "exit") {
		exit(0);
	}

	parseString(ss, argsList);

	
	for (unsigned i = 0; i < argsList.size(); i++) { //start where we left off, only executes when there is two connectors in between a command
		command.push_back(argsList.at(i));

		/*if (isComment(argsList.at(i))) { //if it is a comment, break out of the loop
			break;
		}*/
		if(isComparator(argsList.at(i)) && command.size() > 1) { //if there's more commands and the next command is a connector
			if(command.at(1) == "#") { //handles comments after connector
					break;
			}

			if (command.at(0) == "&&") {
				i--; //decrement i to include next connector
				command.erase(command.begin()); //remove connector from command
				command.pop_back(); //remove connector at the end
				if (execStatus == true) {
					if(isTest(command.at(0))){
						execStatus = execTest(command);
					}
					else{
						execStatus = execute(command);
					}
				}
				command.clear();
			}

			else if(command.at(0)== ";") {
				i--;
				command.erase(command.begin()); //remove connector from command
				command.pop_back(); //pop back twice
				if(isTest(command.at(0))){
					execStatus = execTest(command);
				}
				else{
					execStatus = execute(command);
				}
				command.clear();
			}
			else if(command.at(0) == "||") {
				i--; //decrement i to include next connector
				command.erase(command.begin()); //delete the connector
				command.pop_back(); //remove connector at end of command
				if (execStatus == false) { //if the previous command did not execute, run this command
					if(isTest(command.at(0))){
						execStatus = execTest(command);
					}
					else{
						execStatus = execute(command);
					}
				}
				else {
					execStatus = false;
				}
				command.clear();
			}
			else if(command.at(0) == "#" && command.size() == 1) {

				break; //don't do anything if command has comments
			}
			else if(!isComparator(command.at(0))){
				i--;
				command.pop_back(); //remove the connector
				if(isTest(command.at(0))){
						execStatus = execTest(command);
				}
				else{
					execStatus = execute(command);
				}
				command.clear();
			}
		}
		else if(i == argsList.size()-1) { //if this is the last command
			if(isComparator(command.at(command.size()-1))) { //if there is a dangling connector at the end remove it
				command.pop_back();
				break;
			}
			if (command.size() != 0 && command.at(0) == "&&") {
				command.erase(command.begin()); //delete connector
				if (execStatus == true) {
					if(isTest(command.at(0))){
						execStatus = execTest(command);
					}	
					else{
						execStatus = execute(command);
					}
				}
			}
			else if (command.size() != 0 && command.at(0) == "||") {
				command.erase(command.begin());
				if (execStatus == false) {
					if(isTest(command.at(0))){
						execStatus = execTest(command);
					}
					else{
						execStatus = execute(command);
					}
				}
			}
			else if (command.size() != 0 && command.at(0) == ";") {
				command.erase(command.begin());
				if(isTest(command.at(0))){
						execStatus = execTest(command);
				}
				else{
					execStatus = execute(command);
				}
				break;
			}
			else if (command.size() != 0 && command.at(0) == "#") {
				command.erase(command.begin());
				return;
			}
			else {
				if(isTest(command.at(0))){
					execStatus = execTest(command);
				}
				else{
					execStatus = execute(command);
				}
			}
		}
	}

}

void Parse::print() {
	char* user = getlogin();
	char host[256]; //creates a host name with buffer size
	gethostname(host, 128);
	if (user == NULL) { //could not find the username
		cout << "$ ";
	}
	else {
		cout << user << '@' << host << "$ ";
	}
}

bool Parse::hasOpenParenthesis(string s){ //checks for '('

	if(s.at(0) == '(') {
		return true; //return position of semi
	}
	return false; // if not found return -1
}

bool Parse::hasEndingParenthesis(string s){ //checks for ')'

	if(s.at(s.size() - 1) == ')'){
		return true; //return position of semi
	}
	return false; // if not found return -1
}

bool Parse::isComparator(string s) {
	if (s == "||" || s== "&&" || s == ";" || s == "#") {
		return true;
	}
	return false;	
}

bool Parse::hasSemiColon(string s){ //checks for semicolon attached to a word;
	for(unsigned i = 0; i < s.size(); ++i){
		if(s.at(i) == ';'){
				return true; //return position of semi
		}
	}
	return false; // if not found return -1
}

bool Parse::isComment(string s){
	for(unsigned i = 0; i < s.size(); ++i){
			if(s.at(i) == '#') {
					return true;
			}
	}
	return false;
}
bool Parse::isTest(string s){ //checks if command is a test case
	if(s == "[" || s == "test"){
			return true;
	}
	return false;
}


bool Parse::checkForParenthesis(string command) {
	stack<char>parenthesis;
	for(unsigned i = 0; i < command.size(); i++) {
		if(command.at(i) == '(') {
			parenthesis.push('(');
		}
		else if(command.at(i) == ')') {
			if(!parenthesis.empty()) {
				parenthesis.pop();
			}
			else {
				cout << "Parenthesis mismatch. Missing \'('." << endl;
				return false;
			}
		}
	}
	if(!parenthesis.empty()) {
		cout << "Parenthesis mismatch. Missing \')'." << endl;
		return false;
	}
	return true;
}

void Parse::remove_parenthesis(string &s) {
	for(unsigned i = 0; i < s.size(); i++) {
		if(s.at(i) == '(' || s.at(i) == ')') {
			s.erase(s.begin() + i);
		}
	}
}

void Parse::parseString(string ss, vector<string>&argsList) {
	
	char* token; //split command into separate strings
	char* cmd = new char[ss.length() + 1];
	strcpy(cmd, ss.c_str()); //converts string to char*
	token = strtok(cmd, " "); //tokenize first part of string
	int i = 0;
	for (i = 0; token != NULL; i++) {

		if (token == NULL) { //break out of for loop if there's an empty token
			break;
		}
		string tokenString = string(token);
		//cout << "Token is: " << tokenString << endl;
		if(!isComparator(tokenString)) { //if it a just a connector without any words attached  go straight to push_back
			if (hasSemiColon(tokenString)) {//if there is a semicolon attached enter loop
				string semicolon = ";";
				tokenString.erase(tokenString.begin() + tokenString.size() -1 ); //remove the semicolon at the end of word
				argsList.push_back(tokenString); // push back word without semicolon
				argsList.push_back(semicolon);// push back semi colon as its own string
			}
			else if(isComment(tokenString)) { //if there is a hash enter loop
				string hash = "#";
				tokenString.erase(tokenString.begin()); // remove hashtag located at front of string
				argsList.push_back(hash); // push back has as its own string
				argsList.push_back(tokenString); // push back word 
			}
			else{ 
					argsList.push_back(tokenString); // if it does not contain semi or hash push back word
			}
		}
		else if(hasEndingParenthesis(tokenString)){
			if(tokenString.size() == 1){
				argsList.push_back(tokenString);
			}
			else{
				string paran = ")";
				tokenString.erase(tokenString.begin() + tokenString.size() -1 ); //remove the semicolon at the end of word
				argsList.push_back(tokenString); // push back word without semicolon
				argsList.push_back(paran);// push back semi colon as its own string
			}
		}
		else if(hasOpenParenthesis(tokenString)){
			if(tokenString.size() == 1){
				argsList.push_back(tokenString);
			}
			else{
				string paran = "(";
				tokenString.erase(tokenString.begin()); // remove hashtag located at front of string
				argsList.push_back(paran); // push back has as its own string
				argsList.push_back(tokenString); // push back word 
			}
		}
		
		else{	
			argsList.push_back(tokenString);
			}
		token = strtok(NULL, " ");
	}
	
}  

void Parse::sepParenthesis(string command) { //parses the parenthesis and calls execMain on those parsed strings
	bool execStatus = true;
	checkForParenthesis(command);
	vector<string>parseCommands;
	vector<string>commandList;
	parseString(command,parseCommands);
	
	string comm;
	bool inParenthesis = false;
	for(unsigned i = 0; i < parseCommands.size();i++) {
		comm += parseCommands.at(i);
		if(i < parseCommands.size()-1) { //push individual command if we find a connector and we're not in the parenthesis
			if(isComparator(parseCommands.at(i+1)) && !inParenthesis) {
				remove_parenthesis(comm); //remove any parenthesis before pushing individual commands
				commandList.push_back(comm);
				comm.clear();
			}
		}
		if(i < parseCommands.size()-1 && !hasEndingParenthesis(parseCommands.at(i))) { //won't add a space to the end or after the end parenthesis
			comm += " ";
		}

		if(hasOpenParenthesis(parseCommands.at(i))) {
			inParenthesis = true;
		}
		else if(hasEndingParenthesis(parseCommands.at(i))) {
			remove_parenthesis(comm); //remove any parenthesis before pushing individual commands
			commandList.push_back(comm);
			comm.clear();
			inParenthesis = false;
		}
		else if(i == parseCommands.size() - 1) {
			remove_parenthesis(comm); //remove any parenthesis before pushing individual commands
			commandList.push_back(comm);
		}
	}


    Execute e;
	//execute individual commands based on precedence
	for (unsigned i = 0; i < commandList.size(); i++) {
		e.execMain(commandList.at(i),execStatus);
	}

}