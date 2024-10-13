#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */

struct Token {
	char *name;
	void *lexeme;
	int line;
	struct Symbol *attribute;
	struct Token *next;
};

struct Token *insertToken(struct Token *last, void *lexeme, char *tokenName, int currentLine, struct Symbol *symbol);