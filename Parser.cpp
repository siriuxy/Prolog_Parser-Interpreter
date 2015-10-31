/*
	Parser.cpp defines Parser class: this class includes a series
	of functions that collaborates to finish top-down descent parsing;
	it also include print functions to print out symbols or parsed result.

	Likai Yan
	Jianfeng Zeng
*/
#include "stdafx.h"
#include "Parser.h"
#include <iostream>


Parser::Parser(ifstream& i, ofstream& o) :newGrammarOfile(o) {
	Scanner s(i);
	shared_ptr<Token> t;
	// move semantics to transfer unique onwership 
	// from Scanner to Parser;
	while (s >> t) {

		tokens.push_back(move(t));
	}
	it = tokens.begin();
	Parse();
	cout << symtab.print();
	symtab.match();
}

string Parser::print() {
	return root->print();
};

string Parser::printSym() {
	string p;
	for (auto s : orderedLabels) {
		p += s;
		p += "\n";
	}
	p += "\n";
	for (auto s : orderedNumbers) {
		p += to_string(s);
		p += "\n";
	}
	return p;
}

void Parser::Parse() {
	root = shared_ptr<ASTNode>{ new ASTNode{ASTNode::ROOT} };
	while (it != tokens.end()) {
		try {
			currentParse.clear();
			recovery.clear();
			errorStart = it;
			// error handling:
			if ((**it).type != Token::LEFTPAREN) {
				auto currentIt = it;
				if (it != tokens.end())
					it++;
				error(Token::getTokenName(Token::LEFTPAREN)
					, ((currentIt == tokens.end()) ?
						Token::END : (**currentIt).type)
					, "hornclause -> LEFTPAREN head [body] RIGHTPAREN");
			}
			root->children.push_back(hornclause());
			ofile << currentParse << " " << endl;
		}
		catch (exception& e) {
			if (!recovery.empty()) ofile << recovery << " " << endl;
			cout << endl << "ERROR!" << " At: \"";

			// print as much information as possible, without go out of range in token vector
			auto endOfError = (it == tokens.end()) ? tokens.end() : it + 1;

			// increment iterator = end() is UNDEFINED! So we use previous line to select an end
			for (auto i = errorStart; i != endOfError; i++) {
				cout << (**i).value << " ";
			}
			cout << "\" " << e.what() << endl;
		}
	}
	newGrammarOfile << print();
}

void Parser::match(Token::TokenType t, string currentProduction) {
	if (it == tokens.end()) throw runtime_error("Reaches End");
	if ((**it).type == t) {
		currentParse += ((**it).value + " ");
		it++;
	}
	else {
		throw runtime_error(": Attempted production " + currentProduction
			+ " , but failed to match " + Token::getTokenName((**it).type)
			+ "(" + (**it).value + ")"
			+ " with " + "expected " + Token::getTokenName(t));
	}
}


void Parser::error(string expected, Token::TokenType received, string currentProduction) {
	currentParse.clear();
	throw runtime_error(": In production " + currentProduction
		+ " but failed to match  " + Token::getTokenName(received)
		+ "  with expected: " + expected);
}

shared_ptr<ASTNode> Parser::hornclause() {
	shared_ptr<ASTNode> retVal{ new ASTNode{ ASTNode::HORNCLAUSE } };
	string prod = "hornclause -> LEFTPAREN head [body] RIGHTPAREN";
	match(Token::LEFTPAREN, prod);
	if (it != tokens.end() && (**it).type == Token::LEFTPAREN) {
		retVal->children.push_back(head());
		recovery = currentParse;
		if (it != tokens.end() && (**it).type == Token::LEFTPAREN) {
			retVal->children.push_back(body());
		}
	}
	else {
		error(Token::getTokenName(Token::LABEL)
			, ((it == tokens.end()) ? Token::END : (**it).type)
			, prod);
	}
	match(Token::RIGHTPAREN, prod);
	return retVal;
}

shared_ptr<ASTNode> Parser::head() {
	shared_ptr<ASTNode> retVal{ new ASTNode{ ASTNode::HEAD } };
	if (it != tokens.end() && (**it).type == Token::LEFTPAREN) {
		retVal->children.push_back(predicate());
		while (it != tokens.end() && (**it).type == Token::AND) {
			recovery = currentParse;
			match(Token::AND, "head -> predicate");
			retVal->children.push_back(predicate());
		}
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (**it).type)
		, "head -> predicate");
	return retVal;
}

shared_ptr<ASTNode> Parser::body() {
	shared_ptr<ASTNode> retVal{ new ASTNode{ ASTNode::BODY } };
	// was: body -> predicate {AND predicate}
	string prod = "body -> LEFTPAREN predicate {predicate} RIGHTPAREN";
	match(Token::LEFTPAREN, prod);
	if (it != tokens.end() && (**it).type == Token::LEFTPAREN) {
		retVal->children.push_back(predicate());
		while (it != tokens.end() && (**it).type == Token::LEFTPAREN) {
			retVal->children.push_back(predicate());
		}
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (**it).type)
		, prod);
	match(Token::RIGHTPAREN, prod);
	return retVal;
}

shared_ptr<ASTNode> Parser::predicate() {
	shared_ptr<ASTNode> retVal{ new ASTNode{ ASTNode::PREDICATE } };
	// was: predicate -> name LEFTPAREN [args] RIGHTPAREN
	string prod = "predicate -> LEFTPAREN name [args] RIGHTPAREN"; // modified, but essentially same
	match(Token::LEFTPAREN, prod);
	if (it != tokens.end() && (**it).type == Token::LABEL) {
		retVal->children.push_back(name());
		if ((**it).type == Token::LABEL || (**it).type == Token::NUMBER) {
			auto argSymbs= move(args());
			retVal->children.reserve(argSymbs.size());
			retVal->children.insert(retVal->children.end(), argSymbs.begin(), argSymbs.end());
		}
		match(Token::RIGHTPAREN, prod);
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (**it).type)
		, "predicate -> name LEFTPAREN [args] RIGHTPAREN");
	if (!symtab.exists(retVal))
		symtab.add(retVal);
	else cout << "identical predicate found, not adding to the symbol table" << endl;
	return retVal;
}


shared_ptr<ASTNode> Parser::name() {
	// if match fails later, name throws, retVal will not get returned.
	shared_ptr<ASTNode> retVal{ new ASTNode(**it)};
	match(Token::LABEL, "name -> LABEL");
	symtab.add(retVal);
	return retVal;
}

vector<shared_ptr<ASTNode>> Parser::args() {
	vector<shared_ptr<ASTNode>> retVect;
	if (it != tokens.end() && ((**it).type == Token::LABEL || (**it).type == Token::NUMBER)) {
		retVect.push_back(symbol());
		while ((**it).type == Token::LABEL || (**it).type == Token::NUMBER) {
			// match(Token::COMMA, "args -> symbol {COMMA symbol}");
			retVect.push_back(symbol());
		}
	}
	else error(Token::getTokenName(Token::LABEL) + " or " + Token::getTokenName(Token::NUMBER)
		, ((it == tokens.end()) ? Token::END : (**it).type)
		, "args -> symbol {COMMA symbol}");
	return move(retVect);
}

// declare then define with make_shared doesn't really work. I wndr y
shared_ptr<ASTNode> Parser::symbol() {
	if (it != tokens.end() && (**it).type == Token::LABEL)
	{
		shared_ptr<ASTNode> retVal(new ASTNode(**it));
		match(Token::LABEL, "symbol->LABEL | NUMBER");
		symtab.add(retVal);
		return retVal;
	}
	else if (it != tokens.end() && (**it).type == Token::NUMBER)
	{
		shared_ptr<ASTNode> retVal(new ASTNode(**it));
		match(Token::NUMBER, "symbol->LABEL | NUMBER");
		symtab.add(retVal);
		return retVal;
	}
	else {
		error(Token::getTokenName(Token::LABEL) + " or "
			+ Token::getTokenName(Token::NUMBER)
			, ((it == tokens.end()) ? Token::END : (**it).type)
			, "symbol->LABEL | NUMBER");
		// return empty, as it's supposed to pop stack in error()
		return shared_ptr<ASTNode>();
	}
}