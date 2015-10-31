/*
	Scanner.cpp: this file defines Scanner class. This class has most 
	scanning functions declared as static, but still can be used to construct
	a Scanner object and extract Tokens (using >>) from that object.

	Likai Yan
	Jianfeng Zeng
*/
#include "stdafx.h"
#include "Scanner.h"
#include <cctype>
#include <iostream>

using namespace std;
bool Scanner::comma(const string &s)
{
	if (s.compare(",") == 0) return true;
	return false;
}

bool Scanner::leftparen(const string &s)
{
	if (s.compare("(") == 0) return true;
	return false;
}

bool Scanner::rightparen(const string &s)
{
	if (s.compare(")") == 0) return true;
	return false;
}


bool Scanner::number(const string &s)
{
	if (s.length() == 0) return false;
	if (s.compare("0") == 0) return true;
	if (s.substr(0, 1).compare("0") == 0) return false;
	for (unsigned i = 0; i < s.length(); i++){
		if (!isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

bool Scanner::label(const string& s)
{
	for (unsigned i = 0; i < s.length(); i++){
		if (!isalpha((unsigned char)s[i])) return false;
	}
	return true;
}

bool Scanner::andToken(const string &s)
{
	if (s.compare("^") == 0) return true;
	return false;
}

bool Scanner::separator(const string &s)
{
	if (s.compare(":-") == 0) return true;
	return false;
}

auto Scanner::Scan(const string& s) -> shared_ptr<Token> 
	{
		// According to C++11 std, copy elision takes effect such
		// that "return unique_ptr" is effectively returning 
		// rval form of unique_ptr. 
		if (comma(s)) return shared_ptr<Token>(new Token(Token::COMMA, s));
		if (leftparen(s)) return shared_ptr<Token>(new Token(Token::LEFTPAREN, s));
		if (rightparen(s)) return shared_ptr<Token>(new Token(Token::RIGHTPAREN, s));
		if (number(s))	return shared_ptr<Token>(new numToken(s));
		// I wonder if slicing would happen if I use <labelToken>... 
		if (label(s)) return shared_ptr<Token>(new labelToken(s));
		if (andToken(s)) return shared_ptr<Token>(new Token(Token::AND, s));
		if (separator(s)) return shared_ptr<Token>(new Token(Token::SEPARATOR, s));
		// the UNKNOW empty token:
		return shared_ptr<Token>(new Token());
	}

/* obselete
vector<Token> Scanner::Tokenize(const string& Longline){
	vector <Token> ret;
	istringstream iss(Longline);
	string s;
	Token t;
	while (iss >> s){
		t.type = Scanner::Scan(s);
		t.value = s;
		ret.push_back(t);
	}
	string end("$eof$");
	t.value = end;
	t.type = Token::TokenType::END;
	ret.push_back(t);
	return ret;
}
*/

Scanner::Scanner(ifstream& in) :infile(in){}

Scanner::operator bool() const{
	return (bool)infile;
}

/*
Scanner& Scanner::operator >>(shared_ptr<Token>& t){
	string s;
	infile >> s;
	t = Scan(s);
	return *this;
}
*/

