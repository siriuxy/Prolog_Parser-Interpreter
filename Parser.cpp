#include "stdafx.h"
#include "Parser.h"
#include <iostream>

// NOTE: add function information at MATCH() to save multiple try-catch/

Parser::Parser(ifstream& i, ofstream& o) :ofile(o){
	Scanner s(i);
	Token t;
	while (s >> t) tokens.push_back(t);
	it = tokens.begin();
	Parse();
}

void Parser::Parse(){
	while (it != tokens.end()){
		try {
			currentParse.clear();
			recovery.clear();
			errorStart = it;
			hornclause();
			ofile << currentParse << endl;
		}
		catch (exception& e){
			//system("color 1");
			cout << "ERROR! At: \"";
			for (auto i = errorStart; i != it; i++){
				cout << (*i).value <<" ";
			}
			cout << "\" "<< e.what() << endl;
			//ofile << recovery << endl;
				//cout << "currentParse: " << currentParse << endl;
		//	cout << "recovery content :"<<recovery << endl;
		}
	}
}

// match might throw ex, so use try and catch every time you use match();
void Parser::match(Token::TokenType t, string currentProduction) {
	if (it == tokens.end()) throw runtime_error("Reaches End");
	if ((*it).type == t) {
		currentParse += (*it).value;
		it++;
	}
	else {
		//recovery = currentParse;
		throw runtime_error(" : Failed to match expected: " + Token::getTokenName(t)
			+ " with " + "actual: " + Token::getTokenName((*it).type) + "(" + (*it).value + ")" + " for production: " + currentProduction);
	}
}
/*
void Parser::error(string production){
	// move pointer to the end?
	currentParse.clear();
	throw runtime_error(" in " + production);
}
*/

void Parser::error(string expected, Token::TokenType received, string production){
	//recovery = currentParse;
	currentParse.clear();
	throw runtime_error(": expect" +expected +" but received " +Token::getTokenName(received) );
}

void Parser::hornclause(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		head();
		recovery = currentParse; // because the following is optional
		
		// local try catch, because 2nd part is optional
		// optionally, return bool for each func as chained-stack
		// but ignore the return value for optional part
		try {
			if (it != tokens.end() && (*it).type == Token::SEPARATOR) {
				match(Token::SEPARATOR, "hornclause->head[SEPARATOR body]");
				body();
			}
		}
		catch (exception& e){
			cout << e.what();
			currentParse = recovery;
		}

	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it != tokens.end()) ? Token::END : (*it).type)
		, "hornclause->head[SEPARATOR body]");
}

void Parser::head(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it != tokens.end()) ? Token::END : (*it).type)
		, "head -> predicate");
}

void Parser::body(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
		string LocalRecovery;
		while (it != tokens.end() && (*it).type == Token::AND){
			try {
				LocalRecovery = currentParse;
				match(Token::AND, "body->predicate{ AND predicate }");
				predicate();
			}
			catch (exception& e){
				cout << e.what();
				currentParse = LocalRecovery;
			}
		}
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it != tokens.end()) ? Token::END : (*it).type)
		, "body->predicate{ AND predicate }");
}

void Parser::predicate(){
	//add case or if condition later
	if (it != tokens.end() && (*it).type == Token::LABEL){
		name();
			match(Token::LEFTPAREN, "predicate -> name LEFTPAREN [args] RIGHTPAREN");
			// this is not an END OPTIONAL
			// So if it has some problem, so does the production.
			if ((*it).type == Token::LABEL || (*it).type == Token::NUMBER){
				args();
			}// note: for {}, use while-loop. [] is optional and may appear only once.
			match(Token::RIGHTPAREN, "predicate->name LEFTPAREN[args] RIGHTPAREN");
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it != tokens.end()) ? Token::END : (*it).type)
		, "predicate -> name LEFTPAREN [args] RIGHTPAREN");
}

// review FOLLOW set idea for args()

void Parser::name(){
		match(Token::LABEL, "name -> LABEL");
}

void Parser::args(){
	if (it != tokens.end() && ((*it).type == Token::LABEL || (*it).type == Token::NUMBER)){
		symbol();
		string LocalRecovery;
		while ((*it).type == Token::COMMA){
			LocalRecovery = currentParse;
			try {
				match(Token::COMMA, "args -> symbol {COMMA symbol}");
				symbol();
			}
			catch (exception& e){
				cout << e.what();
				currentParse = LocalRecovery;
			}
		}
	}
	else error(Token::getTokenName(Token::LABEL) + " or " + Token::getTokenName(Token::NUMBER)
		, ((it != tokens.end()) ? Token::END : (*it).type)
		, "args -> symbol {COMMA symbol}");
}

void Parser::symbol(){
	// add case or if condition later
	if (it != tokens.end() && (*it).type == Token::LABEL)
			match(Token::LABEL, "symbol->LABEL | NUMBER");
	else if (it != tokens.end() && (*it).type == Token::NUMBER)
			match(Token::NUMBER, "symbol->LABEL | NUMBER");
		else error(Token::getTokenName(Token::LABEL) + " or " + Token::getTokenName(Token::NUMBER)
			, ((it != tokens.end()) ? Token::END : (*it).type)
			, "symbol->LABEL | NUMBER");
}