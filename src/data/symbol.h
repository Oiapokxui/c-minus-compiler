#pragma once

#define ST_INITIAL_CAPACITY 32
// Starting Offset of the FNV-1a hash
#define FNV_OFFSET 14695981039346656037UL
// Prime number
#define FNV_PRIME 1099511628211UL

enum ReturnType {
	RET_VOID, RET_INT
};

enum SymbolType {
	UNINITIALIZED = 0, // Or whichever number the compiler initializes an int.
	FUNCTION,
	VARIABLE,
	ARRAY_VARIABLE
};

struct FunctionSymbol {
	char *name;
	enum ReturnType returns;
	int arity;
	struct Symbol *params;
	struct SymbolTable *scope;
};

struct SingleVariableSymbol {
	char* name;
};

struct ArrayVariableSymbol {
	char* name;
	int size;
};

struct Symbol {
	enum SymbolType type;
	union {
		struct FunctionSymbol function;
		struct SingleVariableSymbol variable;
		struct ArrayVariableSymbol array;
	} it;
};

struct TableEntry {
	char *id;
	struct Symbol value;
};

// SymbolTable is a HashTable
// The current implementation is FNV-1a
struct SymbolTable {
	// Scope inside which this scope is defined
	struct SymbolTable *previous;
	// SymbolTable 1->n TableEntry 1->1 Symbol
	struct TableEntry ** entries;
	int length;
	int capacity;
};

struct TableEntry *createVariableSymbol(char *id, struct SymbolTable *table);
struct TableEntry *createArraySymbol(char *id, int size, struct SymbolTable *table);
struct TableEntry *createFunctionSymbol(char *type, char *id, struct SymbolTable *symbolTable);

struct SymbolTable *createSymbolTable(void) ;
struct TableEntry *getSymbol(char *id, struct SymbolTable *table);
struct TableEntry *insertSymbol(char *id, struct Symbol symbol, struct SymbolTable *table);

enum ReturnType stringToReturnType(char *string);
char *symbolTypeToString(enum SymbolType type);

void printSymbolTable(struct SymbolTable *table);