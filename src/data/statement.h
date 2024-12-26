#pragma once

enum StatementType {
  STMNT_COMPOUND, STMNT_EXPRESSION, STMNT_ITERATION, STMNT_BRANCHING, STMNT_RETURN
};

struct CompoundStatement {
	char ** declarations;
	struct StatementArray *statements;
	struct SymbolTable *currentScope;
};

struct Statement {
	enum StatementType type;
	union {
		struct CompoundStatement compound;
	} it;
	char *text;
};