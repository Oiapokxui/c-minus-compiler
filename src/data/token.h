#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */

struct TokenList {
	struct Token *first;
	struct Token *last;
};

enum TokenType {
	ID, NUM, SUMOP, MULTOP, RELOP, SPECIAL, TYPE_INT, TYPE_VOID, TYPEDEF, IF, ELSE, WHILE, RETURN, RESERVED, WHITESPACE, COMMENT, ERROR
};

struct Token {
	void *lexeme;
	enum TokenType type;
	int line;
	// Does it make sense to have attributes for tokens like relational operations?
	struct Symbol *attribute;
	struct Token *next;
};

struct TokenList *insertToken(
	struct TokenList *list,
	void *lexeme,
	enum TokenType type,
	int currentLine,
	struct Symbol *symbol
);
void printTokens(struct TokenList *list);