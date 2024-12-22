//
// Created by Enrique Emanuel Silva on 21/12/24.
//

#include <stdio.h>
#include "error.h"

void genericError(char *message, struct State *state) {
  fprintf(stderr, "L%i: Erro inesperado! %s\n", state->currentLine, message);
  YYABORT;
}

void symbolAlreadyDeclared(char * id, struct State *state) {
  fprintf(stderr, "L%i: Simbolo %s ja foi definido anteriormente.\n", state->currentLine, id);
  YYABORT;
}

void variableCreationFailedError(char * id, struct State *state) {
  fprintf(stderr, "Erro ao inserir `%s` na tabela de simbolos\n", id);
  YYABORT;
};


void variableTypeIsInvalidError(char * type, char *id, struct State *state) {
  fprintf(
      stderr,
      "L%i: Tipo `%s` inesperado para a variavel `%s`. Tipos aceitos: int \n",
      state->currentLine,
      type,
      id
  );
  YYABORT;
};
