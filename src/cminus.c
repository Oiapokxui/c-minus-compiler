#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "cminus.h" /* Declares functions to be used by Lexer */
#include "symbol.h" /* Declares functions on Symbol structure */

#define	END_OF_ARRAY ('\0')

struct State *initState() {
	struct State *state = malloc(sizeof(struct State));
	state->symbolTable = NULL;
	state->tokens = NULL;
	state->currentLine = -1;
	return state;
}

char *copyLexeme(char *from, int lexemeLength) {
	int length = lexemeLength + 1;
	char *lexeme = malloc( length * sizeof(char));
	// This should copy the string and NULL terminate it
	strlcpy(lexeme, from, length);
	return lexeme;
}

void increaseCurrentLine(struct State *state) {
	if (state == NULL) {
		return;
	}
	if (state->currentLine == -1) {
		state->currentLine = 0;
	}
	state->currentLine++;
}

bool shouldInsertIntoSymbolTable(const enum TokenType type) {
	return type == ID;
}

void insertIntoState(char *lexeme, enum TokenType type, struct State *state) {
	if (state == NULL) {
		return;
	}
	struct Symbol * symbol = NULL;
	if (shouldInsertIntoSymbolTable(type)) {
		symbol = searchOrInsertSymbol(state->symbolTable, lexeme);
	}
	struct TokenList * tokens = insertToken(state->tokens, lexeme, type, state->currentLine, symbol);
	if (state->tokens == NULL) {
		state->tokens = tokens;
	}
	if(state->symbolTable == NULL && symbol != NULL) {
		state->symbolTable = symbol;
	}
}

void insertGeneric(void *lexeme, enum TokenType type, struct State *state) {
	if (state == NULL) {
		return;
	}
	insertIntoState(lexeme, type, state);
	printTokens(state->tokens);
	printSymbolTable(state->symbolTable);
	printf("\n");
}

void insertAsText(char *text, int length, enum TokenType type, struct State *state) {
	char* lexeme = copyLexeme(text, length);	
	insertGeneric(lexeme, type, state);
}

void insertAsInt(char *text, int length, enum TokenType type, struct State *state) {
	char* lexeme = copyLexeme(text, length);	
	// Right now, I'm not checking for errors with this conversion
	const long int num = strtol(lexeme, NULL, 10);
	long int *numPtr = malloc(sizeof(long int));
	*numPtr = num;

	insertGeneric(numPtr, type, state);
}

void insertWhitespace(struct State *state) {
	char *lexeme = "\n";
	increaseCurrentLine(state);
	insertAsText(lexeme, 1, WHITESPACE, state);
}

void skipComment(int input(), struct State *state) {
	if (state == NULL) {
		return;
	}
	for (int c ; ; c = input()) {
		do { 
			c = input(); 
			if ( c == '\n' ) {
				insertWhitespace(state);
			}
		}
		while ( c != '*' && c != EOF && c != END_OF_ARRAY);    /* eat up text of comment */

		if ( c == '*' ) {
			do { c = input(); }
			while ( c == '*');    
			if ( c == '/' ) break; /* found the end */
		}

		if ( c == EOF || c == END_OF_ARRAY ) {
			// create error token here
			break;
		}
	}
}
