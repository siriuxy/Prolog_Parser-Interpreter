/*
	ASTNode.cpp: This is the definition file for ASTNode class. It defines constructors
	and a recursive printer that conforms output grammar given in 
	http://www.cse.wustl.edu/~cdgill/courses/cse425/lab1/

	Likai Yan
	Jianfeng Zeng
*/
#include "stdafx.h"
#include "ASTNode.h"


// for error recovery, we need both directions.

string ASTNode::getPredicateName() {
	assert(nType == ASTNode::PREDICATE);
	return children[0]->TokenNode->value;
}

ASTNode::ASTNode(const ASTNodeType t) : nType(t)
{
}

ASTNode::ASTNode(Token t): nType(TOKEN), TokenNode(make_shared<Token>(t))
{
}



string ASTNode::print()
{
	// either print current token value (base), or 
	// recursively call it's children's print 
	if (TokenNode) return TokenNode->value;

	printSubtree.clear();

	if (nType == HORNCLAUSE) {
		printSubtree += "\n";
	}
	if (nType != TOKEN && nType != ROOT) {
		printSubtree += "( ";
	}
	for (auto&& p : children) {
			printSubtree = printSubtree
				+ p->print()
				+ " ";
	}
	if (nType != TOKEN && nType != ROOT) {
		printSubtree += ")";
	}

	return move(printSubtree);
}
