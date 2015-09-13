//modular design that supports both synStu1 and synStu2
//by using constructor that requires specification of parsed result
#include "Scanner.h"
#include "string"
using namespace std;

enum parseType{
	UNDEFINED,

	// Assume each list of token contain only tokens for one valid production
	ONEPRODUCT, 

	// store all possible parsed result, valid or not. Parse eagerly.
	STOREALL, // for forward compatibility
};

// declare CSTnode struct with unique_ptr, for later use.

enum errorType{
	NONE,
	OCCURED,
};

class Parser{
public:

	Parser(ifstream& i, ofstream& o);
	void Parse();
//	vector<Token> getParseResult();

private:
	ofstream& ofile;
	//transition functions.
	vector<Token> tokens;
	vector<Token>::const_iterator it;
	vector<Token>::const_iterator errorStart;
	string currentParse;
	string recovery;

	void match(Token::TokenType t, string currentProduction);
	void error(string expected, 
		Token::TokenType received, 
		string production); //better error handling with return?
	//void error(string production);
	// void function();
	void args();

	void hornclause();
	void head();
	void body();
	void predicate();
	void name();

	void symbol();

	void print(...); // new line after one production. 

};