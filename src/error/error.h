#include "../cminus.h"

// All of those should call the macro YYABORT
void genericError(char *message, struct State *state);
void symbolAlreadyDeclared(char * id, struct State *state);
void variableCreationFailedError(char * id, struct State *state);
void variableTypeIsInvalidError(char * type, char *id, struct State *state);
