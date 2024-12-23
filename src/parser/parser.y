%{
#include <stdio.h> // Declares functions to be used by Lexer
#include <stdbool.h> // Declares boolean type
#include <string.h>
#include "parser/parser.h"
#include "syntax/syntax.h"

int yylex(); // nao compila sem essa linha

%}

// WIll allow to define custom union type in a separate file
%define api.value.type { union YYSTYPE }

%type <text> var_declaration type_spec
%type <expression> expr var factor call term simple_expr sum_expr
%type <symbol> param
%type <symbolArray> params params_list
%type <expressionArray> args arg_list

%token <text> ID
%token <text> NUM
%token <text> IF
%token <text> ELSE
%token <text> WHILE
%token <text> RETURN
%token <text> SUMOP
%token <text> MULTOP
%token <text> RELOP
%token <text> TYPE_VOID
%token <text> TYPE_INT
%token <text> COMMENT
%token <text> SPECIAL

%%
// grammar
program :
    declaration_list
declaration_list :
    declaration_list declaration
    | declaration
    | error {
        fprintf(
            stderr,
            "L%i: Programa nao comeca com uma definicao valida de variaveis ou funcoes.\n",
            getState()->currentLine
        );
    };
declaration : var_declaration | function_declaration ;
function_declaration :
    type_spec ID '(' params ')' compound_statement {
        int arity = $4.length;
        struct Symbol (*data)[256] = &($4.data);
        createFunction($1, $2, arity, data, getState());
    }
    | type_spec error '(' params ')' compound_statement {
        fprintf(
            stderr,
            "L%i: Declaracao de funcao deve possuir um identificador valido",
            getState()->currentLine
        );
    }
;
var_declaration :
    type_spec ID ';' {
        $$ = $1;
        struct State *state = getState();
        validateIntTypeSpec($1, $2, state);
        createVariable($2, state);
    }
    | type_spec error ';' {
        fprintf(
            stderr,
            "L%i declaracao de variavel deve ter um identificador valido",
            getState()->currentLine
        );
    }
    | type_spec ID error {
        fprintf(
            stderr,
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
    }
	| type_spec ID '[' NUM ']' ';' {
        $$ = $1;
        struct State *state = getState();
        validateIntTypeSpec($1, $2, state);
        createArrayVariable($2, $4, state);
	}
    | type_spec error '[' NUM ']' ';' {
		fprintf(
		    stderr,
		    "L%i declaracao de variavel array deve ter um identificador valido\n",
		    getState()->currentLine
		);
	}
    | error ID '[' NUM ']' ';' {
		fprintf(
		    stderr,
		    "L%i: declaracao da variavel `%s` deve ser precedida pelo tipo INT\n",
		    getState()->currentLine,
		    $2
        );
		YYABORT;
	}
    | type_spec ID '[' error ']' ';' {
		fprintf(
		    stderr,
		    "L%i: declaracao da variavel array `%s` deve ter comprimento em inteiros\n",
		    getState()->currentLine,
		    $2
		);
	}
    | type_spec ID '[' NUM ']' error {
        fprintf(
            stderr,
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
    }
;
type_spec : TYPE_VOID | TYPE_INT
params :
    params_list {
        $$ = $1;
    }
    | TYPE_VOID {
        $$ = (struct SymbolArray) { .length = 0 };
    };
params_list :
    params_list ',' param {
        // TODO: Array is not carrying from one match to another
        insertToSymbolArray(&($1), $3);
        $$ = $1;
    }
    | param {
        // Inserir parametro em uma lista
        struct SymbolArray * newArray = malloc(sizeof(struct SymbolArray));
        insertToSymbolArray(newArray, $1);
        $$ = *newArray;
    }
;
param :
    type_spec ID {
        validateIntTypeSpec($1, $2, getState());
        // TOOD: IMPLEMENTAR STACK de contextos e inserir variavel no contexto certo
        $$ = (struct Symbol) { .type = VARIABLE, .it = { .variable = { .name = $2 } } };
    }
    | type_spec ID '[' ']' {
        validateIntTypeSpec($1, $2, getState());
        // TOOD: IMPLEMENTAR STACK de contextos e inserir variavel no contexto certo
        $$ = (struct Symbol) { .type = ARRAY_VARIABLE, .it = { .array = { .name = $2 } } };
    }
    | type_spec error {
        fprintf(
            stderr,
            "L%i: Parametro de funcao deve conter um identificador valido\n",
            getState()->currentLine
        );
    }
    | type_spec error '[' ']' {
        fprintf(
            stderr,
            "L%i: Parametro array de funcao deve conter um identificador valido\n",
            getState()->currentLine
        );
    }
;
compound_statement:
    '{' local_declarations statement_list '}'
;
local_declarations :
    var_declaration local_declarations
    | EMPTY ;
statement_list :
    statement_list statement
    | EMPTY
;
// TODO find a better name for *_declr
statement : expr_statement
          | compound_statement
          | selection_statement
          | itr_statement
          | return_statement ;
expr_statement :
    expr ';'
    | expr error {
        fprintf(
            stderr,
            "L%i: Token `%s` inesperado. Esperava `;`.\n",
            getState()->currentLine,
            yylval.text
        );
    }
    | ';'
;
selection_statement :
    IF '(' expr ')' statement
    | IF '(' expr ')' statement ELSE statement
;
itr_statement :
    WHILE '(' expr ')' statement
;
return_statement :
    RETURN ';'
    | RETURN expr ';'
    | RETURN error {
        fprintf(
            stderr,
            "L%i: Statement de retorno deve conter expressao valida ou vazia\n",
            getState()->currentLine
        );
    }
;
expr :
    var '=' expr {
        $$ = $3;
    }
    | var '=' error {
        fprintf(
            stderr,
            "L%i: Variavel `%s` deve ser atribuida de uma expressao valida\n",
            getState()->currentLine,
            $1.text
        );
        $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1.text };
    }
    | simple_expr {
        $$ = $1;
    };
var :
    ID {
        struct State *state = getState();
        validateSymbolExistence($1, state);
        validateNotFunctionSymbol($1, state);
        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL) {
            $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1 };
        }
        else if (entry->value.type == ARRAY_VARIABLE) {
            $$ = (struct Expression) { .type = EXPR_INT_ARRAY, .text = $1 };
        }
        else {
            $$ = (struct Expression) { .type = EXPR_INT, .text = $1 };
        }
    }
    | ID '[' expr ']' {
        struct State *state = getState();
        validateSymbolExistence($1, state);
        validateNotFunctionSymbol($1, state);
        validateIntegerArraySymbol($1, state);
        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL) {
            $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1 };
        }
        $$ = (struct Expression) { .type = EXPR_INT, .text = $1 } ;
    }
    | ID '[' error ']' {
        fprintf(
            stderr,
            "L%i: Indice da variavel array `%s` deve ser uma expressao valida\n",
            getState()->currentLine,
            $1
        );
        $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1 };
    };
simple_expr :
    sum_expr RELOP sum_expr {
        validateIntReturnedFrom($3, $2, getState());
    }
    | sum_expr
;
sum_expr :
    sum_expr SUMOP term {
        validateIntReturnedFrom($3, $2, getState());
    }
    | term
;
term :
    term MULTOP factor {
        validateIntReturnedFrom($3, $2, getState());
    }
    | factor
    | factor error {
        fprintf(
            stderr,
            "L%i: Operacao `%s` nao reconhecida.\n",
            getState()->currentLine,
            yylval.text
        );
    }
;
factor :
    '(' expr ')' {
        $$ = $2;
    }
    | var {
        $$ = $1;
    }
    | call {
        $$ = $1;
    }
    | NUM {
        $$ = (struct Expression) { .type = EXPR_INT, .text = $1 };
    }
    | '(' error ')' {
        fprintf(
            stderr,
            "L%i: Expressao invalida entre parenteses.\n",
            getState()->currentLine
        );
    }
;
call :
    ID '(' args ')' {
        struct State *state = getState();
        validateSymbolExistence($1, state);
        validateArgsArity($1, $3.length, state);

        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL
            || entry->value.type != FUNCTION
        ) {
            $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1 };
        }
        else if (entry->value.it.function.returns == RET_VOID) {
            $$ = (struct Expression) { .type = EXPR_VOID, .text = $1 };
        }
        else {
            $$ = (struct Expression) { .type = EXPR_INT, .text = $1 };
        }
    }
    | ID '(' error ')' {
        fprintf(
            stderr,
            "L%i: Ativacao da funcao `%s` nao possui argumentos vlaidos\n",
            getState()->currentLine,
            $1
        );
        $$ = (struct Expression) { .type = EXPR_ERROR, .text = $1 };
    }
;
args :
    arg_list {
        $$ = $1;
    }
    | EMPTY {
        $$ = (struct ExpressionArray) { .length = 0 };
    }
;
arg_list :
    arg_list ',' expr {
        insertToExpressionArray(&($1), $3);
        $$ = $1;
    }
    | expr {
        struct ExpressionArray *newArray = malloc(sizeof(struct ExpressionArray));
        insertToExpressionArray(newArray, $1);
        $$ = *newArray;
    }
;
EMPTY: /* empty */ ;
     
%%
void yyerror(const char* msg) {
	struct State* s = getState();
	// printf("\nERRO L%i, Token: [%s]: %s\n", s->currentLine, s->tokens->last->lexeme, msg);
	// printTokens(s->tokens);
	s->errors++;
}

int main(void) {
	extern int yydebug;
	yydebug = 0; // Mudar para 1 se quiser detalhes para debug do parser

	setState(initState());
    int result = yyparse();
	if (getState()->errors > 0) return -1;
}
