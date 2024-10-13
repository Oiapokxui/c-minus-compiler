#! /bin/bash
mkdir -p bin
cd src
flex lexer.l
gcc -g lex.yy.c -o ../bin/lexer.exe cminus.c symbol.c
