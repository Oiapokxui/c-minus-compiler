#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "cminus.h" /* Declares functions to be used by Lexer */
#include "symbol.h" /* Declares functions on Symbol structure */

#define	END_OF_ARRAY ('\0')
#define EXTRA_BUFFER_CAPACITY 64

struct State *initState() {
	struct State *state = malloc(sizeof(struct State));
	state->symbolTable = NULL;
	state->tokens = NULL;
	// file has at least one line
	state->currentLine = 1;
	return state;
}

char *appendChar(char* buffer, int *length, int *capacity, char c) {
	if (*length + 1 >= *capacity && buffer != NULL) {
		*capacity *= 2;
		buffer = realloc(buffer, *capacity * sizeof(char));
	}
	if (buffer == NULL) {
		exit(1);
	}
	buffer[*length] = c;
	(*length)++;
	buffer[*length] = '\0'; // Make sure string ends in '\0'
	return buffer;
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

void skipComment(char *startComment, int startCommentLength, int input(), struct State *state) {
	if (state == NULL) {
		return;
	}
	// Initializing comment string
	int initialCapacityMultiplier = (startCommentLength / EXTRA_BUFFER_CAPACITY ) + 1;
	int capacity = initialCapacityMultiplier * EXTRA_BUFFER_CAPACITY;
	char *comment = malloc(capacity * sizeof(char));
	memcpy(comment, startComment, startCommentLength);
	comment[startCommentLength] = '\0';

	int length = startCommentLength;
	char c = '\0';
	for (; ; ) {
		do {
			c = input();
			comment = appendChar(comment, &length, &capacity, c);
			if ( c == '\n' ) {
				increaseCurrentLine(state, 1);
			}
		}
		while ( c != '*' && c != EOF && c != END_OF_ARRAY);    /* eat up text of comment */

		if ( c == '*' ) {
			do {
				c = input();
				comment = appendChar(comment, &length, &capacity, c);
			}
			while ( c == '*');
			if ( c == '/' ) {
				insertAsText(comment, length, COMMENT, state);
				break; /* found the end */
			}
		}

		if ( c == EOF || c == END_OF_ARRAY ) {
			insertAsText(comment, length, ERROR, state);
			break;
		}
	}
}
