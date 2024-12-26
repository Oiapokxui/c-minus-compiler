#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdlib.h>  /* Declares malloc.  */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include <string.h>  /* Declares functions operating on strings.  */
#include "symbol.h" /* Declares functions to be used by Lexer */

#include <cminus.h>

enum ReturnType stringToReturnType(char *string) {
	if (string != NULL && strcmp(string, "int") != 0) return RET_VOID;
	return RET_INT;
}

char *symbolTypeToString(enum SymbolType type) {
	switch (type) {
		case FUNCTION: return "funcao";
		case VARIABLE: return "variavel";
		case ARRAY_VARIABLE: return "array";
		default: return "???";
	}
}

struct TableEntry * allocEntry(char * symbolPtr, struct Symbol value) {
	struct TableEntry * ptr = malloc(sizeof(struct TableEntry));
	ptr->id = symbolPtr;
	ptr->value = value;
	return ptr;
}

struct TableEntry *createVariableSymbol(char *name, struct SymbolTable *symbolTable) {
 	if (symbolTable == NULL) {
        return NULL;
 	}
	;
	struct Symbol symbol = (struct Symbol) {
		.type = VARIABLE,
		.it = {
			.variable = {
				.name = name,
			}
		}
	};
    return insertSymbol(name, symbol, symbolTable);
}

struct TableEntry *createArraySymbol(char *name, int size, struct SymbolTable *symbolTable) {
  	if (symbolTable == NULL) {
        return NULL;
  	}
	struct Symbol symbol = (struct Symbol) {
		.type = ARRAY_VARIABLE,
		.it = {
			.array = {
				.name = name,
				.size = size,
		}
	} };

    return insertSymbol(name, symbol, symbolTable);
}

struct TableEntry *createFunctionSymbol(char *type, char *id, struct SymbolTable *symbolTable) {
	if (symbolTable == NULL) {
        return NULL;
  	}

	enum ReturnType returnType = stringToReturnType(type);

	struct Symbol symbol = (struct Symbol) {
		.type = FUNCTION,
		.it = {
			.function = {
				.name = id,
				.returns = returnType
			}
		}
	};

    return insertSymbol(id, symbol, symbolTable);
}

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
uint64_t hash(char *key) {
	uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;

}

struct TableEntry *_probe(char *key, int index, int capacity, struct TableEntry **entries) {
	while (entries != NULL && entries[index] != NULL && entries[index]->id != NULL) {
        if (strcmp(key, entries[index]->id) == 0) {
            // Found key, return value.
            return entries[index];
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

struct TableEntry *_insert(struct TableEntry *symbol, int index, struct SymbolTable *table) {
	if (table == NULL || table->entries == NULL || symbol == NULL || symbol->id == NULL) {
		return NULL;
	}
	
	struct TableEntry **entries = table->entries;

    // Loop till we find an empty entry.
    while (entries[index]!= NULL) {
        if (strcmp(symbol->id, entries[index]->id) == 0) {
            // Found key (it already exists), dont do nothing.
            return NULL;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    // Didn't find existing key, inserting it.
	table->length++;
    entries[index] = symbol;
    return symbol;

}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool _expand(struct SymbolTable *table) {
    // Allocate new entries array.
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        return false;  // overflow (capacity would be too big)
    }
    struct TableEntry **new_entries = calloc(new_capacity, sizeof(struct TableEntry *));
    if (new_entries == NULL) {
        return false;
    }
	
	// Updating table details
    struct TableEntry **oldEntries = table->entries;
    int oldCapacity = table->capacity;
    table->entries = new_entries;
    table->capacity = new_capacity;
    table->length = 0;

    // Iterate entries, move all non-empty ones to new table's entries.
    for (size_t i = 0; i < oldCapacity; i++) {
        struct TableEntry * entry = oldEntries[i];
        if (entry->id != NULL) {
            _insert(entry, i, table);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    return true;
}

struct TableEntry *getSymbol(char *key, struct SymbolTable *table) {
	if (table == NULL || table->entries == NULL) {
		return NULL;
	}
    uint64_t id_hashed = hash(key);
    size_t index = id_hashed % (table->capacity - 1);

	return _probe(key, index, table->capacity, table->entries);
}

struct TableEntry *insertSymbol(char *id, struct Symbol symbol, struct SymbolTable *table) {
	if (table == NULL || table->entries == NULL) {
		return NULL;
	}

    // If length will exceed half of current capacity, expand it.
    if (table->length >= table->capacity / 2) {
        if (!_expand(table)) {
            return NULL;
        }
    } 

    uint64_t id_hashed = hash(id);
    size_t index = id_hashed % (table->capacity - 1);
	struct TableEntry * entry = allocEntry(id, symbol);

	return _insert(entry, index, table);
}

struct SymbolTable *createSymbolTable(void) {
    // Allocate space for hash table struct.
    struct SymbolTable* table = malloc(sizeof(struct SymbolTable));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->capacity = ST_INITIAL_CAPACITY;

    // Allocate (zero'd) space for entry buckets.
    table->entries = calloc(table->capacity, sizeof(struct TableEntry *));
    if (table->entries == NULL) {
        free(table); // error, free table before we return!
        return NULL;
    }
	table->previous = NULL;
    return table;
}

void print(struct TableEntry **entries, int index, int length) {
	if (index >= length) {
		return;
	}
	if (entries[index] != NULL) {
		printf("%d: %s, ", index, entries[index]->id);
	}
	print(entries, index + 1, length);
}

void printSymbolTable(struct SymbolTable *table) {
	if (table == NULL) {
		printf("");
		return;
	}
	print(table->entries, 0, table->capacity);
	bool initialized;
}
