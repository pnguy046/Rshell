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

int main(int argc, char* argv[]){
    
    
    bool loopingStatus = true;
    
    
    //to keep reading in the user input until enter or "exit" is reached
    while (loopingStatus){
        //|| user_input == '\n'){
        
        string user_input;
        vector <char *> args_list;
        
        //get userinput
        getline(cin, user_input);
       
        
        char* message_pointer = new char [user_input.length()+1];
        strcpy(message_pointer, user_input.c_str());
        //this seperates each word after a space is read in
         
        message_pointer = strtok(message_pointer, " "); //<- may need to test for (";")
        //now this holds the pointers for the chars and pushes them back into a vector of char*
        while( message_pointer != NULL)
        {
            //cout << message_pointer << endl;
            args_list.push_back(message_pointer);
            message_pointer = strtok(NULL, " ");
            
        }
        
        
        //after storing the commands/comparators, then use conditional statements to compare each action (i think that execvp will do the main commands with the fork)
        
        for(unsigned i = 0; i < args_list.size(); i++){
            
            //made exit work with goto -> stop_status
            //may need to see if it works if exit is the last word so that
            //the program doesn't keep on doing processes
            if(args_list.at(i) == "exit")
            {
                goto stop_status;
                
            }
            
            //if(args_list.at(i) == '&')
            //push back a comparator (is this forking/execvp/waitpid?)
            //else if(args_list.at(i) == '|')
            //else if(args_list.at(i) == ';')
            
        }
        
       
        
       
    
        
        stop_status: cout << "Exit has been entered. Process terminated" << endl;
        loopingStatus = false;
        break;
        
        
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