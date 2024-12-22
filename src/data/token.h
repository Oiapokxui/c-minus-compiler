#pragma once
#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */

struct TokenList {
	struct Token *first;
	struct Token *last;
};

struct Token {
	void *lexeme;
	int type;
	int line;
	// Does it make sense to have attributes for tokens like relational operations?
	struct TableEntry *attribute;
	struct Token *next;
};

struct TokenList *insertToken(
	struct TokenList *list,
	void *lexeme,
	int type,
	int currentLine
);

void printTokens(struct TokenList *list);
