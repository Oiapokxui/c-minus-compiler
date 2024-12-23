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
};

struct SingleVariableSymbol {
	char* name;
	int value;
};

struct ArrayVariableSymbol {
	char* name;
	int size;
	int *value;
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
	// Array of pointers to TableEntry
	struct TableEntry ** entries;
	int length;
	int capacity;	
};

struct TableEntry *createVariableSymbol(char *id, struct SymbolTable *table);
struct TableEntry *updateVariableSymbol(char* id, int value, struct SymbolTable *table);
struct TableEntry *createFunctionSymbol(char *type, char *id, int arity, struct Symbol *params, struct SymbolTable *table);
struct TableEntry *createArraySymbol(char *id, int size, struct SymbolTable *table);

struct SymbolTable *createSymbolTable(void) ;
struct TableEntry *getSymbol(char *id, struct SymbolTable * table);
struct TableEntry *insertSymbol(char *id, struct Symbol symbol, struct SymbolTable *table);

enum ReturnType stringToReturnType(char *string);
char *symbolTypeToString(enum SymbolType type);

void printSymbolTable(struct SymbolTable *table);