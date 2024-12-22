#pragma once

#define ST_INITIAL_CAPACITY 32
// Starting Offset of the FNV-1a hash
#define FNV_OFFSET 14695981039346656037UL
// Prime number
#define FNV_PRIME 1099511628211UL

struct FunctionSymbol {};

struct SingleVariableSymbol {
	char* name;
	int value;
};
struct ArrayVariableSymbol {
	char* name;
	int size;
	int *value;
};

union Symbol {
	struct FunctionSymbol function;
	struct SingleVariableSymbol variable;
	struct ArrayVariableSymbol array;
};

struct TableEntry {
	char *id;
	int type;
	union Symbol value;
};

enum SymbolType {
	SINGLE, ARRAY, FUNCTION
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
struct TableEntry *createFunctionSymbol(struct FunctionSymbol *symbol);
struct TableEntry *createArraySymbol(char *id, int size, struct SymbolTable *table);
struct SymbolTable *createSymbolTable(void) ;
struct TableEntry *getSymbol(char *id, struct SymbolTable * table);
struct TableEntry *insertSymbol(char *id, enum SymbolType type, union Symbol symbol, struct SymbolTable *table);
void printSymbolTable(struct SymbolTable *table);
