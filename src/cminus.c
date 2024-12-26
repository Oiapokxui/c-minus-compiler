#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "cminus.h" /* Declares functions to be used by Lexer */
#include "data/symbol.h"

#define	END_OF_ARRAY ('\0')
#define EXTRA_BUFFER_CAPACITY 64

/** VARIABLES **/

static struct State* state;

/** FUNCTIONS **/
struct State *initState() {
	struct State *state = malloc(sizeof(struct State));
	state->symbolTable = createSymbolTable();
	state->tokens = NULL;
	// file has at least one line
	state->currentLine = 1;
	state->errors = 0;
	return state;
}

struct State *getState() {
	return state;
}

void setState(struct State *newState) {
	state = newState;
}

struct SymbolTable *enterNewScope(struct State *state) {

	if (state == NULL || state->symbolTable == NULL) {
		return NULL;
	}
	struct SymbolTable *outerScope = state->symbolTable;
	struct SymbolTable *innerScope = createSymbolTable();
	if (innerScope == NULL) {
		return NULL;
	}
	innerScope->previous = outerScope;
	state->symbolTable = innerScope;
	return innerScope;
}


struct SymbolTable *exitCurrentScope(struct State *state) {

	if (state == NULL || state->symbolTable == NULL || state->symbolTable->previous == NULL) {
		return NULL;
	}
	struct SymbolTable *currentScope = state->symbolTable;
	state->symbolTable = currentScope->previous;
	return currentScope;
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

void insertIntoState(void *lexeme, int type, struct State *state) {
	if (state == NULL) {
		return;
	}
	struct TokenList * tokens = insertToken(state->tokens, lexeme, type, state->currentLine);
	if (state->tokens == NULL) {
		state->tokens = tokens;
	}
	if(state->symbolTable == NULL) {
		//state->symbolTable = symbol;
	}
}

void insertAsText(char *text, int length, int type, struct State *state) {
	char* lexeme = strndup(text, length);
	insertIntoState(lexeme, type, state);
}

void insertAsInt(char *text, int length, int type, struct State *state) {
	char* lexeme = strndup(text, length);
	// Right now, I'm not checking for errors with this conversion
	const long int num = strtol(lexeme, NULL, 10);
	long int *numPtr = malloc(sizeof(long int));
	*numPtr = num;

	insertIntoState(numPtr, type, state);
}

void insertLineBreak(struct State *state) {
	char *lexeme = "\n";
	increaseCurrentLine(state, 1);
	insertAsText(lexeme, 1, 1000, state);
}

void insertWhitespace(char *text, int length, struct State *state) {
	int count = countLineBreaks(strndup(text, length));
	increaseCurrentLine(state, count);
	insertAsText(text, length, 1000, state);
}

char *skipComment(char *startComment, int startCommentLength, int input(), struct State *state) {
	if (state == NULL) {
		return NULL;
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
				insertAsText(comment, length, -1, state);
				return comment; /* found the end */
			}
		}

		if ( c == EOF || c == END_OF_ARRAY ) {
			insertAsText(comment, length, -1, state);
			return comment;
		}
	}
}
