#pragma once
#include "../data/symbol.h"
#include "../data/expression.h"
#include "../data/statement.h"

#define MAX_ARRAY_SIZE 256

struct SymbolArray {
	int length;
	struct Symbol data[MAX_ARRAY_SIZE];
};

struct ExpressionArray {
	int length;
	struct Expression data[MAX_ARRAY_SIZE];
};

struct StatementArray {
	int length;
	struct Statement data[MAX_ARRAY_SIZE];
};

struct StringArray {
	int length;
	char *data [MAX_ARRAY_SIZE];
};

typedef union YYSTYPE {
	char *text;
	int num;
	struct Symbol symbol;
	struct Expression expression;
	struct Statement statement;
	struct SymbolArray symbolArray;
	struct ExpressionArray expressionArray;
	struct StatementArray statementArray;
	struct StringArray stringArray;
} YYSTYPE;