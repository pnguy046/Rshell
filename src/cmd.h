#ifndef __CMD_H__
#define __CMD_H__

#include <iostream>
#include <vector>
#include <stack>

using namespace std;


class Parse {
 public:
    void print();
    
    bool hasOpenParenthesis(string);
    
    bool hasEndingParenthesis(string);
    
    bool isComparator(string);
    	
    bool hasSemiColon(string);
    
    bool isComment(string);
    	
    bool isTest(string);
    
    bool checkForParenthesis(string command);
    
    void remove_parenthesis(string&);
    
    void parseString(string, vector<string>&);
    	
    void sepParenthesis(string command);
};

class Execute : public Parse {
 private: 
    char cwd[256];
 public:
    char* getDirectory(string);
    
    bool execCD(vector<string>);
    
    bool execute(vector<string>);
    	
    bool execTest(vector<string>);
    
    void execMain(string, bool&);
    
   
};
#endif