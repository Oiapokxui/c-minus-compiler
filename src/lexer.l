/*+++ a lexer for c- +++*/

/* Will disable wrapping at the end of file */
%option noyywrap
/* Will make `yytext` return a char pointer */
%pointer
%{
#include "cminus.h" // Declares functions to be used by Lexer
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
RELOP [+\-*]|{GT}|{LT}|{GE}|{LE}|{EQ}|{NE}
SPECIAL_CHARS [/;,()[\]{}=]
KEYWORDS if|else|while|return
TYPE_ASSIGN int|void

%%

{WHITESPACE} { insertWhitespace(yytext, yyleng, lexerState);  }
{RELOP} { insertAsText(yytext, yyleng, RELOP, lexerState);  }
{SPECIAL_CHARS} { insertAsText(yytext, yyleng, SPECIAL, lexerState);  }
{KEYWORDS} { insertAsText(yytext, yyleng, RESERVED, lexerState);  }
{TYPE_ASSIGN} { insertAsText(yytext, yyleng, TYPEDEF, lexerState);  }
{BEGIN_COMMENT}  { skipComment(input, lexerState); }
{ID} { insertAsText(yytext, yyleng, ID, lexerState); }
{NUM} { insertAsInt(yytext, yyleng, NUM, lexerState); }
. { insertAsText(yytext, yyleng, ERROR, lexerState); }
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
	/* Dunno if I'll need it
	++argv, --argc;  // skip over program name
	*/
    lexerState = initState();
	yylex();
}
