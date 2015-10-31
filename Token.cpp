/*
Token.cpp: This is the definition file for Token class. It defines constructors
and struct functions to use in this class.
http://www.cse.wustl.edu/~cdgill/courses/cse425/lab2

Likai Yan
Jianfeng Zeng
*/
#include "stdafx.h"
#include "Token.h"


Token::Token (){
	type = UNKNOWN;
	value = "";
}

Token::Token(TokenType tType, const string & s)
{
	type = tType;
	value = s;
}

string Token::getTokenName(TokenType t){
	switch (t){
	case UNKNOWN:
		return "UNKNOWN";
	case 	NUMBER:
		return "NUMBER";
	case 	LABEL:
		return "LABEL";
	case 	LEFTPAREN:
		return "LEFTPAREN";
	case 	RIGHTPAREN:
		return "RIGHTPAREN";
	case 	COMMA:
		return "COMMA";
	case 	AND:
		return "AND";
	case 	SEPARATOR:
		return "SEPARATOR";
	case 	END:
		return "END";
	}
	return "ERROR IN getTokenName()";
}