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
			// this for-loop credits to Christ Ogle
			if ((*it).type != Token::LABEL){
				auto currentIt = it;
				it++;
				error(Token::getTokenName(Token::LABEL)
					, ((currentIt == tokens.end()) ? Token::END : (*currentIt).type)
					, "hornclause->head[SEPARATOR body]");
			}
			hornclause();
			ofile << currentParse << endl;
		}
		catch (exception& e){
			currentParse = recovery;
			//system("color 1");
			cout <<endl<<"ERROR!"<<" At: \"";
			for (auto i = errorStart; i == it; i++){
				cout << (*i).value <<" ";
			}
			cout << "\" "<< e.what() << endl;
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
		throw runtime_error(": Attempted production " + currentProduction 
			+ " , but failed to match expected: " + Token::getTokenName(t)
			+ " with " + "actual: " + Token::getTokenName((*it).type) + "(" + (*it).value + ")" 
			);
	}
}


void Parser::error(string expected, Token::TokenType received, string production){
	currentParse.clear();
	throw runtime_error(": expect " +expected +" but received " +Token::getTokenName(received) );
}

void Parser::hornclause(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		head();
		recovery = currentParse;
			if (it != tokens.end() && (*it).type == Token::SEPARATOR) {
				match(Token::SEPARATOR, "hornclause->head[SEPARATOR body]");
				body();
			}
	}
	else {
		error(Token::getTokenName(Token::LABEL)
			, ((it == tokens.end()) ? Token::END : (*it).type)
			, "hornclause->head[SEPARATOR body]");
	}
}

void Parser::head(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "head -> predicate");
}

void Parser::body(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
		string LocalRecovery;
		while (it != tokens.end() && (*it).type == Token::AND){
				LocalRecovery = currentParse;
				match(Token::AND, "body->predicate{ AND predicate }");
				predicate();
		}
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "body->predicate{ AND predicate }");
}

void Parser::predicate(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		name();
			match(Token::LEFTPAREN, "predicate -> name LEFTPAREN [args] RIGHTPAREN");
			if ((*it).type == Token::LABEL || (*it).type == Token::NUMBER){
				args();
			}
			match(Token::RIGHTPAREN, "predicate->name LEFTPAREN[args] RIGHTPAREN");
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "predicate -> name LEFTPAREN [args] RIGHTPAREN");
}


void Parser::name(){
		match(Token::LABEL, "name -> LABEL");
}

void Parser::args(){
	if (it != tokens.end() && ((*it).type == Token::LABEL || (*it).type == Token::NUMBER)){
		symbol();
		string LocalRecovery;
		while ((*it).type == Token::COMMA){
			LocalRecovery = currentParse;
				match(Token::COMMA, "args -> symbol {COMMA symbol}");
				symbol();
		}
	}
	else error(Token::getTokenName(Token::LABEL) + " or " + Token::getTokenName(Token::NUMBER)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "args -> symbol {COMMA symbol}");
}

void Parser::symbol(){
	if (it != tokens.end() && (*it).type == Token::LABEL)
			match(Token::LABEL, "symbol->LABEL | NUMBER");
	else if (it != tokens.end() && (*it).type == Token::NUMBER)
			match(Token::NUMBER, "symbol->LABEL | NUMBER");
		else error(Token::getTokenName(Token::LABEL) + " or " 
			+ Token::getTokenName(Token::NUMBER)
			, ((it == tokens.end()) ? Token::END : (*it).type)
			, "symbol->LABEL | NUMBER");
}