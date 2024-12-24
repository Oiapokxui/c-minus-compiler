#include "expression.h"

char *expressionReturnTypeToString(enum ExpressionReturnType type) {
	switch (type) {
		case EXPR_INT: return "int";
		case EXPR_VOID: return "void";
		case EXPR_INT_ARRAY: return "array";
		default: return "unknown";
	}
}
