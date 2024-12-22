#include "../cminus.h"

void genericError(char *message, struct State *state);
void symbolUsageBeforeDeclaration(char * id, struct State *state);
void symbolAlreadyDeclared(char * id, struct State *state);
void variableCreationFailedError(char * id, struct State *state);
void variableTypeIsInvalidError(char * type, char *id, struct State *state);
