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

void symbolUsageBeforeDeclarationError(char * id, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Simbolo `%s` esta sendo usado antes de sua declaracao.\n",
      state->currentLine,
      id
	);
	increaseErrors(state);
}

void symbolHasNotIntegerArrayType(char * id, struct State *state) {
  	fprintf(
      stderr,
      // TODO: melhorar essa msg de erro
      "L%i: Simbolo `%s` nao eh um array mas esta sendo acessado como um.\n",
      state->currentLine,
      id
	);
	increaseErrors(state);
}

void functionNameWithoutCall(char * id, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Simbolo `%s` eh um funcao mas nao eh sucedido por sua ativacao.\n",
      state->currentLine,
      id
	);
	increaseErrors(state);
}

void symbolAlreadyDeclared(char * id, struct State *state) {
  	fprintf(stderr, "L%i: Simbolo `%s` ja foi definido anteriormente.\n", state->currentLine, id);
	increaseErrors(state);
}

void symbolCreationFailedError(char * id, struct State *state) {
  	fprintf(stderr, "Erro ao inserir `%s` na tabela de simbolos\n", id);
	increaseErrors(state);
};

void variableTypeIsInvalidError(char * type, char *id, struct State *state) {
	fprintf(
		stderr,
	    "L%i: Tipo `%s` inesperado para a variavel `%s`. Tipos aceitos: `int`.\n",
	    state->currentLine,
	    type,
	    id
	);
	increaseErrors(state);
};

void functionReturnTypeIsInvalid(char * type, char *id, struct State *state) {
  	fprintf(
  		stderr,
  		"L%i: Tipo `%s` inesperado para a funcao `%s`. Tipos aceitos: `int` ou `void`.\n",
  		state->currentLine,
  		type,
  		id
	);
	increaseErrors(state);
}

void symbolTypeMismatchedError(char * id, enum SymbolType type, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Simbolo `%s` esta sendo chamado como funcao mas seu tipo é %s.\n",
      state->currentLine,
      id,
      symbolTypeToString(type)
	);
	increaseErrors(state);
}

void intExpressionReturnTypeExpectedError(char * expr, enum ExpressionReturnType type, char *contextOperation, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Expressao `%s` de tipo de retorno `%s` nao pode ser argumento da operacao `%s`.\n",
      state->currentLine,
      expr,
      expressionReturnTypeToString(type),
      contextOperation
	);
	increaseErrors(state);
}

void functionArityMismatchedError(char * id, int obtainedLength, int expectedLength, struct State *state) {
  	fprintf(
      stderr,
      "L%i: Chamada para `%s` contem aridade `%i` diferente da esperada `%i`.\n",
      state->currentLine,
      id,
      obtainedLength,
      expectedLength
	);
	increaseErrors(state);
}

void programWithoutEntrypointError(struct State *state) {
  	fprintf(
      stderr,
      "Programa nao possui metodo de entrada com assinatura `void main(void)`\n"
	);
	increaseErrors(state);
}