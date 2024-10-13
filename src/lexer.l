/*+++ a lexer for c- +++*/

/* Will disable wrapping at the end of file */
%option noyywrap
/* Will make `yytext` return a char pointer */
%pointer
%{
#include "cminus.h" // Declares functions to be used by Lexer
#include <stdbool.h> // Declares boolean type
int currentLine = 1;
bool isFirstMatch = true;
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
GE >=
LE <=
EQ ==
NE !=
RELOP [<>+-*]|{GE}?|{LE}?|{EQ}?|{NE}? 
SPECIAL_CHARS [*/;,()[\]{}]|{GE}?|{LE}?|{EQ}?|{NE}? 
KEYWORDS int|void|if|else|while|return

%%

{ID} { 
	insertId(yytext, yyleng, currentLine, isFirstMatch); 
	if (isFirstMatch) isFirstMatch = false;
	printf("\nCurrent line: %i, Current token: %s\n", currentLine, yytext);
}
{SPACE}|{TAB} { /* do something */}
{NUM} {/* do something */}
{SPECIAL_CHARS} {/* do something */}
{KEYWORDS} {/* do something */}
{NEW_LINE} { currentLine++; }
{BEGIN_COMMENT}  %{ currentLine = skipComment(input, currentLine); %}
            
%%

int main(int argCount, char *argVector[]) {
	/* Dunno if I'll need it
	++argv, --argc;  // skip over program name
	*/
	symbolTable = NULL;
	firstToken = NULL;
	lastToken = NULL;
	yylex();
}
