/*
Token.cpp: This is the declaration file for Token class. It declares constructors
and a few enum types
http://www.cse.wustl.edu/~cdgill/courses/cse425/lab2

Likai Yan
Jianfeng Zeng
*/
#pragma once
#include <string>
#include <sstream>

using namespace std;

struct Token{
	enum TokenType{
		UNKNOWN,
		NUMBER,
		LABEL,
		LEFTPAREN,
		RIGHTPAREN,
		COMMA,
		AND,
		SEPARATOR,
		END
	};
	TokenType type;
	string value;
	Token();
	Token(TokenType tType, const string& s);
	static string getTokenName(TokenType t);
};

struct labelToken : public Token {
	labelToken(const string& s) : Token(TokenType::LABEL, s){
	}
};

struct numToken : public Token {
	unsigned int numVal;
	numToken(const string& s) :Token(TokenType::NUMBER, s) {
		// Assume s contain a non-negative number. No throw.
		istringstream iss(s);
		iss >> numVal;
	}
};