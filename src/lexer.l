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

{NEW_LINE} { insertWhitespace(lexerState); }
{RELOP} { insertAsText(yytext, yyleng, RELOP, lexerState);  }
{SPECIAL_CHARS} { insertAsText(yytext, yyleng, SPECIAL, lexerState);  }
{KEYWORDS} { insertAsText(yytext, yyleng, RESERVED, lexerState);  }
{TYPE_ASSIGN} { insertAsText(yytext, yyleng, TYPEDEF, lexerState);  }
{SPACE}|{TAB} { insertAsText(yytext, yyleng, WHITESPACE, lexerState);  }
{BEGIN_COMMENT}  { skipComment(input, lexerState); }
{ID} { insertAsText(yytext, yyleng, ID, lexerState); }
{NUM} { insertAsInt(yytext, yyleng, NUM, lexerState); }
. { insertAsText(yytext, yyleng, ERROR, lexerState); }
%%

int main(int argCount, char *argVector[]) {
	/* Dunno if I'll need it
	++argv, --argc;  // skip over program name
	*/
    lexerState = initState();
	yylex();
}
