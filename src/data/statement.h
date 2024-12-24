#pragma once

enum StatementType {
  STMNT_COMPOUND, STMNT_EXPRESSION, STMNT_ITERATION, STMNT_BRANCHING, STMNT_RETURN
};

struct Statement {
	enum StatementType type;
	union {
		struct {
			char ** declarations;
			struct StatementArray *statements;
		} compound;
	} it;
	char *text;
};