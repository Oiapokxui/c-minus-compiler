#include "../cminus.h"

void genericError(char *message, struct State *state);

void symbolUsageBeforeDeclarationError(char * id, struct State *state);
void symbolAlreadyDeclared(char * id, struct State *state);
void symbolCreationFailedError(char * id, struct State *state);
void symbolHasNotIntegerArrayType(char * id, struct State *state);
void symbolTypeMismatchedError(char * id, enum SymbolType type, struct State *state);
void variableTypeIsInvalidError(char * type, char *id, struct State *state);

void functionNameWithoutCall(char * id, struct State *state);
void functionReturnTypeIsInvalid(char * type, char *id, struct State *state);
void functionArityMismatchedError(char * id, int obtainedLength, int expectedLength, struct State *state);

void intExpressionTypeExpectedError(char * expr, enum ExpressionType type, char *contextOperation, struct State *state);
