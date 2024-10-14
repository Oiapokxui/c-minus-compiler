#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */

struct TokenList {
	struct Token *first;
	struct Token *last;
};

enum TokenType {
	ID, NUM, RELOP, SPECIAL, TYPEDEF, RESERVED, WHITESPACE, ERROR
};

struct Token {
	void *lexeme;
	enum TokenType type;
	int line;
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