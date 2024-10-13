
#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "cminus.h" /* Declares functions to be used by Lexer */
#include "symbol.h" /* Declares functions on Symbol structure */
#include "token.h" /* Declares functions on Symbol structure */

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
