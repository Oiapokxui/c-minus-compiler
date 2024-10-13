#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include "token.h" /* Declares token structure */

struct Token * allocToken() {
	struct Token * ptr = malloc(sizeof(struct Token));
	ptr->name = NULL;
	ptr->lexeme = NULL;
	ptr->line = -1;
	ptr->attribute = NULL;
	ptr->next = NULL;
	return ptr;
}

void insertToken(struct Token *last, struct Token *next) {
	last->next = next;
}