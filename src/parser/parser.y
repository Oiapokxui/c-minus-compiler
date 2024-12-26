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

%type <text> function_declaration var_declaration type_spec
%type <expression> expr var factor call term simple_expr sum_expr
%type <symbol> param declaration
%type <symbolArray> params params_list program declaration_list
%type <expressionArray> args arg_list
%type <statementArray> statement_list
%type <statement> compound_statement statement
%type <stringArray> local_declarations

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
program :
    declaration_list {
      validateProgramHasMainMethod(&$1, getState());
    }
declaration_list :
    declaration_list declaration {
        insertToSymbolArray(&($1), $2);
        $$ = $1;
    }
    | declaration {
        struct SymbolArray *newArray = malloc(sizeof(struct SymbolArray));
        $$ = *newArray;
    }
    | error {
        fprintf(
            stderr,
            "L%i: Programa nao comeca com uma definicao valida de variaveis ou funcoes.\n",
            getState()->currentLine
        );
		YYERROR;
    };
declaration :
    var_declaration {
        $$ = getSymbol($1, getState()->symbolTable)->value;
    }
    | function_declaration {
        $$ = getSymbol($1, getState()->symbolTable)->value;
    }
;
function_declaration :
    type_spec ID '(' { enterNewScope(getState()); }  params ')' compound_statement {
        struct State *state = getState();
        struct SymbolTable *functionScope = exitCurrentScope(state);
        int arity = $5.length;
        validateSymbolNotExistsInCurrentScope($2, state);
        createFunction($1, $2, arity, &($5.data), functionScope, state);
        $$ = $2;
    }
    | type_spec error '(' params ')' compound_statement {
        fprintf(
            stderr,
            "L%i: Declaracao de funcao deve possuir um identificador valido",
            getState()->currentLine
        );
		YYERROR;
    }
;
var_declaration :
    type_spec ID ';' {
        $$ = $2;
        struct State *state = getState();
        validateIntTypeSpec($1, $2, state);
        validateSymbolNotExistsInCurrentScope($2, state);
        createVariable($2, state);
    }
    | type_spec error ';' {
        fprintf(
            stderr,
            "L%i declaracao de variavel deve ter um identificador valido",
            getState()->currentLine
        );
		YYERROR;
    }
    | type_spec ID error {
        fprintf(
            stderr,
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
		YYERROR;
    }
	| type_spec ID '[' NUM ']' ';' {
        $$ = $2;
        struct State *state = getState();
        validateIntTypeSpec($1, $2, state);
        validateSymbolNotExistsInCurrentScope($2, state);
        createArrayVariable($2, $4, state);
	}
    | type_spec error '[' NUM ']' ';' {
		fprintf(
		    stderr,
		    "L%i declaracao de variavel array deve ter um identificador valido\n",
		    getState()->currentLine
		);
		YYERROR;
	}
    | error ID '[' NUM ']' ';' {
		fprintf(
		    stderr,
		    "L%i: declaracao da variavel `%s` deve ser precedida pelo tipo INT\n",
		    getState()->currentLine,
		    $2
        );
		YYERROR;
	}
    | type_spec ID '[' error ']' ';' {
		fprintf(
		    stderr,
		    "L%i: declaracao da variavel array `%s` deve ter comprimento em inteiros\n",
		    getState()->currentLine,
		    $2
		);
		YYERROR;
	}
    | type_spec ID '[' NUM ']' error {
        fprintf(
            stderr,
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
		YYERROR;
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
        createVariable($2, getState());
        $$ = (struct Symbol) { .type = VARIABLE, .it = { .variable = { .name = $2 } } };
    }
    | type_spec ID '[' ']' {
        validateIntTypeSpec($1, $2, getState());
        createArrayVariable($2, "-1", getState());
        $$ = (struct Symbol) { .type = ARRAY_VARIABLE, .it = { .array = { .name = $2 } } };
    }
    | type_spec error {
        fprintf(
            stderr,
            "L%i: Parametro de funcao deve conter um identificador valido\n",
            getState()->currentLine
        );
        YYERROR;
    }
    | type_spec error '[' ']' {
        fprintf(
            stderr,
            "L%i: Parametro array de funcao deve conter um identificador valido\n",
            getState()->currentLine
        );
        YYERROR;
    }
;
compound_statement:
    '{' local_declarations statement_list '}' {
        $$ = (struct Statement) {
            .type = STMNT_COMPOUND,
            .it = {
                .compound = {
                    .declarations = $2.data,
                    .statements = &$3,
                    .currentScope = getState()->symbolTable
                }
            }
        };
    }
;
local_declarations :
    var_declaration local_declarations {
        insertToStringArray(&$2, $1);
        $$ = $2;
    }
    | EMPTY {
        struct StringArray *newArray = malloc(sizeof(struct StringArray));
        $$ = *newArray;
    }
;
statement_list :
    statement_list statement {
        insertToStatementArray(&$1, $2);
        $$ = $1;
    }
    | EMPTY {
        struct StatementArray *newArray = malloc(sizeof(struct StatementArray));
        $$ = *newArray;
    }
;
statement :
    expr_statement {
        $$ = (struct Statement) { .type = STMNT_EXPRESSION };
    }
    | { enterNewScope(getState()); } compound_statement {
        struct SymbolTable *functionScope = exitCurrentScope(getState());
        $$ = (struct Statement) { .type = STMNT_COMPOUND };
    }
    | selection_statement {
        $$ = (struct Statement) { .type = STMNT_BRANCHING };
    }
    | itr_statement {
        $$ = (struct Statement) { .type = STMNT_ITERATION };
    }
    | return_statement {
        $$ = (struct Statement) { .type = STMNT_RETURN };
    }
;
expr_statement :
    expr ';'
    | expr error {
        fprintf(
            stderr,
            "L%i: Token `%s` inesperado. Esperava `;`.\n",
            getState()->currentLine,
            yylval.text
        );
        YYERROR;
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
		YYERROR;
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
		YYERROR;
        $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1.text };
    }
    | simple_expr {
        $$ = $1;
    };
var :
    ID {
        struct State *state = getState();
        validateSymbolExistsInAnyScope($1, state);
        validateNotFunctionSymbol($1, state);
        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL) {
            $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1 };
        }
        else if (entry->value.type == ARRAY_VARIABLE) {
            $$ = (struct Expression) { .returnType = EXPR_INT_ARRAY, .text = $1 };
        }
        else {
            $$ = (struct Expression) { .returnType = EXPR_INT, .text = $1 };
        }
    }
    | ID '[' expr ']' {
        struct State *state = getState();
        validateSymbolExistsInAnyScope($1, state);
        validateNotFunctionSymbol($1, state);
        validateIntegerArraySymbol($1, state);
        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL) {
            $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1 };
        }
        $$ = (struct Expression) { .returnType = EXPR_INT, .text = $1 } ;
    }
    | ID '[' error ']' {
        fprintf(
            stderr,
            "L%i: Indice da variavel array `%s` deve ser uma expressao valida\n",
            getState()->currentLine,
            $1
        );
		YYERROR;
        $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1 };
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
		YYERROR;
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
        $$ = (struct Expression) { .returnType = EXPR_INT, .text = $1 };
    }
    | '(' error ')' {
        fprintf(
            stderr,
            "L%i: Expressao invalida entre parenteses.\n",
            getState()->currentLine
        );
		YYERROR;
    }
;
call :
    ID '(' args ')' {
        struct State *state = getState();
        validateSymbolExistsInAnyScope($1, state);
        validateArgsArity($1, $3.length, state);

        struct TableEntry *entry = getSymbol($1, state->symbolTable);
        if (entry == NULL
            || entry->value.type != FUNCTION
        ) {
            $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1 };
        }
        else if (entry->value.it.function.returns == RET_VOID) {
            $$ = (struct Expression) { .returnType = EXPR_VOID, .text = $1 };
        }
        else {
            $$ = (struct Expression) { .returnType = EXPR_INT, .text = $1 };
        }
    }
    | ID '(' error ')' {
        fprintf(
            stderr,
            "L%i: Ativacao da funcao `%s` nao possui argumentos vlaidos\n",
            getState()->currentLine,
            $1
        );
		YYERROR;
        $$ = (struct Expression) { .returnType = EXPR_ERROR, .text = $1 };
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
	// As funcoes `input()` e `output()` sao predefinidas na linguagem
	// TODO: Fornecer impl padrao para essas funcoes
    struct Symbol *outputParams = &((struct Symbol) { .type = VARIABLE, .it = { .variable = { .name = "x" } } });
    createFunction("void", "output", 1, outputParams, NULL, getState());
    createFunction("int", "input", 0, NULL, NULL, getState());
    int result = yyparse();
    if (getState()->errors > 0) return -1;
}
