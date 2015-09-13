#ifndef SCANNER_H_
#define SCANNER_H_
#include <string>
#include <locale>
#include <vector>
#include <sstream>
#include <fstream>
#include "Token.h"
using namespace std;



class Scanner{
public:
	//Scanner() = delete; //do not allow object construction. 
			//Class encapsulated for staticfunctions only.
	static bool comma(const string &s);
	static bool leftparen(const string &s);
	static bool rightparen(const string &s);
	static bool number(const string &s);
	static bool label(const string& s);
	static bool and(const string& s);
	static bool separator(const string& s);
	static Token::TokenType Scan(const string& s);
	static vector<Token> Tokenize(const string& strings);
	// decided to use static class method instead, as there is no need 
	// to store anything as a scanner.

	operator bool() const;
	Scanner& operator >> (Token& t);
	Scanner(ifstream& in);
	//what is the design consideration to use ifstream, but not reading all line at once?
private:
	ifstream& infile;
};
#endif