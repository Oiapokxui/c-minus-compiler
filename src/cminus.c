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
	// file has at least one line
	state->currentLine = 1;
	return state;
}

char *copyLexeme(char *from, int lexemeLength) {
	int length = lexemeLength + 1;
	char *lexeme = malloc( length * sizeof(char));
	// This should copy the string and NULL terminate it
	strlcpy(lexeme, from, length);
	return lexeme;
}

int countLineBreaks(const char *sequence) {
	if (sequence == NULL) {
		return 0;
	}
	int count = -1;
	sequence--;
	do {
		count++;
		sequence++;
		sequence = strchr(sequence, '\n');
	} while (sequence != NULL && *sequence != END_OF_ARRAY );
	return count;
}

void increaseCurrentLine(struct State *state, int count) {
	if (state == NULL) {
		return;
	}
	state->currentLine += count;
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

void insertLineBreak(struct State *state) {
	char *lexeme = "\n";
	increaseCurrentLine(state, 1);
	insertAsText(lexeme, 1, WHITESPACE, state);
}

void insertWhitespace(char *text, int length, struct State *state) {
	int count = countLineBreaks(copyLexeme(text, length));
	increaseCurrentLine(state, count);
	insertAsText(text, length, WHITESPACE, state);
}

void skipComment(int input(), struct State *state) {
	if (state == NULL) {
		return;
	}
	for (int c ; ; c = input()) {
		do { 
			c = input(); 
			if ( c == '\n' ) {
				increaseCurrentLine(state, 1);
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
