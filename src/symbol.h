
struct Symbol {
	// Symbol is a BST
	struct Symbol *left;
	struct Symbol *right;
	void *symbol;
};

struct Symbol *searchOrInsert(struct Symbol *table, char *symbol);
void printSymbolTable(struct Symbol *table);