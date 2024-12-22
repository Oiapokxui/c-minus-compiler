#include <stdio.h>

#include "error.h"

void increaseErrors(struct State *state) {
	if (state == NULL) {
		return;
	}
	state->errors++;
}

void genericError(char *message, struct State *state) {
	fprintf(stderr, "L%i: Erro inesperado! %s\n", state->currentLine, message);
	increaseErrors(state);
}

void symbolUsageBeforeDeclaration(char * id, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Simbolo `%s` esta sendo usado antes de sua declaracao.\n",
      state->currentLine,
      id
	);
	increaseErrors(state);
}

void symbolAlreadyDeclared(char * id, struct State *state) {
  	fprintf(stderr, "L%i: Simbolo `%s` ja foi definido anteriormente.\n", state->currentLine, id);
	increaseErrors(state);
}

void variableCreationFailedError(char * id, struct State *state) {
  	fprintf(stderr, "Erro ao inserir `%s` na tabela de simbolos\n", id);
	increaseErrors(state);
};


void variableTypeIsInvalidError(char * type, char *id, struct State *state) {
	fprintf(
		stderr,
	    "L%i: Tipo `%s` inesperado para a variavel `%s`. Tipos aceitos: int\n",
	    state->currentLine,
	    type,
	    id
	);
	increaseErrors(state);
};
