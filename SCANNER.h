//Khoi Tran, Revised from Dave A. Berque Aug 2014
//This file contains definition of classes for the scanning 
//system YASL compiler.

#ifndef _scanner
#define _scanner

#include "filemngr.h"
#include <iostream>
#include <fstream>
#include <string>
#include "general.h"


class TokenClass{ 
public:
	TokenClass();
	TokenClass(int type, int subtype, string lexeme);
	int type;
	int subtype;
	string lexeme;
	static string tokenNumToString(int tokenNum);
	void display();
};

//Define state object to be used to construct table of states
//State is the soul of this software because everytime compiler
//reads a new character, it is going to store a new state, at which
//appropriate action, message, and next state ought to 
//be determined. The getToken() will later read info from this table
class State{
public:
	State(); //initial state
	State(int state, bool pushBack); 
	State(int state, Action everythingelse);
	State(int type, int subtype, string lexeme, bool pushBack);
	State(string errorMessage); //lexical error
	
	friend ostream& operator<<(std::ostream& os, const State& stream); //to cout to matrix file
	int state;
	Action action;
	string *actionInfo;
	TokenClass *token;
	bool pushBack;
	
	
};


class ScannerClass{ 
public:
	ScannerClass();
	TokenClass getToken();
	void close();
	void printStateTable();
	void printminiStateTable();
	int getCurrentLine();

private:
	State stateTable[MAX_STATE][MAX_CHAR];
	State ministateTable[MAX_STATE][MAX_CHAR];
	fileManagerClass fileManager;
	void buildStateTable();
	void buildStateTableMinimized();
};

#endif
