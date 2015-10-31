#pragma once

/*
		Parser.h: This is the headerfile for Parser class. In the comment 
		at the bottom, I explained in detail how I transformed EBNF grammar to 
		attribute grammar that helps building abstract syntax tree.

		Likai Yan
		Jianfeng Zeng
*/

#include <memory>
#include "Scanner.h"
#include "Token.h"
#include "ASTNode.h"
#include "Table.h"
#include <string>
#include <set>
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
	// explicitly deleted, because one does not simply
	// need two instance of parser!
	Parser(const Parser& p) = delete;
	Parser& operator = (const Parser&) = delete;
	string print();
	string Parser::printSym();
private:
	ofstream ofile;
	ofstream& newGrammarOfile;
	//transition functions.
	vector<shared_ptr<Token>> tokens;
	vector<shared_ptr<Token>>::iterator it;
	vector<shared_ptr<Token>>::const_iterator errorStart;
	string currentParse;
	string recovery;

	set<string> orderedLabels;
	set<unsigned int> orderedNumbers;
	// symbol table
	Table symtab;

	shared_ptr<ASTNode> root;

	void match(Token::TokenType t, string currentProduction);
	

	void error(string expected, 
		Token::TokenType received, 
		string production); 
	vector<shared_ptr<ASTNode>> args();

	shared_ptr<ASTNode> hornclause();
	shared_ptr<ASTNode> head();
	shared_ptr<ASTNode> body();
	shared_ptr<ASTNode> predicate();
	shared_ptr<ASTNode> name();
	shared_ptr<ASTNode> symbol();
};


/* for lab 1, this parse has been modified to include attribute subroutine.
Following AG given in Scott:
E  => T { TT.st := T.n } TT { E.n := TT.n }
TT => + T { TT2.st := new bin_op(+, TT1.st, T.n) } TT { TT1.n := TT2.n }
TT => - T { TT2.st := new bin_op(-, TT1.st, T.n) } TT { TT1.n := TT2.n }
TT => { TT.n := TT.st }
T  => F { FT.st := F.n } FT { T.n := FT.n }
FT => * F { FT2.st := new bin_op(*, FT1.st, F.n) } FT { FT1.n := FT2.n }
FT => / F { FT2.st := new bin_op(/, FT1.st, F.n) } FT { FT1.n := FT2.n }
FT => { FT.n := FT.st }
F  => - F { F1.n := new un_op(-, F2.n) }
F  => ( E ) { F.n := E.n }
F  => const { F.n := new num(const.val) }
F  => id { F.n := new ident(is.name) }
(https://www.cs.rochester.edu/u/scott/courses/254/notes/04-semantics)

I modified the original hornclause grammr from:
hornclause -> head [SEPARATOR body]
head -> predicate
body -> predicate {AND predicate}
predicate -> name LEFTPAREN [args] RIGHTPAREN
name -> LABEL
args -> symbol {COMMA symbol}
symbol -> LABEL | NUMBER
(http://www.cse.wustl.edu/~cdgill/courses/cse425/lab0/)

To attribute grammar:
root -> hornclause root
hornclause -> head [body]
head -> predicate (predicate)*
body -> predicate (predicate)*
predicate -> name [(symbol)*]
name -> LABEL
symbol -> LABEL | NUMBER

And then added subroutines to construct	AST from AST node.
-Likai Yan
*/