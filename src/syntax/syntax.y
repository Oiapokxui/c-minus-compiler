%{
#include <stdio.h> // Declares functions to be used by Lexer
#include <stdbool.h> // Declares boolean type
#include "cminus.h" // Declares functions to be used by Lexer

int yylex(); // nao compila sem essa linha

%}

%union {
	char *text;
	int num;
}

%type <text> var expr sum_expr var_declaration type_spec simple_expr term factor call

%token <text> ID
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

%token <num> NUM
%%
// grammar
program :
    declaration_list
declaration_list :
    declaration_list declaration
    | declaration
    | error {
        printf(
            "L%i: Programa nao comeca com uma definicao valida de variaveis ou funcoes.\n",
            getState()->currentLine
        );
    };
declaration : var_declaration | function_declaration ;
function_declaration :
    type_spec ID '(' params ')' compound_statement
    | type_spec error '(' params ')' compound_statement {
        printf("L%i: Declaracao de funcao deve possuir um identificador valido", getState()->currentLine);
    }
;
// TODO: Verificar que variavel nao tem tipo void
var_declaration :
    type_spec ID ';' {
        struct State *state = getState();
        //validateVariableType($1, $2, state);
        //createVariable($2, state);
    }
    | type_spec error ';' {
        printf("L%i declaracao de variavel deve ter um identificador valido", getState()->currentLine);
    }
    | type_spec ID error {
        printf(
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
    }
	| type_spec ID '[' NUM ']' ';' {
        struct State *state = getState();
        //validateVariableType($1, $2, state);
        //createArrayVariable($2, $4, state);
	}
    | type_spec error '[' NUM ']' ';' {
		printf("L%i declaracao de variavel array deve ter um identificador valido\n", getState()->currentLine);
	}
    | error ID '[' NUM ']' ';' {
		printf("L%i declaracao da variavel `%s` deve ser precedida pelo tipo INT\n", getState()->currentLine, $2);
	}
    | type_spec ID '[' error ']' ';' {
		printf(
		    "L%i declaracao da variavel array `%s` deve ter comprimento em inteiros",
		    getState()->currentLine,
		    $1
		);
	}
    | type_spec ID '[' NUM ']' error {
        printf(
            "L%i: token `%s` inesperado apos declaracao da variavel `%s`. Esperava o token `;`\n",
            getState()->currentLine,
            yylval.text,
            $2
        );
    }
;
type_spec : TYPE_VOID | TYPE_INT
params :
    params_list
    | TYPE_VOID ;
params_list : params_list ',' param | param ;
param :
    type_spec ID
    | type_spec ID '[' ']'
    | type_spec error {
        printf("L%i: Parametro de funcao deve conter um identificador valido\n", getState()->currentLine);
    }
    | type_spec error '[' ']' {
        printf("L%i: Parametro array de funcao deve conter um identificador valido\n", getState()->currentLine);
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
        printf("L%i: Token `%s` inesperado. Esperava `;`.\n", getState()->currentLine, yylval.text);
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
        printf("L%i: Statement de retorno deve conter expressao valida ou vazia\n", getState()->currentLine);
    }
;
expr :
    var '=' expr
    | var '=' error {
        printf("L%i: Variavel `%s` deve ser atribuida de uma expressao valida\n", getState()->currentLine, $1);
    }
    | simple_expr ;
var :
    ID {
        $$ = $1;
    }
    | ID '[' expr ']' {
        $$ = $1;
    }
    | ID '[' error ']' {
        printf("L%i: Indice da variavel array `%s` deve ser uma expressao valida\n", getState()->currentLine, $1);
        $$ = $1;
    };
simple_expr :
    sum_expr RELOP sum_expr
    | sum_expr
;
sum_expr :
    sum_expr SUMOP term
    | term
;
term :
    term MULTOP factor
    | factor
    | factor error {
        printf("L%i: Operacao `%s` nao reconhecida.\n", getState()->currentLine, yylval.text);
    }
;
factor :
    '(' expr ')'
    | var
    | call
    | NUM
    | '(' error ')' {
        printf("L%i: Expressao invalida entre parenteses.\n", getState()->currentLine);
    }
;
call :
    ID '(' args ')'
    | ID '(' error ')' {
        printf(
            "L%i: Ativacao da funcao `%s` nao possui argumentos vlaidos\n",
            getState()->currentLine,
            $1
        );
    }
;
args : arg_list | EMPTY ; 
arg_list :
    arg_list ',' expr
    | expr ;
EMPTY: /* empty */ ;
     
%% 

int main(void) {
	extern int yydebug;
	yydebug = 1;

	setState(initState());
    int result = yyparse();
}

