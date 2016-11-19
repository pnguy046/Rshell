#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <iostream>
#include <vector>

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
 public:
    bool execute(vector<string>);
    	
    bool execTest(vector<string>);
    
    void execMain(string, bool&);
};
#endif
