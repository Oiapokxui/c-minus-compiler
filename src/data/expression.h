#pragma once
#include <stdbool.h>

enum ExpressionReturnType {
	EXPR_INT, EXPR_VOID, EXPR_INT_ARRAY, EXPR_ERROR
};

enum ExpressionType {
	EXPR_ASSIGNMENT, EXPR_INFIX_OPERATION, EXPR_VARIABLE, EXPR_NUMBER_VALUE, EXPR_CALL
};

enum ExpressionOperatorType {
	GE, GT, LT, EQ, LE, SUM, SUBTRACTION, MULTIPLICATION, DIVISION
};

struct AssignmentExpression {
	bool isArrayAssignment;
	int arrayIndex;
	char *varId;
	struct Expression *value;
};

struct VariableExpression {
	char *varId;
};

struct NumberValueExpression {
	int value;
};

struct CallExpression {
	int value;
};

struct InfixOperationExpression {
	enum ExpressionReturnType returnType;
	struct Expression *left;
	struct Expression *right;
};

struct Expression {
	enum ExpressionReturnType returnType;
	enum ExpressionType type;
	union {
		struct AssignmentExpression assignment;
		struct VariableExpression variable;
		struct InfixOperationExpression infixOperation;
		struct CallExpression call;
		struct NumberValueExpression numberValue;
	} it;
	char *text;
};

char *expressionReturnTypeToString(enum ExpressionReturnType returnType);