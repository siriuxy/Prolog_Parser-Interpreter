#include <string>

using namespace std;
struct Token{
	enum TokenType{
		UNKOWN,
		NUMBER,
		LABEL,
		LEFTPAREN,
		RIGHTPAREN,
		COMMA,
		AND,
		SEPARATOR,
		END
	};
	TokenType type;
	string value;
	Token();
	static string getTokenName(TokenType t);
};