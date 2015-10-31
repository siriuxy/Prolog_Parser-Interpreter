/*
	Scanner.h: This is the header file for Scanner class. This file identifies
	possible tokens Scanner can differentiate.

	Likai Yan
	Jianfeng Zeng
*/
#ifndef SCANNER_H_
#define SCANNER_H_
#include <string>
#include <locale>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include "Token.h"
using namespace std;


class Scanner{
public:
	// explicitly detele assignment operator and copy constructor
	// because we do not expect more than one instance.
	Scanner & operator=(const Scanner&) = delete;
	Scanner(const Scanner&) = delete;
	//Class encapsulated for staticfunctions only.
	static bool comma(const string &s);
	static bool leftparen(const string &s);
	static bool rightparen(const string &s);
	static bool number(const string &s);
	static bool label(const string& s);
	static bool andToken(const string& s);
	static bool separator(const string& s);
	// has to use auto return type, or VC compiler consider following 
	// as static variable declaration
	static auto Scan(const string& s)->shared_ptr<Token>;
	//static vector<Token> Tokenize(const string& strings);
	// decided to use static class method instead, as there is no need 
	// to store anything as a scanner.

	operator bool() const;
	Scanner& Scanner::operator >>(shared_ptr<Token>& t) {
		string s;
		infile >> s;
		t = Scan(s);
		return *this;
	}
	Scanner(ifstream& in);
private:
	ifstream& infile;
};
#endif