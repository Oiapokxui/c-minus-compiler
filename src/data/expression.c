#include "expression.h"

char *expressionTypeToString(enum ExpressionType type) {
	switch (type) {
		case EXPR_INT: return "int";
		case EXPR_VOID: return "void";
		case EXPR_INT_ARRAY: return "array";
		default: return "unknown";
	}
}
