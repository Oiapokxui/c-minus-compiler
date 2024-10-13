
#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "cminus.h" /* Declares functions to be used by Lexer */

struct Symbol * allocSymbol() {
	struct Symbol * ptr = malloc(sizeof(struct Symbol));
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->symbol = NULL;
	return ptr;
}

struct Token * allocToken() {
	struct Token * ptr = malloc(sizeof(struct Token));
	ptr->name = NULL;
	ptr->lexeme = NULL;
	ptr->line = -1;
	ptr->attribute = NULL;
	ptr->next = NULL;
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
	struct Symbol *new = allocSymbol();
	new->symbol = symbol;
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

void printSymbol(struct Symbol *symbol) {
	if (symbol == NULL) {
		printf("()");
		return;
	}
	printf("( %s (%p) (%p)", (char *) symbol->symbol, symbol->left, symbol->right);
}

void insertToken(struct Token *last, struct Token *next) {
	last->next = next;
}

void insertFirst(struct Symbol *symbol, struct Token *token) {
	symbolTable = symbol;	
	firstToken = token;	
	lastToken = token;
}

char *copyLexeme(char *from, int lexemeLength) {
	int length = lexemeLength + 1;
	char *lexeme = malloc( length * sizeof(char));
	// This should copy the string and NULL terminate it
	strlcpy(lexeme, from, length);
	return lexeme;
}

void insertGeneric(void *lexeme, char *tokenName, int currentLine, bool isFirstMatch) {
	printf("Insert generic is called %s %s %i %i\n", (char *) lexeme, tokenName, currentLine, isFirstMatch);
	struct Symbol * symbol = searchOrInsert(symbolTable, lexeme);

	struct Token * token = allocToken();
	token->lexeme = lexeme;
	token->name = tokenName;
	token->line = currentLine;
	token->attribute = symbol;
	
	if (isFirstMatch) {
		insertFirst(symbol, token);
		return;
	} 

	printSymbolTable(symbolTable);
	insertToken(lastToken, token);
	lastToken = token;
}

void insertId(char *text, int length, int currentLine, bool isFirstMatch) {
	char* lexeme = copyLexeme(text, length);	
	insertGeneric(lexeme, "ID", currentLine, isFirstMatch);
}

void insertNum(char *text, int length, int currentLine, bool isFirstMatch) {
	char* lexeme = copyLexeme(text, length);	
	// Right now, I'm not checking for errors with this conversion
	long int num = strtol(lexeme, NULL, 10);
	long int *numPtr = malloc(sizeof(long int));

	insertGeneric(numPtr, "NUM", currentLine, isFirstMatch);
}

void insertSpecial(char *text, int length, int currentLine, bool isFirstMatch) {
	char* lexeme = copyLexeme(text, length);	
	insertGeneric(lexeme, lexeme, currentLine, isFirstMatch);
}

int skipComment(int input(), int currentLine) {
	int c = input();

	for ( ; ; ) {
		do { 
			c = input(); 
			if ( c == '\n' ) {
				currentLine++;
			}
		}
		while ( c != '*' && c != EOF );    /* eat up text of comment */

		if ( c == '*' ) {
			do { c = input(); }
			while ( c == '*');    
			if ( c == '/' ) break; /* found the end */
		}

		if ( c == EOF ) {
			// create error token here
			break;
		}
		c = input();
	}
	return currentLine;
}
