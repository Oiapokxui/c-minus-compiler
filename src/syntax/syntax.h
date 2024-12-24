#pragma once

#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include "../cminus.h"  /* Declare Symbol data structures and functions to operate them. */
#include "parser/parser.h"

#define ARRAY_CAPACITY 256

enum LIST_ERRORS {
  GENERIC, LIST_INDEX_OUT_OF_RANGE, OK
};

void createVariable(char *id, struct State *state);
void createArrayVariable(char *id, char *length, struct State *state);
void createFunction(char *type, char *id, int arity, struct Symbol (*params)[256], struct State *state);

void validateIntTypeSpec(char *type, char *id, struct State *state);
void validateSymbolExistence(char *id, struct State *state);
void validateNotFunctionSymbol(char *id, struct State *state);
void validateArgsArity(char *id, int argsLength, struct State *state);
void validateIntegerArraySymbol(char *id, struct State *state);
void validateIntReturnedFrom(struct Expression expression, char *contextOperation, struct State *state);

struct SingleVariableSymbol getVariable(char *id, struct State *state);

enum LIST_ERRORS insertToSymbolArray(struct SymbolArray *list, struct Symbol symbol);
enum LIST_ERRORS insertToExpressionArray(struct ExpressionArray *list, struct Expression expression);
enum LIST_ERRORS insertToStatementArray(struct StatementArray *list, struct Statement expression);
enum LIST_ERRORS insertToStringArray(struct StringArray *list, char *string);
