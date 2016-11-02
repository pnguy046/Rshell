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

void execute();
void parsing(string, vector<string>&);

bool hasSemi(string s);
bool hasHash(string s);
bool isConnector(string s);

int main(int argc, char* argv[]){
    
    
    bool loopingStatus = true;
    
    
    //to keep reading in the user input until enter or "exit" is reached
    while (loopingStatus){
        //|| user_input == '\n'){
        
        string user_input;
        vector <string> args_list;
        vector <string> temp;
        
        //get userinput
        getline(cin, user_input);
       
        
        char* message_pointer = new char [user_input.length()+1];
        strcpy(message_pointer, user_input.c_str());
        //this seperates each word after a space is read in
         
        message_pointer = strtok(message_pointer, " "); //<- may need to test for (";")
        //now this holds the pointers for the chars and pushes them back into a vector of char*
        while( message_pointer != NULL)
        {
            cout << message_pointer << endl;
            args_list.push_back(message_pointer);
            if(message_pointer == ";")
            {
                args_list.push_back(";");
                message_pointer = NULL;
            }
            message_pointer = strtok(NULL, " ");
            
        }
        
        
        //after storing the commands/comparators, then use conditional statements to compare each action (i think that execvp will do the main commands with the fork)
        
        for(unsigned i = 0; i < args_list.size(); i++){
            
            //made exit work with goto -> stop_status
            //may need to see if it works if exit is the last word so that
            //the program doesn't keep on doing processes
            if(args_list.at(i) == "exit")
            {
                loopingStatus = false;
                exit(0);
                
            }
            
            //if(args_list.at(i) == '&')
            // {
            //  if child executes, then parent
            // }
            //push back a comparator (is this forking/execvp/waitpid?)
            //else if(args_list.at(i) == '|')
            // {
            //    if child fails, parent succeeds
            // }
            //else if(args_list.at(i) == ';') <- dont know
            
        }
        
       
        
       
    
        
        // stop_status: cout << "Exit has been entered. Process terminated" << endl;
        // loopingStatus = false;
        // break;
        
        
    }
    
    
}

void execute(char **argv) { //passed in command
    pid_t pid = fork();
    if(pid >= 0){ //fork succeeded
        if(pid == 0) { //child process
            if (execvp(argv[0], argv) < 0) {     /* execute the command  */
              printf("*** ERROR: exec failed\n"); //error execvp failed
              perror("execvp failed.\n");
              exit(1);
          }
        }
        else { //parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else { //fork failed
        printf("*** ERROR: forking child process failed\n"); 
        perror("fork failed\n");
        exit(errno);
    }
    
}

//fork process, child process call the execvp


//some example to fall back on for learning execvp

// void  parse(char *line, char **argv)
// {
//      while (*line != '\0') {       /* if not the end of line ....... */ 
//           while (*line == ' ' || *line == '\t' || *line == '\n')
//               *line++ = '\0';     /* replace white spaces with 0    */
//           *argv++ = line;          /* save the argument position     */
//           while (*line != '\0' && *line != ' ' && 
//                  *line != '\t' && *line != '\n') 
//               line++;             /* skip the argument until ...    */
//      }
//      *argv = '\0';                 /* mark the end of argument list  */
// }

// void  execute(char **argv) // incorporated this into execute() above
// {
//      pid_t  pid;
//      int    status;

//      if ((pid = fork()) < 0) {     /* fork a child process           */
//           printf("*** ERROR: forking child process failed\n");
//           exit(1);
//      }
//      else if (pid == 0) {          /* for the child process:         */
//           if (execvp(*argv, argv) < 0) {     /* execute the command  */
//               printf("*** ERROR: exec failed\n");
//               exit(1);
//           }
//      }
//      else {                                  /* for the parent:      */
//           while (wait(&status) != pid)       /* wait for completion  */
//               ;
//      }
// }


//for waitpid, maybe use this in execute?

//  int main(void)
//   {
//      int i, status;
//      pid_t childID, endID;
//      time_t when;

//      if ((childID = fork()) == -1) {     /* Start a child process.      */
//         perror("fork error");
//         exit(EXIT_FAILURE);
//      }
//      else if (childID == 0) {            /* This is the child.          */
//         time(&when);
//         printf("Child process started at %s", ctime(&when));
//         sleep(10);                       /* Sleep for 10 seconds.       */
//         exit(EXIT_SUCCESS);
//      }
//      else {                              /* This is the parent.         */
//         time(&when);
//         printf("Parent process started at %s", ctime(&when));

//           /* Wait 15 seconds for child process to terminate.           */
//         for(i = 0; i < 15; i++) {
//           endID = waitpid(childID, &status, WNOHANG|WUNTRACED);
//           if (endID == -1) {            /* error calling waitpid       */
//               perror("waitpid error");
//               exit(EXIT_FAILURE);
//           }
//           else if (endID == 0) {        /* child still running         */
//               time(&when);
//               printf("Parent waiting for child at %s", ctime(&when));
//               sleep(1);
//           }
//           else if (endID == childID) {  /* child ended                 */
//               if (WIFEXITED(status))
//                  printf("Child ended normally.n");
//               else if (WIFSIGNALED(status))
//                  printf("Child ended because of an uncaught signal.n");
//               else if (WIFSTOPPED(status))
//                  printf("Child process has stopped.n");
//               exit(EXIT_SUCCESS);
//           }
//         }
//      }
//   }

//example for forking

// int main(int argc, char **argv)
// {
//     printf("--beginning of program\n");

//     int counter = 0;
//     pid_t pid = fork();

//     if (pid == 0)
//     {
//         // child process
//         int i = 0;
//         for (; i < 5; ++i)
//         {
//             printf("child process: counter=%d\n", ++counter);
//         }
//     }
//     else if (pid > 0)
//     {
//         // parent process
//         int j = 0;
//         for (; j < 5; ++j)
//         {
//             printf("parent process: counter=%d\n", ++counter);
//         }
//     }
//     else
//     {
//         // fork failed
//         printf("fork() failed!\n");
//         return 1;
//     }

//     printf("--end of program--\n");

//     return 0;
// }

// int main(int argc, char *argv[])  //example 
// {
//         cout << "program has started\n";
 
//         // issue prompt
//         cout << "> ";
 
//         // get input from user
//         bool background = false;
//         string inputStr;
//         string commandStr;
//         char inputStrArr[128];
//         char * argArr[128];
//         char separators[]   = " ,\t\n";
//         char *token, *command;
//         int numArgs = -1;
//         int k;
//         int i;
 
//         getline(cin, inputStr);
//         while (inputStr.length() == 0)
//         {
//                 cout << "> ";
//                 getline(cin, inputStr);
//         }
 
//         for(i=0; i<inputStr.length(); i++)
//                 inputStrArr[i] = inputStr[i];
//         inputStrArr[i] = '\0';
                                                                    
//     token = strtok( inputStrArr, separators );
//         command = token;
 
//         for (i=0; token[i] != '\0'; i++)
//         {
//                 commandStr += token[i];
 
//         }
 
//         argArr[++numArgs] = token;
 
//         while( token != NULL )
//         {
//                 token = strtok( NULL, separators );
//                 argArr[++numArgs] = token;
//         }
 
//         if (argArr[numArgs] == '&')
//         {
//                 background = true;
//                 argArr[numArgs] = NULL;
//         }
//         else
//                 argArr[++numArgs] = NULL;
 
//     // is command built-in?
//         if(commandStr == "quit")
//         {
//                 cout << "============= quit called\n";
//                 return 0;
//         }
//         else
//         {
//         // execute external command
//             int childpid, flag;
//             childpid = fork();
//             cout << "forking\n";
//             if (childpid == 0) // child code
//             {
//                     cout << "Process ID: child: " << getpid();
//                     flag = execvp(command, argArr);
     
//                     if (flag < 0)
//                     {
//                             perror("execvp failed");
//                     }
//             }
//             else if (childpid > 0) // parent code
//             {
//                     cout << "Process ID: " << getpid();
//             }
//             else // fork unsuccessful
//             {
//                     cout << "\nError: fork failed\n";
//             }
//     } // end else
// return 0;
// }

// int main()   //vector into array for execvp
// {
//     std::vector<std::string> args;
//     args.push_back("ls");
//     args.push_back("-l");

//     const size_t array_size = args.size();

//     char** arg_array = new char*[array_size + 1];  // extra space for terminating NULL

//     for (size_t i = 0; i < array_size; ++i)
//     {
//         arg_array[i] = strdup(args[i].c_str());
//     }
//     arg_array[array_size] = 0;

//     execvp(arg_array[0], arg_array);

//     std::cout << "This statement is never reached." << std::endl;
// }

// int main(void) {   //better way to make vector for execvp /*Works*/
//   vector<char *> commandVector;

//   // do a push_back for the command, then each of the arguments
//   commandVector.push_back((char *)"echo");       /* add (char *) to avoid warning:deprecated conversion from string constant to ‘char*’
//   commandVector.push_back((char *)"testing");
//   commandVector.push_back((char *)"1");
//   commandVector.push_back((char *)"2");
//   commandVector.push_back((char *)"3");  

//   // push NULL to the end of the vector (execvp expects NULL as last element)
//   commandVector.push_back(NULL);

//   // pass the vector's internal array to execvp
//   char **command = &commandVector[0];

//   int status = execvp(command[0], command);
//   return 0;
// }

bool isComparator(string s) {
	if (s == "||" || s== "&&" || s == ";" || s == "#") {
		return true;
	}
	return false;	
}

bool hasSemiColon(string s){ //checks for semicolon attached to a word;
	for(unsigned i = 0; i < s.size(); ++i){
		if(s.at(i) == ';'){
				return true; //return position of semi
		}
	}
	return false; // if not found return -1
}

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
	char* token; //split command into separate strings
	char* messagePointer = new char[userInput.length() + 1];
	strcpy(messagePointer, userInput.c_str()); //converts string to char*
	token = strtok(messagePointer, " "); //tokenize first part of string
	int i = 0;
	for (i = 0; token != NULL; i++) {

		if (token == NULL) { //break out of for loop if there's an empty token
			break;
		}
		string parsedString = string(token);
		if(!isComparator(parsedString)) { //if it a just a connector without any words attached  go straight to push_back
			if (hasSemiColon(parsedString)) {//if there is a semicolon attached enter loop
				string semicolon = ";";
				parsedString.erase(parsedString.begin() + parsedString.size() -1 ); //remove the semicolon at the end of word
				argsList.push_back(parsedString); // push back word without semicolon
				argsList.push_back(semicolon);// push back semi colon as its own string
			}
			else if(isComment(parsedString)) { //if there is a hash enter loop
				string hash = "#";
				parsedString.erase(parsedString.begin()); // remove hashtag located at front of string
				argsList.push_back(hash); // push back has as its own string
				argsList.push_back(parsedString); // push back word 
			}
			else{ 
					argsList.push_back(parsedString); // if it does not contain semi or hash push back word
			}
		}
		else{	
			argsList.push_back(parsedString);
			}
		token = strtok(NULL, " ");
	}
	
}  

