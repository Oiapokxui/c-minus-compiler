#include <string.h>
#include "syntax.h"
#include "../error/error.h"  /* Declares functions that stops parsing. */

void createVariable(char *id, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}

	struct TableEntry *createdEntry = createVariableSymbol(id, state->symbolTable);
	if (createdEntry == NULL) {
		return symbolCreationFailedError(id, state);
	}
}

void createArrayVariable(char *id, char *lengthString, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("estado do programa esta invalido", state);
	}

    int length = atoi(lengthString);
    if (length < -1) { // -1 is a length reserved for unknown array length. Other lenghts bigger than this one are valid.
		return genericError("nao foi possivel converter inteiro representado em string para inteiro", state);
    }

	// TODO: Make sure array creation won't fail if index == 0
	struct TableEntry *createdEntry = createArraySymbol(id, length, state->symbolTable);
	if (createdEntry == NULL) {
		return symbolCreationFailedError(id, state);
	}
}

void createFunction(char *type, char *id, int arity, struct Symbol *params, struct SymbolTable *functionScope, struct State *state) {
	if (state == NULL || state->symbolTable == NULL) {
		return genericError("Error generico: estado do programa esta invalido", state);
	}

	if (type == NULL) {
		return functionReturnTypeIsInvalid(type, id, state);
	}

	struct TableEntry *createdEntry = createFunctionSymbol(type, id, arity, params, functionScope, state->symbolTable);
	if (createdEntry == NULL) {
		return symbolCreationFailedError(id, state);
	}
}

void validateIntTypeSpec(char *type, char *id, struct State *state) {
	if (strcmp("int", type) != 0) variableTypeIsInvalidError(type, id, state);
}

void validateSymbolNotExistsInCurrentScope(char *id, struct State *state) {
	struct SymbolTable *current = state->symbolTable;
	struct TableEntry *existingEntry = getSymbol(id, current);
	if (existingEntry != NULL) {
		return symbolAlreadyDeclared(id, state);
	}
}

void validateSymbolExistsInAnyScope(char *id, struct State *state) {
	struct SymbolTable *current = state->symbolTable;
	while (current != NULL) {
		struct TableEntry *existingEntry = getSymbol(id, current);
		if (existingEntry != NULL) {
			return;
		}
		current = current->previous;
	}
	symbolUsageBeforeDeclarationError(id, state);
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
		return intExpressionReturnTypeExpectedError(expression.text, expression.returnType, contextOperation, state);
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

enum LIST_ERRORS insertToStatementArray(struct StatementArray *list, struct Statement statement) {
    int nextIndex = list->length;
	if (nextIndex < 0 || nextIndex >= 256) return LIST_INDEX_OUT_OF_RANGE;
    list->data[nextIndex] = statement;
    list->length++;
	return OK;
}

enum LIST_ERRORS insertToStringArray(struct StringArray *list, char *string) {
    int nextIndex = list->length;
	if (nextIndex < 0 || nextIndex >= 256) return LIST_INDEX_OUT_OF_RANGE;
    list->data[nextIndex] = strdup(string);
    list->length++;
	return OK;
}