/*
	ASTNode.h: This is the header file for ASTNode class. AST stands for 
	abstract syntax tree. By declaring a versatile node type, I can 
	build abstract syntax tree without worrying about different Token
	type and different production rules.

	Likai Yan
	Jianfeng Zeng
*/
#pragma once
#include<memory>
#include "Token.h"
#include <vector>
#include <set>
#include <sstream>
#include <cassert>
using namespace std;

/*
	print requirement:
	ancestor ( x , z ) :- parent ( x , y ) ^ ancestor ( y , z )
	age ( x , 78 )
	-->
	( ( ancestor x z ) ( ( parent x y ) ( ancestor y z ) ) )
	( ( age x 78 ) )


*/
struct ASTNode {
	enum ASTNodeType {
		ROOT,
		HORNCLAUSE,
		HEAD,
		BODY,
		PREDICATE,
		TOKEN
	};
	// maybe declare static? but in class initialization unavaiable
//	const set<ASTNodeType>	bracketNeededNode = { PREDICATE, HORNCLAUSE, HEAD, BODY };
	ASTNodeType nType;
	vector<shared_ptr<ASTNode>> children;
	// for error recovery, we need both directions.
	string getPredicateName();
	shared_ptr<ASTNode> parent;

	// only available for NAME, SYMBOL
	shared_ptr<Token> TokenNode;

	// this is infact the current node name
	string printSubtree;

	ASTNode(const ASTNodeType t);
	//ASTNode(const ASTNodeType t, const vector<shared_ptr<ASTNode>>& c);
	ASTNode(Token t);
	string print();
};