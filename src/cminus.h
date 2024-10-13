#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */

/** VARIABLES **/

struct Symbol * symbolTable;
struct Token * firstToken;
struct Token * lastToken;

/** FUNCTIONS **/

void insertId(char *text, int length, int currentLine, bool isFirstMatch);
void insertNum(char *text, int length, int currentLine, bool isFirstMatch);
void insertSpecial(char *text, int length, int currentLine, bool isFirstMatch);
int skipComment(int input(), int currentLine);
