#pragma once
#include "../data/symbol.h"
#include "../data/expression.h"

#define MAX_ARITY 256

struct SymbolArray {
	int length;
	struct Symbol data[MAX_ARITY];
};

struct ExpressionArray {
	int length;
	struct Expression data[MAX_ARITY];
};

typedef union YYSTYPE {
	char *text;
	int num;
	struct Symbol symbol;
	struct Expression expression;
	struct SymbolArray symbolArray;
	struct ExpressionArray expressionArray;
} YYSTYPE;

struct SymbolArray newSymbolArray();
struct ExpressionArray newExpressionArray();