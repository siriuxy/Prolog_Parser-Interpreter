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
				if (it != tokens.end())
					it++;
				error(Token::getTokenName(Token::LABEL)
					, ((currentIt == tokens.end()) ? Token::END : (*currentIt).type)
					, "hornclause->head[SEPARATOR body]");
			}
			hornclause();
			ofile << currentParse <<" "<< endl;
		}
		catch (exception& e){
			if (!recovery.empty()) ofile << recovery << " "<< endl;
			//system("color 1");
			cout <<endl<<"ERROR!"<<" At: \"";

			// print as much information as possible, without go out of range in token vector
			auto endOfError = (it == tokens.end()) ? tokens.end() : it + 1;
			// increment iterator = end() is UNDEFINED! 
			for (auto i = errorStart; i != endOfError; i++){
				/// cout << "current t mem loc: " << &(*i) << endl;
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
		currentParse += ((*it).value + " ");
		it++;
	}
	else {
		//recovery = currentParse;
		throw runtime_error(": Attempted production " + currentProduction 
			+ " , but failed to match " + Token::getTokenName((*it).type) + "(" + (*it).value + ")"
			+ " with " + "expected " + Token::getTokenName(t) 			);
	}
}


void Parser::error(string expected, Token::TokenType received, string currentProduction){
	currentParse.clear();
	throw runtime_error(": In production " + currentProduction 
		+ " but failed to match  " + Token::getTokenName(received) 
		+ "  with expected: " + expected	);
}

void Parser::hornclause(){
	if (it != tokens.end() && (*it).type == Token::LABEL){
		// for extra credit:
		head();
		// rational: 
		// hornclause -> body [SEPARATOR body]
		// body -> predicate {AND predicate}
		// whenever first body throws, this horn clause will only have one body
		// body();
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
	
	// extra credit:
	// difference from body(): it modifies (global) recovery!
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
		while (it != tokens.end() && (*it).type == Token::AND){
			recovery = currentParse;
			match(Token::AND, "body->predicate{ AND predicate }");
			predicate();
		}
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "body->predicate{ AND predicate }");
	/*
	if (it != tokens.end() && (*it).type == Token::LABEL){
		predicate();
	}
	else error(Token::getTokenName(Token::LABEL)
		, ((it == tokens.end()) ? Token::END : (*it).type)
		, "head -> predicate");
		*/
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