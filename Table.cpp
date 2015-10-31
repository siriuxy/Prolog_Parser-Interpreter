/*
Table.cpp: This is the declaration file for Table class. It defines
symbol table class
http://www.cse.wustl.edu/~cdgill/courses/cse425/lab2

Likai Yan
Jianfeng Zeng
*/
#include "stdafx.h"
#include "Table.h"

using namespace std;

symbolEntry::symbolEntry(shared_ptr<ASTNode> addr) {
	address = addr;
	type = UNKNOWNSYM;
	if (address->nType == ASTNode::ASTNodeType::PREDICATE) {
		type = symbolType::PREDICATE;
		name = address->getPredicateName();
	}
	else if (address->nType == ASTNode::ASTNodeType::TOKEN) {
		if (address->TokenNode->type == Token::TokenType::NUMBER)
			type = symbolType::CONST;
		else if (address->TokenNode->type == Token::TokenType::LABEL)
			type = symbolType::VAR;
		name = address->TokenNode->value;
	}
}

unsigned symbolEntry::numberVal() {
	assert(type == CONST);
	return stoul(name);
}

string symbolEntry::varName() {
	assert(type == VAR);
	return name;
}

// add one 
void Table::add(shared_ptr<ASTNode> addr) {
	symbolTable.push_back(symbolEntry(addr));
}

void Table::match() {
	for (auto p = symbolTable.begin(); p != symbolTable.end(); p++) {
		vector<shared_ptr<ASTNode>> unifiedChildren;
		bool matchStatus = false;
		if (p->type == symbolEntry::PREDICATE) {
			// find next predicate
			auto q = p + 1;
			while (q != symbolTable.end()) {
				if (q->type == symbolEntry::PREDICATE) {
					matchStatus = predicateMatch(*p, *q, unifiedChildren);
					if (matchStatus) {
						cout << "matched! " << endl;
						cout << "(";
						for (auto m : p->address->children)
							cout << m->TokenNode->value << " ";
						cout << ") AND (";
						for (auto n : q->address->children)
							cout << n->TokenNode->value << " ";
						cout << ") => (";
						for (auto s : unifiedChildren)
							cout << s->TokenNode->value << " ";
						cout << ")" << endl<<endl;
					}
					else {
						cout << "MISMATCH DETECTED while unifying: " << endl;
						cout << "(";
						for (auto m : p->address->children)
							cout << m->TokenNode->value << " ";
						cout << ") AND (";
						for (auto n : q->address->children)
							cout << n->TokenNode->value << " ";
						cout << ")" << endl<<endl;
					}
					break;
				}
				q++;
			}
		}

	}
}

// print the predicate
string Table::print() {
	string ret;
	for (auto p : symbolTable) {
		if (p.type == symbolEntry::symbolType::PREDICATE) {
			ret += p.address->print();
			ret += "\n";
		}
	}
	return ret;
}

bool Table::exists(symbolEntry pred) {
	for (auto p : symbolTable) {
		if (p.type == symbolEntry::PREDICATE) {
			if (p.address->children.size() == pred.address->children.size()) {
				auto currentIt = p.address->children.begin();
				auto nextIt = pred.address->children.begin();
				bool unifiable = true;
				while (currentIt != p.address->children.end()) {
					if ((*currentIt)->TokenNode->type == (*nextIt)->TokenNode->type) {
						if ((*currentIt)->TokenNode->value != (*nextIt)->TokenNode->value)
							return false;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Table::predicateMatch(symbolEntry current, symbolEntry next, vector<shared_ptr<ASTNode>>& unifiedChildren) {
	if (current.name != next.name) return false;
	if (current.address->children.size() == next.address->children.size())
	{
		// map for lookup
		map<string, string> lookupTable;
		auto currentIt = current.address->children.begin();
		auto nextIt = next.address->children.begin();
		bool unifiable = true;
		while (currentIt != current.address->children.end()) {
			if ((*currentIt)->TokenNode->type == (*nextIt)->TokenNode->type) {
				if ((*currentIt)->TokenNode->value == (*nextIt)->TokenNode->value) {
					unifiedChildren.push_back(*currentIt);
				}
				else if ((*currentIt)->TokenNode->type == Token::LABEL) {
					unifiedChildren.push_back(*currentIt);
					lookupTable[(*nextIt)->TokenNode->value] = (*currentIt)->TokenNode->value;
				}
				else unifiable = false;
			}
			else if ((*currentIt)->TokenNode->type == Token::LABEL
				&& (*nextIt)->TokenNode->type == Token::NUMBER) {
				if (lookupTable.find((*currentIt)->TokenNode->value) == lookupTable.end()) {
					unifiedChildren.push_back(*nextIt);
					lookupTable[(*currentIt)->TokenNode->value] = ((*nextIt)->TokenNode->value);
				}
				else {
					if (lookupTable[(*currentIt)->TokenNode->value] != ((*nextIt)->TokenNode->value))
						unifiable = false;
				}
			}
			else if ((*nextIt)->TokenNode->type == Token::LABEL
				&& (*currentIt)->TokenNode->type == Token::NUMBER) {
				if (lookupTable.find((*nextIt)->TokenNode->value) == lookupTable.end()) {
					unifiedChildren.push_back(*currentIt);
					lookupTable[(*nextIt)->TokenNode->value] = ((*currentIt)->TokenNode->value);
				}
				else {
					if (lookupTable[(*nextIt)->TokenNode->value] != ((*currentIt)->TokenNode->value))
						unifiable = false;
				}
			}
			else unifiable = false;
			if (unifiable == false) {
				cout << "Spotted! ";
				cout << "Variable/ Constant mismatch at " << (*currentIt)->TokenNode->value << " vs. "
					<< (*nextIt)->TokenNode->value << endl;
				break;
			}
			currentIt++;
			nextIt++;
		}
		return unifiable;
	}
	else return false;
}
