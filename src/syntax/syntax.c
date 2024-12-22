#include <string.h>
#include "syntax.h"
#include "../error/error.h"  /* Declares functions that stops parsing. */

void createVariable(char *id, struct State *state) {
	printf("\nCriando variavel: %s\n", id);
	if (state == NULL || state->symbolTable == NULL) {
		genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL) {
		symbolAlreadyDeclared(id, state);
	}
	struct TableEntry *createdEntry = createVariableSymbol(id, state->symbolTable);
	if (createdEntry == NULL) {
		variableCreationFailedError(id, state);
	}
}

void createArrayVariable(char *id, int length, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL) {
		symbolAlreadyDeclared(id, state);
	}

	// Make sure array creation won't fail if index == 0
	struct TableEntry *createdEntry = createArraySymbol(id, length, state->symbolTable);
	if (createdEntry == NULL) {
		variableCreationFailedError(id, state);
	}
}
