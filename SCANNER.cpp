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
State::State():state(ERROR_STATE), action(NOTHING), token(NULL), actionInfo(NULL), pushBack(false){}

//Non-final state, add the char to the lexeme and move on
State::State(int state, bool pushBack = false) : state(state), action(NOTHING), token(NULL), actionInfo(NULL), pushBack(pushBack){}

//Accept state or final state
State::State(bool pushBack, int type, int subtype, string lexeme):state(0), pushBack(pushBack), token(new TokenClass(type,subtype,lexeme)), action(ACCEPT), actionInfo(NULL){}

//Error state
State::State(string errorMessage):state(0), action(ERROR), pushBack(false), token(NULL), actionInfo(new string(errorMessage)){}

//Everything else state
State::State(int state, Action action):state(state), action(action), pushBack(false), token(NULL) , actionInfo(NULL){}


std::ostream& operator<<(std::ostream &stream, const State &state) {
//Implement overloading << method
	if(state.token != NULL){
		return  stream << state.state <<" "<<TokenClass::tokenNumToString(state.token->type)<<" "<<TokenClass::tokenNumToString(state.token->subtype)<<" "<<state.token->lexeme;
	}
	return stream << state.state <<" "<<"NULL";
}

//Impletment methods of Scanner class
ScannerClass::ScannerClass(){
	buildStateMatrix();
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
		State s = stateMatrix[state][chr];
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
				cout<<"Warning, compiler directive "+ currentLexeme +" is undefined."<<endl;
			break;
		}

		state = s.state;//ready for next char
	}while(true);

	//In case something in the settoken process goes wrong
	return TokenClass(EMPTY_T,EMPTY_ST,EMPTY_LEXEME);
}

void ScannerClass::buildStateMatrix(){
	//keeps track of the number of all states in compiler
	int currentStateNum = 0; 

	for(int i = 0;i<MAX_STATE;++i)
		for(int j = 0;j<MAX_CHAR;++j)
			//default all error states
			stateMatrix[i][j] = State();

	//Read CR and whitespace at state 0 lead to 0
	char whitespaces[9] = {' ','\t','\n','\0','\a','\b','\f','\r','\v'};
	for(int i = 0;i<9;++i)
		stateMatrix[0][whitespaces[i]]=State(0);
	stateMatrix[0][EOF_INDEX] = State(EOF_INDEX);

	//Read *();+-,.~& and go to the respective final state
	stateMatrix[0]['*'] = State(false,MULOP_T,MULTIPLY_ST,"*");
	stateMatrix[0]['('] = State(false, LEFTPARENTH_T, NONE_ST, "(");
	stateMatrix[0][')'] = State(false, RIGHTPARENTH_T, NONE_ST, ")");
	stateMatrix[0]['+'] = State(false, ADDOP_T, ADD_ST, "+");
	stateMatrix[0]['-'] = State(false,ADDOP_T,SUBSTRACT_ST,"-");
	stateMatrix[0][','] = State(false, COMMA_T, NONE_ST, ",");
	stateMatrix[0]['.'] = State(false,DOT_T,NONE_ST,".");
	stateMatrix[0]['~'] = State(false,TILDE_T,NONE_ST,"~");
	stateMatrix[0][';'] = State(false,SEMICOLON_T,NONE_ST,";");
	stateMatrix[0]['&'] = State(false,AMPERSAND_T,NONE_ST,"&");
	stateMatrix[0]['!'] = State(false, EXCLAIM_T, NONE_ST, "!");
	stateMatrix[0]['@'] = State(false, ATSIGN_T, NONE_ST, "@");
	stateMatrix[0]['#'] = State(false, POUND_T, NONE_ST, "#");

	//Read digit
	int firstDigitState = ++currentStateNum;//current state is 4
	string digits = "0123456789";
	for (int i = 0; i<digits.length(); ++i)
	{
		stateMatrix[0][digits[i]] = State(firstDigitState);
	}
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i <= '9' && i >= '0')
			//keep reading in the characters. not check for length here
			stateMatrix[firstDigitState][i] = State(firstDigitState);
		else
			stateMatrix[firstDigitState][i] = State(true, INTEGER_T, NONE_ST, "int_lexeme");
	}

	//Read string
	int leftQuoteStateNum = ++currentStateNum;//State 5
	stateMatrix[0]['\''] = State(leftQuoteStateNum);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (i == '\'')
			stateMatrix[leftQuoteStateNum][i] = State(false, STRING_T, NONE_ST, "string_lexeme");
		else if (i == '\r' || i == '\n')
			stateMatrix[leftQuoteStateNum][i] = State("String cannot be split across lines.");
		else if (i == EOF_INDEX)
			stateMatrix[leftQuoteStateNum][i] = State("Single quote expected at the end of string.");
		else
			stateMatrix[leftQuoteStateNum][i] = State(leftQuoteStateNum);
	}

	//Read identifier
	int firstLetterStateNum = ++currentStateNum;//State 8
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < letters.length(); ++i)
		stateMatrix[0][letters[i]] = State(firstLetterStateNum);
	for (int i = 0; i<MAX_CHAR; ++i)
	{
		if (('A' <= i&&i <= 'z') || ('0' <= i&&i <= '9') || (i == '_'))
			stateMatrix[firstLetterStateNum][i] = State(firstLetterStateNum);
		else
			stateMatrix[firstLetterStateNum][i] = State(true, IDENT_T, NONE_ST, "identifier_lexeme");
	}

	//Read "="
	int firstEqualState = ++currentStateNum;//current state is 1;
	stateMatrix[0]['='] = State(firstEqualState);//Read first equal
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '=')
			stateMatrix[firstEqualState]['=']=State(false,RELOP_T,EQUAL_ST,"==");
		else
			stateMatrix[firstEqualState][i] = State(true,ASSIGNMENT_T,NONE_ST,"=");
	}

	//Read "<"
	int lessThanState = ++currentStateNum;//State 2
	stateMatrix[0]['<'] = State(lessThanState);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '<' )
			stateMatrix[lessThanState]['<'] = State(false, INSERT_T, NONE_ST,"<<");
		else if(i=='>')
			stateMatrix[lessThanState]['>'] = State(false, RELOP_T, NOTEQ_ST, "<>");
		else if (i == '=')
			stateMatrix[lessThanState]['='] = State(false, RELOP_T, LESSOREQUAL_ST, "<=");
		else
			stateMatrix[lessThanState][i] = State(true, RELOP_T, LESS_ST, "<");
	}

	//Read ">"
	int firstThanState = ++currentStateNum;//State 3
	stateMatrix[0]['>']=State(firstThanState);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '>')
			stateMatrix[firstThanState]['>'] = State(false, EXTRACT_T, NONE_ST, ">>");
		else if (i == '=')
			stateMatrix[firstThanState]['='] = State(false, RELOP_T, GREATEROREQUAL_ST, ">=");
		else if (i == '=')
			stateMatrix[firstThanState][i] = State(true, RELOP_T, GREATER_ST, ">");
		else 
			stateMatrix[firstThanState][i] = State(true, RELOP_T, SWAP_ST, "><");
	}

	//Read double slash started comment
	int openStringState = ++currentStateNum;//current state is 6
	int closeStringState = ++currentStateNum;//current is 7
	stateMatrix[0]['/'] = State(openStringState);
	for(int i = 0;i<MAX_CHAR;++i){
		if(i == '/')
			stateMatrix[openStringState][i] = State(closeStringState);
		else 
			stateMatrix[openStringState][i] = State("Comment should start with //.");
	}

	//Read new line
	for (int i = 0; i<MAX_CHAR; i++){
		if (i == '\r' || i == '\n')
			//if newline in the middle of string, ignore the lexemes
			stateMatrix[closeStringState][i] = State(0);
		else if (i == EOF_INDEX)
			//if reach eof in the middle of string, push back the lexemes
			stateMatrix[closeStringState][i] = State(0, true);
		else
			stateMatrix[closeStringState][i] = State(closeStringState, CLEAR_BUFFER);
	}

	//Read comment enclosed in brace
	int leftBraceState = ++currentStateNum;//current state is 9
	int secondDirectiveState = ++currentStateNum;
	int thirdDirectiveState = ++currentStateNum;
	int fourthDirectiveState = ++currentStateNum;
	int rightBraceState = ++currentStateNum;
	stateMatrix[0]['{'] = State(leftBraceState);
	for(int i = 0;i<MAX_CHAR;++i)
	{
		if(i == '}')
			//empty: ignore chars
			stateMatrix[leftBraceState][i] = State(0);
		else if(i == '$')
			stateMatrix[leftBraceState][i] = State(secondDirectiveState);
		else if(i == EOF_INDEX)
			stateMatrix[leftBraceState][i] = State("Missing } for Comment or compiler directives.");
		else 
			stateMatrix[leftBraceState][i] = State(rightBraceState);

		//comment or directive case
		if(i == '}')
			stateMatrix[secondDirectiveState][i] = State(0);
		else if (i <= 'z' && i >= 'A')
			stateMatrix[secondDirectiveState][i] = State(thirdDirectiveState);
		else if (i == EOF_INDEX)
			stateMatrix[secondDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else
			stateMatrix[secondDirectiveState][i] = State(rightBraceState);

		//directive
		if(i == '}')
			stateMatrix[thirdDirectiveState][i] = State(0);
		else if(i == '+' ||i == '-')
			stateMatrix[thirdDirectiveState][i] = State(fourthDirectiveState);
		else if (i == EOF_INDEX)
			stateMatrix[thirdDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else 
			stateMatrix[thirdDirectiveState][i] = State(rightBraceState);

		//directive 4th letter
		if(i == '}')
			stateMatrix[fourthDirectiveState][i] = State(0,COMPILER_DIRECTIVE);
		else if(i == EOF_INDEX)
			stateMatrix[fourthDirectiveState][i] = State("Missing } for Comment or compiler directives.");
		else stateMatrix[fourthDirectiveState][i] =State(rightBraceState);

		if(i == '}')
			stateMatrix[rightBraceState][i] = State(0);
		else if(i == EOF_INDEX)
			stateMatrix[rightBraceState][i] = State("Missing } for Comment or compiler directives.");
		else
			//we ignore comments so no need to store them anyway
			stateMatrix[rightBraceState][i] = State(rightBraceState, CLEAR_BUFFER);
	}
}

void ScannerClass::printStateMatrix()
{
	ofstream myfile;
	myfile.open ("stateMatrix.csv");

	for (int i = 0; i < MAX_STATE; i++)
	{
		for (int j = 0; j < MAX_CHAR; j++){
		myfile << stateMatrix[i][j] << ",";
		}
		myfile<<"\n";
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
			delete stateMatrix[i][j].actionInfo;
			delete stateMatrix[i][j].token;
		}
}
