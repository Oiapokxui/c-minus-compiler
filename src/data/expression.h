#pragma once

enum ExpressionType {
	EXPR_INT, EXPR_VOID, EXPR_INT_ARRAY, EXPR_ERROR
};

struct Expression {
	enum ExpressionType type;
	char *text;
};

char *expressionTypeToString(enum ExpressionType type);