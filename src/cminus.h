#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */

/** STRUCTURES **/

struct Token {
	char *name;
	void *lexeme;
	int line;
	struct Symbol *attribute;
	struct Token *next;
};

/** VARIABLES **/

struct Symbol * symbolTable;
struct Token * firstToken;
struct Token * lastToken;

/** FUNCTIONS **/

struct Token * allocToken();
void insertToken(struct Token *last, struct Token *next);
void insertId(char *text, int length, int currentLine, bool isFirstMatch);
void insertNum(char *text, int length, int currentLine, bool isFirstMatch);
void insertSpecial(char *text, int length, int currentLine, bool isFirstMatch);
int skipComment(int input(), int currentLine);
