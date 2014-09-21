//Khoi Tran, Revised from Dave A. Berque Aug 2014
//This file contains implementations of the
//Token, State and Scanner classes.


#include "stdafx.h"  // Required for visual studio to work correctly
#include "scanner.h"
#include <string.h>

TokenClass::TokenClass():type(EMPTY_T),subtype(EMPTY_ST),lexeme(EMPTY_LEXEME){}

TokenClass::TokenClass(int type, int subtype, string lexeme):type(type),subtype(subtype),lexeme(lexeme){}

//Impletment methods of Token class
string TokenClass::tokenNumToString(int tokenNum)
{
	switch(tokenNum)
	{
	case EMPTY_T:
		return "EMPTY_T";
	case EMPTY_ST:
		return "EMPTY_ST";
	case NONE_ST:
		return "NONE_ST";

	case ADDOP_T:
		return "ADDOP_T";
	case ADD_ST:
		return "ADD_ST";
	case SUBSTRACT_ST:
		return "SUBSTRACT_ST";
	case OR_ST:
		return "OR_ST";

	case MULOP_T:
		return "MULOP_T";
	case MULTIPLY_ST:
		return "MULTIPLY_ST";
	case AND_ST:
		return "AND_ST";
	case DIV_ST:
		return "DIV_ST";
	case MOD_ST:
		return "MOD_ST";

	case RELOP_T:
		return "RELOP_T";
	case NOTEQ_ST:
		return "NOTEQ_ST";
	case SWAP_ST:
		return "SWAP_ST";
	case LESS_ST:
		return "LESS_ST";
	case LESSOREQUAL_ST:
		return "LESSOREQUAL_ST";
	case GREATEROREQUAL_ST:
		return "GREATEROREQUAL_ST";
	case GREATER_ST:
		return "GREATER_ST";
	case UNEQUAL_ST:
		return "UNEQUAL_ST";
	case EQUAL_ST:
		return "EQUAL_ST";

	case INSERT_T:
		return "INSERT_T";
	case EXTRACT_T:
		return "EXTRACT_T";


	case LEFTPARENTH_T:
		return "LEFTPARENTH_T";
	case RIGHTPARENTH_T:
		return "RIGHTPARENTH_T";
	case SEMICOLON_T:
		return "SEMICOLON_T";
	case COMMA_T:
		return "COMMA_T";
	case DOT_T:
		return "DOT_T";
	case AMPERSAND_T:
		return "AMPERSAND_T";
	case TILDE_T:
		return "TILDE_T";
	case EXCLAIM_T:
		return "EXCLAIM_T";
	case ATSIGN_T:
		return "ATSIGN_T";
	case POUND_T:
		return "POUND_T";

	case STRING_T:
		return "STRING_T";
	case IDENT_T:
		return "IDENT_T";
	case INTEGER_T:
		return "INTEGER_T";
	case ASSIGNMENT_T:
		return "ASSIGNMENT_T";
	case KEYWORD_T:
		return "KEYWORD_T";
	case EOF_T:
		return "EOF_T";

	default:
		return "Token index not recognized";
	}
}

void TokenClass::display(){
	cout << lexeme << "\t\t" << tokenNumToString(type) << "\t\t" << tokenNumToString(subtype) << endl;
}

//Impletment methods of State class
//Default state error: unless indicated otherwise, all cells in the matrix should be in error state
State::State():state(ERROR_STATE), action(NOTHING), actionInfo(NULL), token(NULL), pushBack(false){}

//Add the char to the lexeme and push back
State::State(int state, bool pushBack = false): state(state),  pushBack(pushBack), action(action), actionInfo(NULL), token(NULL){}

//Everything else state
State::State(int state, Action action):state(state), action(action), actionInfo(NULL), token(NULL), pushBack(false){}

//Accept state or final state
State::State(int type, int subtype, string lexeme, bool pushBack): state(0), token(new TokenClass(type,subtype,lexeme)), action(ACCEPT), actionInfo(NULL), pushBack(pushBack){}

//Print error message state
State::State(string errorMessage):state(0), action(ERROR), pushBack(false), token(NULL), actionInfo(new string(errorMessage)){}




std::ostream& operator<<(std::ostream &stream, const State &state) {
//Implement overloading << method
	if(state.token != NULL){
		return  stream << state.state <<" "<<TokenClass::tokenNumToString(state.token->type)<<" "<<TokenClass::tokenNumToString(state.token->subtype)<<" "<<state.token->lexeme;
	}
	return stream << state.state <<" "<<"NULL";
}

//Impletment methods of Scanner class
ScannerClass::ScannerClass(){
	buildStateTable();
}

TokenClass ScannerClass::getToken(){
	int state = 0;
	int chr;
	string currentLexeme;
	do
	{
		//First clear the buffer if it is a new state.
		if(state == 0)
			currentLexeme = "";

		//Second identify the char, with some special cases.
		chr = (int)fileManager.getNextChar();
		if(chr == EOF)//assume EOF is 128 - end of the used ascii system.
			chr = EOF_INDEX;

		if(chr >= MAX_CHAR)//all the chars in YASL definition is in first 128 ascii chars
			return TokenClass(EMPTY_T, EMPTY_ST, "Illegal symbol: " + chr);

		//Then lookup in the matrix
		State s = stateTable[state][chr];
		if(s.state == ERROR_STATE)
			return TokenClass(EMPTY_T, EMPTY_ST, "Invalid char sequence: " + (currentLexeme + (char)chr));

		if(s.state == EOF_INDEX)//If reached EOF, in this case no action needs to be taken.
			return TokenClass(EOF_T, NONE_ST, "EOF");

		switch(s.action){
		case NOTHING:
			//In process or reading, just store the char
			currentLexeme += chr;
			break;

		case ACCEPT:{
				if(!s.pushBack)
					currentLexeme += chr;
				else
					fileManager.pushBack();

				int type = s.token->type;

				if(type == INTEGER_T)//Check for interger length
					if(currentLexeme.length() > 4)
						return TokenClass(EMPTY_T, EMPTY_ST, "Integer can have at most four digits: " + currentLexeme);

				if(type == STRING_T)//Check for string length
					if(currentLexeme.length() > 52)
						return TokenClass(EMPTY_T, EMPTY_ST, "String can have at most fifty characters: " + currentLexeme);

				if(type == IDENT_T)//Check for identifier length
				{
					if(currentLexeme.length() > 12)
						return TokenClass(EMPTY_T, EMPTY_ST, "Identifier can have at most twelve characters: " + currentLexeme);

					const char* cString = currentLexeme.c_str();  //trim all unnecessary spaces

					if (_strcmpi("or", cString) == 0){
						return TokenClass(ADDOP_T, OR_ST, currentLexeme);
					}
					if(_strcmpi("and",cString)==0)
						return TokenClass(MULOP_T, AND_ST, currentLexeme);
					if(_strcmpi("div",cString)==0)
						return TokenClass(MULOP_T, DIV_ST, currentLexeme);
					if(_strcmpi("mod",cString)==0)
						return TokenClass(MULOP_T, MOD_ST, currentLexeme);

					char* keywords[16]={"program","function","begin","end","if","then","else","while","do","cout","cin","endl","int","boolean","true","false"};
					for(int i = 0;i<16;i++)
						if(_strcmpi(keywords[i],cString)==0)
							return TokenClass(KEYWORD_T,NONE_ST,currentLexeme);
				}

				return TokenClass(s.token->type,s.token->subtype,currentLexeme);
			}
		case ERROR:{
				if(s.actionInfo != NULL)
					return TokenClass(EMPTY_T,EMPTY_ST,*s.actionInfo);
				else
					//In case the error messag was forgotten.
					return TokenClass(EMPTY_T,EMPTY_ST,"Error");
			}
		case CLEAR_BUFFER:
			currentLexeme = ""; //Clear buffer, for comments only.
			break;
		case COMPILER_DIRECTIVE:
			currentLexeme += chr;
			if(currentLexeme != "{$p+}" && currentLexeme != "{$p-}")
				cout<<"Warning, compiler directive " + currentLexeme + " is undefined."<<endl;
			break;
		}


		state = s.state;//ready for next char
	}while(true);

	//In case something in the settoken process goes wrong
	return TokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
}

void ScannerClass::buildStateTableMinimized(){
	int currentStateNum = 0;
	for (int i = 0; i<MAX_STATE; ++i)
		for (int j = 0; j<MAX_CHAR; ++j)
			//default all error states
			ministateTable[i][j] = State();

	//Read whitespace col = 0
	stateTable[0][0] = State(0);

	// Read *();+-,.~&
	char special[13] = { '+', '(', ')', '+', '-', ',', '.', '~', ';', '&', '!', '@', '#' };
	stateTable[0][1] = State(1);
}

void ScannerClass::buildStateTable(){

	int currentStateNum = 0; //keeps track of the number of all states in compiler

	for(int i = 0;i<MAX_STATE;++i)
		for(int j = 0;j<MAX_CHAR;++j)
			//default all error states
			stateTable[i][j] = State();

	//Read CR and whitespace at state 0 lead to 0
	char whitespaces[9] = {' ','\t','\n','\0','\a','\b','\f','\r','\v'};
	for(int i = 0;i<9;++i)
		stateTable[0][whitespaces[i]]=State(0);
	stateTable[0][EOF_INDEX] = State(EOF_INDEX);

	//Read digit
	int firstDigitState = ++currentStateNum;//current state is 1
	string digits = "0123456789";
	for (int i = 0; i<digits.length(); ++i)
	{
		stateTable[0][digits[i]] = State(firstDigitState);
	}
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i <= '9' && i >= '0')
			//keep reading in the characters. not check for length here
			stateTable[firstDigitState][i] = State(firstDigitState);
		else
			stateTable[firstDigitState][i] = State(INTEGER_T, NONE_ST, "int_lexeme", true);
	}

	//Read identifier
	int firstLetterStateNum = ++currentStateNum;//current state is 2
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < letters.length(); ++i)
		stateTable[0][letters[i]] = State(firstLetterStateNum);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (('A' <= i&&i <= 'z') || ('0' <= i&&i <= '9') || (i == '_'))
			stateTable[firstLetterStateNum][i] = State(firstLetterStateNum);
		else
			stateTable[firstLetterStateNum][i] = State(IDENT_T, NONE_ST, "identifier_lexeme", true);
	}

	//Read string
	int leftQuoteStateNum = ++currentStateNum;//current state is 3
	stateTable[0]['\''] = State(leftQuoteStateNum);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i == '\'')
			stateTable[leftQuoteStateNum][i] = State(STRING_T, NONE_ST, "string_lexeme", false);
		else if (i == '\r' || i == '\n')
			stateTable[leftQuoteStateNum][i] = State("String cannot be split across lines.");
		else if (i == EOF_INDEX)
			stateTable[leftQuoteStateNum][i] = State("Single quote expected at the end of string.");
		else
			stateTable[leftQuoteStateNum][i] = State(leftQuoteStateNum);
	}


	//Read "="
	int firstEqualState = ++currentStateNum;//current state is 4;
	stateTable[0]['='] = State(firstEqualState);//Read first equal
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i == '=')
			stateTable[firstEqualState]['='] = State(RELOP_T, EQUAL_ST, "==", false);
		else
			stateTable[firstEqualState][i] = State(ASSIGNMENT_T, NONE_ST, "=", true);
	}

	//Read "<"
	int lessThanState = ++currentStateNum;//State 5
	stateTable[0]['<'] = State(lessThanState);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i == '<')
			stateTable[lessThanState]['<'] = State(INSERT_T, NONE_ST, "<<", false);
		else if (i == '>')
			stateTable[lessThanState]['>'] = State(RELOP_T, NOTEQ_ST, "<>", false);
		else if (i == '=')
			stateTable[lessThanState]['='] = State(RELOP_T, LESSOREQUAL_ST, "<=", false);
		else
			stateTable[lessThanState][i] = State(RELOP_T, LESS_ST, "<", true);
	}

	//Read ">"
	int firstThanState = ++currentStateNum;//State 6
	stateTable[0]['>'] = State(firstThanState);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i == '>')
			stateTable[firstThanState]['>'] = State(EXTRACT_T, NONE_ST, ">>", false);
		else if (i == '=')
			stateTable[firstThanState]['='] = State(RELOP_T, GREATEROREQUAL_ST, ">=", false);
		else if (i == '=')
			stateTable[firstThanState][i] = State(RELOP_T, GREATER_ST, ">", true);
		else
			stateTable[firstThanState][i] = State(RELOP_T, SWAP_ST, "><", true);
	}

	//Read double slash started comment
	int openStringState = ++currentStateNum;//current state 7
	int closeStringState = ++currentStateNum;
	stateTable[0]['/'] = State(openStringState);
	for(int i = 0;i<MAX_CHAR;++i){
		if(i == '/')
			stateTable[openStringState][i] = State(closeStringState);
		else
			stateTable[openStringState][i] = State("Comment should start with //.");
	}

	//Read new line
	for (int i = 0; i<MAX_CHAR; i++){
		if (i == '\r' || i == '\n')
			//if newline in the middle of string, ignore the lexemes
			stateTable[closeStringState][i] = State(0);
		else if (i == EOF_INDEX)
			//if reach eof in the middle of string, push back the lexemes
			stateTable[closeStringState][i] = State(0, true);
		else
			stateTable[closeStringState][i] = State(closeStringState, CLEAR_BUFFER);
	}

	//Read comment enclosed in brace
	int leftBraceState = ++currentStateNum;//current state is 9
	int secondDirectiveState = ++currentStateNum;
	int thirdDirectiveState = ++currentStateNum;
	int fourthDirectiveState = ++currentStateNum;
	int rightBraceState = ++currentStateNum;
	stateTable[0]['{'] = State(leftBraceState);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '}')
			//empty: ignore chars
			stateTable[leftBraceState][i] = State(0);
		else if(i == '$')
			stateTable[leftBraceState][i] = State(secondDirectiveState);
		else if(i == EOF_INDEX)
			stateTable[leftBraceState][i] = State("Missing } for Comment or compiler directives.");
		else
			stateTable[leftBraceState][i] = State(rightBraceState);

		//comment or directive case
		if(i == '}')
			stateTable[secondDirectiveState][i] = State(0);
		else if (i <= 'z' && i >= 'A')
			stateTable[secondDirectiveState][i] = State(thirdDirectiveState);
		else if (i == EOF_INDEX)
			stateTable[secondDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else
			stateTable[secondDirectiveState][i] = State(rightBraceState);

		//directive
		if(i == '}')
			stateTable[thirdDirectiveState][i] = State(0);
		else if(i == '+' ||i == '-')
			stateTable[thirdDirectiveState][i] = State(fourthDirectiveState);
		else if (i == EOF_INDEX)
			stateTable[thirdDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else
			stateTable[thirdDirectiveState][i] = State(rightBraceState);

		//directive 4th letter
		if(i == '}')
			stateTable[fourthDirectiveState][i] = State(0, COMPILER_DIRECTIVE);
		else if(i == EOF_INDEX)
			stateTable[fourthDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else stateTable[fourthDirectiveState][i] =State(rightBraceState);

		if(i == '}')
			stateTable[rightBraceState][i] = State(0);
		else if(i == EOF_INDEX)
			stateTable[rightBraceState][i] = State("Missing } for Comment or compiler directives.");
		else
			//we ignore comments so no need to store them anyway
			stateTable[rightBraceState][i] = State(rightBraceState, CLEAR_BUFFER);
	}

	//Read arithmetic operator and go to the respective final state
	stateTable[0]['*'] = State(MULOP_T, MULTIPLY_ST, "*", false);
	stateTable[0]['('] = State(LEFTPARENTH_T, NONE_ST, "(", false);
	stateTable[0][')'] = State(RIGHTPARENTH_T, NONE_ST, ")", false);
	stateTable[0]['+'] = State(ADDOP_T, ADD_ST, "+", false);
	stateTable[0]['-'] = State(ADDOP_T, SUBSTRACT_ST, "-", false);
	stateTable[0][','] = State(COMMA_T, NONE_ST, ",", false);
	stateTable[0]['.'] = State(DOT_T, NONE_ST, ".", false);
	stateTable[0]['~'] = State(TILDE_T, NONE_ST, "~", false);
	stateTable[0][';'] = State(SEMICOLON_T, NONE_ST, ";", false);
	stateTable[0]['&'] = State(AMPERSAND_T, NONE_ST, "&", false);
	stateTable[0]['!'] = State(EXCLAIM_T, NONE_ST, "!", false);
	stateTable[0]['@'] = State(ATSIGN_T, NONE_ST, "@", false);
	stateTable[0]['#'] = State(POUND_T, NONE_ST, "#", false);
}

void ScannerClass::printStateTable()
{
	ofstream myfile;
	myfile.open ("stateTable.csv");

	for (int i = 0; i < MAX_STATE; i++)
	{
		for (int j = 0; j < MAX_CHAR; j++){
		myfile << stateTable[i][j] << ",";
		}
		myfile<<"\n";
	}
	myfile.close();
}

void ScannerClass::printminiStateTable()
{
	ofstream myfile;
	myfile.open("miniStateTable.csv");

	for (int i = 0; i < MAX_STATE; i++){
		for (int j = 0; j < MAX_CHAR; j++){
			myfile << ministateTable[i][j] << ",";
		}
		myfile << "\n";
	}
	myfile.close();
}

int ScannerClass::getCurrentLine()
{
	return fileManager.numLinesProcessed();
}

void ScannerClass::close()
{
	fileManager.closeSourceProgram();
	//Clear the memory used by the state matrix
	for(int i = 0;i<MAX_STATE;i++)
		for(int j = 0;j<MAX_CHAR;j++)
		{
			delete stateTable[i][j].actionInfo;
			delete stateTable[i][j].token;
		}
}
