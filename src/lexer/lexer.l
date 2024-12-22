/*+++ a lexer for c- +++*/

/* Will disable wrapping at the end of file */
%option noyywrap
/* Will make `yytext` return a char pointer */
%pointer
%{
#include <stdbool.h> // Declares boolean type
#include "cminus.h"
#include "syntax.tab.h"

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
//	printf("\nLENDO RELOP: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return RELOP; 
}
{SUMOP} {
//	printf("\nLENDO SOMA: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return SUMOP;
}
{MULTOP} {
//	printf("\nLENDO MULTIPLICACAO: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return MULTOP;
}
{SPECIAL_CHARS} {
//	printf("\nLENDO CHAR SPECIAL: %s\n", yytext);
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
//	printf("\nLENDO IF: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return IF;
}
{ELSE} {
//	printf("\nLENDO ELSE: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return ELSE;
}
{WHILE} {
//	printf("\nLENDO WHILE: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return WHILE;
}
{RETURN} {
//	printf("\nLENDO RETURN: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return RETURN;
}
{TYPE_INT} {
//	printf("\nLENDO INT: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return TYPE_INT;
}
{TYPE_VOID} {
//	printf("\nLENDO VOID: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1001, getState());
	return TYPE_VOID;
}
{BEGIN_COMMENT}  {
    char * comment = skipComment(yytext, yyleng, input, getState());
//    printf("\nLENDO COMENTARIO: %s\n", comment);
    yylval.text = comment;
    insertAsText(yytext, yyleng, 1000, getState());
}
{ID} {
//	printf("\nLENDO ID: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1001, getState());
	return ID;
}
{NUM} {
//	printf("\nLENDO NUMERO: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return NUM;
}
. {
//	printf("\nLENDO ERRO: %s\n", yytext);
	yylval.text = strdup(yytext);
    insertAsText(yytext, yyleng, 1234, getState());
	return *yytext;
}

%%