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
	//locale loc;
	for (unsigned i = 0; i < s.length(); i++){
		if (!isalpha((unsigned char)s[i])) return false;
		//cast is somewhat required for ctype.h
		// isalpha accepts int, so we cast it into unsigned
		// and expect for implicit conversion.
	}
	return true;
	//It returns false upon "?, on my machine with US locale
	// still not portable.. let's use c's isalpha instead
}

bool Scanner::and(const string &s)
{
	if (s.compare("^") == 0) return true;
	return false;
}

bool Scanner::separator(const string &s)
{
	if (s.compare(":-") == 0) return true;
	return false;
}

Token::TokenType Scanner::Scan(const string& s){
	if (comma(s)) return Token::COMMA;
	if (leftparen(s)) return Token::LEFTPAREN;
	if (rightparen(s)) return Token::RIGHTPAREN;
	if (number(s))	return Token::NUMBER;
	if (label(s)) return Token::LABEL;
	if (and(s)) return Token::AND;
	if (separator(s)) return Token::SEPARATOR;
	return Token::UNKNOWN;
}

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

Scanner::Scanner(ifstream& in) :infile(in){}

Scanner::operator bool() const{
	//cout << "bool(): "<<infile.good();
	//return infile.good();
	return (bool)infile;
}

Scanner& Scanner::operator >>(Token& t){
	infile >> t.value;
	t.type = Scan(t.value);
	return *this;
}

