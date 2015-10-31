/*
Table.cpp: This is the declaration file for Table class. It declares
a few enum types for symbol table entries, as well as symbol table class
http://www.cse.wustl.edu/~cdgill/courses/cse425/lab2

Likai Yan
Jianfeng Zeng
*/

#pragma once
#include "ASTNode.h"
#include <memory>
#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <map>

using namespace std;

struct symbolEntry {
	enum symbolType {
		PREDICATE,
		VAR,
		CONST,
		UNKNOWNSYM,
	};

	string name;
	shared_ptr<ASTNode> address;
	symbolType type;

	symbolEntry(shared_ptr<ASTNode> addr);

	unsigned numberVal();

	string varName();
	// Value could be found at their address, hence not stored here.
};

class Table {
public:
	vector<symbolEntry> symbolTable;

	// add one entry
	void add(shared_ptr<ASTNode> addr);

	void match();

	// print the predicate
	string print();

	bool exists(symbolEntry pred);

private:
	bool predicateMatch(symbolEntry current, symbolEntry next, vector<shared_ptr<ASTNode>>& unifiedChildren);

};