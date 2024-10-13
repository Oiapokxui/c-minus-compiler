
#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "symbol.h" /* Declares functions to be used by Lexer */

struct Symbol * allocSymbol(void * symbolPtr) {
	struct Symbol * ptr = malloc(sizeof(struct Symbol));
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->symbol = symbolPtr;
	return ptr;
}

int compareSymbols(struct Symbol * current, struct Symbol * next) {
	if (current == NULL || next == NULL) {
		printf("current ou next nulos\n");
		return (current == NULL) - (next == NULL);
	}
	if (current->symbol == NULL || next->symbol == NULL) {
		printf("current ou next nulos\n");
		return (current->symbol == NULL) - (next->symbol == NULL);
	}
	return strcmp(current->symbol, next->symbol);
}

int compareStrings(char * a, char * b) {
	if (a == NULL || b == NULL) {
		printf("A ou B nulos\n");
		return (a == NULL) - (b == NULL);
	}
	return strcmp(a, b);
}

void insert(int comparison, struct Symbol * current, struct Symbol * next) {
	if (comparison <= 0) {
		current->right = next;
	}
	else {
		current->left = next;
	}
	return;
}

void insertSymbolTable(struct Symbol *current, struct Symbol *next) {
	if (current == NULL || next == NULL || current == next) {
		// Won't do anything if either of symbols are null or both symbols have the same address
		return;
	}
	printf("Insert symbol table is called %s %s\n", (char *) current->symbol, (char *) next->symbol);

	struct Symbol * nextCurrent = NULL;
	int comparison = compareSymbols(current, next);
	int inverseComparison = compareSymbols(next, current);

	if(comparison  == inverseComparison) {
		// symbols are equal and insertion is not needed
		nextCurrent = current->right;
		return;
	}

	if(comparison  <= 0 && current->right != NULL) {
		nextCurrent = current->right;
	}
	else if(comparison > 0 && current->left != NULL) {
		nextCurrent = current->left;
	}

	if (nextCurrent == NULL) {
		insert(comparison, current, next);
		return;
	}

	// TODO: Please tail call optimize this	
	insertSymbolTable(nextCurrent, next);
}

struct Symbol *searchSymbol(struct Symbol *current, char *symbol) {
	if (current == NULL) {
		return NULL;	
	}

	struct Symbol * nextCurrent = NULL;
	int comparison = compareStrings(current->symbol, symbol);
	int inverseComparison = compareStrings(symbol, current->symbol);

	if(comparison  == inverseComparison) {
		// symbols are equal, return this symbol
		return current;
	}
	if(comparison  <= 0 && current->right != NULL) {
		nextCurrent = current->right;
	}
	else if(comparison > 0 && current->left != NULL) {
		nextCurrent = current->left;
	}
	
	if (nextCurrent == NULL) {
		return NULL;
	}
	return searchSymbol(nextCurrent, symbol);
}

struct Symbol *searchOrInsert(struct Symbol *table, char *symbol) {
	struct Symbol *obtained = searchSymbol(table, symbol);
	if (obtained != NULL) {
		return obtained;
	}
	struct Symbol *new = allocSymbol(symbol);
	insertSymbolTable(table, new);
	return new;	
}

void printSymbolTable(struct Symbol *table) {
	if (table == NULL) {
		printf("()");
		return;
	}
	printf("( "/*, (char *) table->symbol*/);
	if (table->symbol == NULL) {
		printf("[] ");
	}
	else {
		printf("[%s] ", (char *) table->symbol);
	}

	printf("l");
	printSymbolTable(table->left);
	printf("l");
	printf(" r");
	printSymbolTable(table->right);
	printf("r");
	printf(" )");
}