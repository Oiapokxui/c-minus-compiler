#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdio.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include "token.h" /* Declares token structure */
#include "symbol.h" /* Declares token structure */

struct TokenList * allocTokenList(struct Token *first) {
	struct TokenList * ptr = malloc(sizeof(struct TokenList));
	ptr->first = first;
	ptr->last = first;
	return ptr;
}

struct Token * allocToken(void *lexeme, int type, int currentLine) {
	struct Token * ptr = malloc(sizeof(struct Token));
	ptr->lexeme = lexeme;
	ptr->line = currentLine;
	ptr->type = type;
	ptr->next = NULL;
	return ptr;
}

struct TokenList *insertToken(struct TokenList *list, void *lexeme, int type, int currentLine) {
	struct Token *next = allocToken(lexeme, type, currentLine);
	if (list == NULL) {
		return allocTokenList(next);
	}
	list->last->next = next;
	list->last = next;
	return list;
}

void printToken(struct Token *token) {
	if (token == NULL) {
		return;
	}
	printf("[");
	printf("%s", (char *) token->lexeme);
	printf(" line: %i type: %i] -> ", token->line, token->type);
	printToken(token->next);
}

void printTokens(struct TokenList *list) {
	if (list == NULL) {
		return;
	}
	printf("{");
	printToken(list->first);
	printf("}\n");
}