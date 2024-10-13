#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include "token.h" /* Declares token structure */
#include "symbol.h" /* Declares token structure */

struct Token * allocToken(void *lexeme, char *tokenName, int currentLine, struct Symbol *symbol) {
	struct Token * ptr = malloc(sizeof(struct Token));
	ptr->name = tokenName;
	ptr->lexeme = lexeme;
	ptr->line = currentLine;
	ptr->attribute = symbol;
	ptr->next = NULL;
	return ptr;
}

struct Token *insertToken(struct Token *last, void *lexeme, char *tokenName, int currentLine, struct Symbol *symbol) {
	struct Token *next = allocToken(lexeme, tokenName, currentLine, symbol);
	if (last != NULL) {
		last->next = next;
	} 
	return next;
}