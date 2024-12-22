#include <string.h>
#include "syntax.h"
#include "../error/error.h"  /* Declares functions that stops parsing. */

void createVariable(char *id, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL) {
		return symbolAlreadyDeclared(id, state);
	}
	struct TableEntry *createdEntry = createVariableSymbol(id, state->symbolTable);
	if (createdEntry == NULL) {
		return variableCreationFailedError(id, state);
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

	// TODO: Make sure array creation won't fail if index == 0
	struct TableEntry *createdEntry = createArraySymbol(id, length, state->symbolTable);
	if (createdEntry == NULL) {
		variableCreationFailedError(id, state);
	}
}

void validateVariableType(char *type, char *id, struct State *state) {
	if (strncmp("int", type, 3) != 0) variableTypeIsInvalidError(type, id, state);
}

void validateSymbolExistence(char *id, struct State *state) {
	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry == NULL) symbolUsageBeforeDeclaration(id, state);
}
