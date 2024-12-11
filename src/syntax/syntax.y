%{
#include <data/token.h> /* For getting the token types */
#include "cminus.h" // Declares functions to be used by Lexer
#include "token.h" // Declares Tokens and TokenTypes
#include <stdbool.h> // Declares boolean type

struct State *lexerState = initState() ;

%}

%token ID ID
%token NUM NUM
%token IF IF
%token ELSE ELSE
%token WHILE WHILE
%token RETURN RETURN
%token SUMOP SUMOP
%token MULTOP MULTOP
%token RELOP RELOP
%token VOID TYPE_VOID
%token INT TYPE_INT
%token TYPEDEF TYPEDEF

%%
// grammar
program : define_list ;
define_list : define_list define | define ;
define : function_define | var_define ;
var_define : INT ID ';' { /** TODO: inserir na tabela de simbolos. **/  } | INT ID '[' NUM ']';
function_define : TYPEDEF ID '(' params ')' compound_expr ;
params : params_list | VOID ;
params_list : params_list ',' param | param ;
param : INT ID | INT ID '[' ']' ;
compound_expr : '{' local_expr statement_list '}' ;
local_expr : local_expr var_exp | EMPTY ;
statement_list: statement_list statement | EMPTY ;
// TODO find a better name for *_declr
statement : expr_declr
          | compound_expr 
          | selection_declr 
          | itr_expr
          | return_declr ;
expr_declr : expr ';' | ';' ;
selection_declr : IF '(' expr ')' statement 
                | IF '(' expr ')' ELSE statement ;
itr_expr : WHILE '(' expr ')' statement ;
return_declr : RETURN ';' | RETURN expr ';' ;
expr : var '=' expr | simple_expr ;
var : ID | ID '[' expr ']' ; 
simple_expr : sum_expr RELOP sum_expr | sum_expr ; 
sum_expr : sum_expr SUMOP term | term ; 
term : term MULTOP factor | factor ; 
factor : '(' expr ')' | var | call | NUM ; 
call : ID '(' args ')' ; 
args : arg_list | EMPTY ; 
arg_list : arg_list ',' expr | expr ; 
EMPTY: /* empty */ ;
     
%% 