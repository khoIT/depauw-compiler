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

//Define state object to be used to construct matrix of states
//Store the action to be taken upon reading a new character.
//State is the soul of this software because everytime compiler
//reads a new character, it is going to store a new state, at which
//appropriate action, message, or moving to next state ought to 
//be executed.  It is the spine of the compiler
class State{
public:
	State();
	State(int state, bool pushBack); 
	State(bool pushBack, int type, int subtype, string lexeme); 
	State(string errorMessage); //lexical error
	State(int state, Action everythingelse);
	friend ostream& operator<<(std::ostream& os, const State& stream); //to cout to matrix file
	int state;
	Action action;
	bool pushBack;
	string *actionInfo;
	TokenClass *token;
};


class ScannerClass{ 
public:
	ScannerClass();
	TokenClass getToken();
	void close();
	void printStateMatrix();
	int getCurrentLine();

private:
	State stateMatrix[MAX_STATE][MAX_CHAR];
	fileManagerClass fileManager;
	void buildStateMatrix();
}  ;


#endif
