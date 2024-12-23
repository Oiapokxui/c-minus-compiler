#pragma once
#include "data/token.h"
#include "data/symbol.h"
#include "data/expression.h"

struct State {
    struct SymbolTable *symbolTable;
    struct TokenList *tokens;
    int currentLine;
	int errors;
};

/** FUNCTIONS **/

struct State *initState();
struct State *getState();
void setState(struct State *newState);
void insertAsText(char *text, int length, int type, struct State *state);
void insertWhitespace(char *text, int length, struct State *state);
void yyerror(const char* msg);
char *skipComment(char* startComment, int startCommentLength, int input(), struct State *state);
