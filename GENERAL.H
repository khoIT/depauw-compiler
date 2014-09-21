//Khoi Tran, Revised from Dave A. Berque Aug 2014
//This file contains some general constant definitions that will
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.

#ifndef _general
#define _general

const int MAXCELLSPERSTRING = 80;  

//Below are constant used to represent token type/subtypes.

#define ADDOP_T 10
#define ADD_ST 11
#define SUBSTRACT_ST 12
#define OR_ST 13

#define MULOP_T 20
#define MULTIPLY_ST 21
#define AND_ST 22
#define DIV_ST 23
#define MOD_ST 24

#define RELOP_T 30
#define LESS_ST 31
#define LESSOREQUAL_ST 32
#define EQUAL_ST 33
#define GREATEROREQUAL_ST 34
#define GREATER_ST 35
#define UNEQUAL_ST 36
#define NOTEQ_ST 37
#define SWAP_ST 38

#define INSERT_T 40
#define EXTRACT_T 41

#define LEFTPARENTH_T 50
#define RIGHTPARENTH_T 51
#define SEMICOLON_T 52
#define COMMA_T 53
#define DOT_T 54
#define AMPERSAND_T 55
#define TILDE_T 56 //extra
#define EXCLAIM_T 57 //extra
#define ATSIGN_T 58 //extra
#define POUND_T 59 //extra

#define STRING_T 70
#define IDENT_T 80
#define INTEGER_T 90
#define ASSIGNMENT_T 100
#define KEYWORD_T 110
#define EOF_T 120
#define NONE_ST 121

#define MAX_STATE 14
#define MAX_CHAR 129//128 ascii + EOF char
#define EOF_INDEX 128//Index of EOF in the matrix

#define ERROR_STATE -1
#define EMPTY_T -2
#define EMPTY_ST -3
#define EMPTY_LEXEME ""

enum Action {ACCEPT, ERROR, NOTHING, CLEAR_BUFFER, COMPILER_DIRECTIVE};

#endif