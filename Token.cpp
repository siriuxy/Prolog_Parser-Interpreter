#include "stdafx.h"
#include "Token.h"

Token::Token (){
	type = UNKOWN;
	value = "";
}

string Token::getTokenName(TokenType t){
	switch (t){
	case UNKOWN:
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