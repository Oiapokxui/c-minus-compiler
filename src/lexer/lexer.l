/*+++ a lexer for c- +++*/

/* Will disable wrapping at the end of file */
%option noyywrap
/* Will make `yytext` return a char pointer */
%pointer
%{
#include <stdbool.h> // Declares boolean type
#include "cminus.h"
#include "parser.tab.h"
#include "parser/parser.h"

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
MULTOP [\*/]
RELOP {GT}|{LT}|{GE}|{LE}|{EQ}|{NE}
OPEN_PAREN [\(]
CLOSE_PAREN [)]
SEP_COMMA ,
OPEN_BRACE \{
CLOSE_BRACE \}
OPEN_BRACKET \[
CLOSE_BRACKET \]
SPECIAL_CHARS [/;=]
IF if
ELSE else
WHILE while
RETURN return
TYPE_VOID void
TYPE_INT int

%%

{WHITESPACE} {
	insertWhitespace(yytext, yyleng, getState());
}
{RELOP} { 
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return RELOP; 
}
{SUMOP} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return SUMOP;
}
{MULTOP} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return MULTOP;
}
{SPECIAL_CHARS} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
    return yytext[0];
}
{OPEN_PAREN} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return '(';
}
{CLOSE_PAREN} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return ')';
}
{OPEN_BRACKET} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return '[';
}
{CLOSE_BRACKET} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return ']';
}
{OPEN_BRACE} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return '{';
}
{CLOSE_BRACE} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return '}';
}
{SEP_COMMA} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return ',';
}
{IF} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return IF;
}
{ELSE} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return ELSE;
}
{WHILE} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return WHILE;
}
{RETURN} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return RETURN;
}
{TYPE_INT} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return TYPE_INT;
}
{TYPE_VOID} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1001, getState());
	return TYPE_VOID;
}
{BEGIN_COMMENT}  {
    char * comment = skipComment(yytext, yyleng, input, getState());
    yylval.text = comment;
    insertAsText(yytext, yyleng, 1000, getState());
}
{ID} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1001, getState());
	return ID;
}
{NUM} {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return NUM;
}
. {
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return *yytext;
}

%%