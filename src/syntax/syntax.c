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
		return symbolCreationFailedError(id, state);
	}
}

void createArrayVariable(char *id, char *lengthString, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}
    int length = atoi(lengthString);
    if (length < 1) {
		return genericError("Error generico: nao foi possivel converter inteiro representado em string para inteiro", state);
    }

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL) {
		return symbolAlreadyDeclared(id, state);
	}

	// TODO: Make sure array creation won't fail if index == 0
	struct TableEntry *createdEntry = createArraySymbol(id, length, state->symbolTable);
	if (createdEntry == NULL) {
		return symbolCreationFailedError(id, state);
	}
}

void createFunction(char *type, char *id, int arity, struct Symbol (*params)[256], struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);

	if (existingEntry != NULL) {
		return symbolAlreadyDeclared(id, state);
	}

	if (type == NULL) {
		return functionReturnTypeIsInvalid(type, id, state);
	}

	struct TableEntry *createdEntry = createFunctionSymbol(type, id, arity, *params, state->symbolTable);
	if (createdEntry == NULL) {
		return symbolCreationFailedError(id, state);
	}
}

void validateIntTypeSpec(char *type, char *id, struct State *state) {
	if (strcmp("int", type) != 0) variableTypeIsInvalidError(type, id, state);
}

void validateSymbolExistence(char *id, struct State *state) {
	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry == NULL) symbolUsageBeforeDeclarationError(id, state);
}

void validateIntegerArraySymbol(char *id, struct State *state) {
	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL && existingEntry->value.type != ARRAY_VARIABLE) return symbolHasNotIntegerArrayType(id, state);
}

void validateNotFunctionSymbol(char *id, struct State *state) {
	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);
	if (existingEntry != NULL && existingEntry->value.type == FUNCTION) return functionNameWithoutCall(id, state);
}

void validateArgsArity(char *id, int argsLength, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *existingEntry = getSymbol(id, state->symbolTable);

	if (existingEntry == NULL) {
		return;
	}

	if (existingEntry->value.type != FUNCTION) {
		return symbolTypeMismatchedError(id, existingEntry->value.type, state);
	}

	int expectedArity = existingEntry->value.it.function.arity;
	if (expectedArity != argsLength) {

		functionArityMismatchedError(id, argsLength, expectedArity, state);
	}
}

void validateIntReturnedFrom(struct Expression expression, char *contextOperation, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}
	if (expression.type != EXPR_INT) {
		return intExpressionTypeExpectedError(expression.text, expression.type, contextOperation, state);
	}
}

enum LIST_ERRORS insertToSymbolArray(struct SymbolArray *list, struct Symbol symbol) {
    int nextIndex = list->length;
	if (nextIndex < 0 || nextIndex >= 256) return LIST_INDEX_OUT_OF_RANGE;
    list->data[nextIndex] = symbol;
    list->length++;
	return OK;
}

enum LIST_ERRORS insertToExpressionArray(struct ExpressionArray *list, struct Expression expression) {
    int nextIndex = list->length;
	if (nextIndex < 0 || nextIndex >= 256) return LIST_INDEX_OUT_OF_RANGE;
    list->data[nextIndex] = expression;
    list->length++;
	return OK;
}
