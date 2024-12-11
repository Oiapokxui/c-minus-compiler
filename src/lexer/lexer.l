/*+++ a lexer for c- +++*/

/* Will disable wrapping at the end of file */
%option noyywrap
/* Will make `yytext` return a char pointer */
%pointer
%{
#include "cminus.h" // Declares functions to be used by Lexer
#include "token.h" // Declares Tokens and TokenTypes
#include <stdbool.h> // Declares boolean type

struct State *lexerState;
%}

digito [0-9]
letra [a-zA-Z]
ID {letra}+
NUM {digito}+
SPACE [ ]
TAB [\t]
NEW_LINE [\n]
WHITESPACE ({SPACE}|{TAB}|{NEW_LINE})+
BEGIN_COMMENT \/\*
END_COMMENT \*\/
GT >
LT <
GE >=
LE <=
EQ ==
NE !=
SUMOP [+-]
MULTOP [\*]
RELOP {GT}|{LT}|{GE}|{LE}|{EQ}|{NE}
SPECIAL_CHARS [/;,()[\]{}=]
IF if
ELSE else
WHILE while
RETURN return
TYPE_VOID void
TYPE_INT int

%%

{WHITESPACE} { /* return WHITESPACE; */ }
{RELOP} { return RELOP; }
{MULTOP} { return MULTOP;  }
{SPECIAL_CHARS} { return SPECIAL; }
{IF} { return IF; }
{ELSE} { return ELSE; }
{WHILE} { return WHILE; }
{RETURN} { return RETURN; }
{TYPE_INT} { return TYPEDEF: }
{TYPE_VOID} { return TYPEDEF: }
{BEGIN_COMMENT}  { return COMMENT; }
{ID} { return ID; }
{NUM} { return NUM; }
. { return ERROR; }

%%

void printStateData(struct State *lexerState) {
    printf("*****************\nLista de tokens:\n");
	printTokens(lexerState->tokens);
    printf("\n*****************\n");
    printf("*****************\nTabela de simbolos:\n");
    printSymbolTable(lexerState->symbolTable);
    printf("\n*****************\n");
	printf("\n");
}

int main(int argCount, char *argVector[]) {
    lexerState = initState();
	yylex();
    printStateData(lexerState);
}
