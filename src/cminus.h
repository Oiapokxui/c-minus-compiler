#include "data/token.h"
#include "data/symbol.h"

/** VARIABLES **/

struct State {
    struct Symbol *symbolTable;
    struct TokenList *tokens;
    int currentLine;
};

/** FUNCTIONS **/

struct State *initState();
void insertWhitespace(char *text, int length, struct State *state);
void insertAsText(char *text, int length, enum TokenType type, struct State *state);
void insertAsInt(char *text, int length, enum TokenType type, struct State *state);
void skipComment(char* startComment, int startCommentLength, int input(), struct State *state);
