# Nome do executável
TARGET = cminus.exe

# Compilador
CC = gcc

# Sources directory
SRC_DIR = src

# Binaries and intermediaries directory
BIN_DIR = bin

# Flags de compilação
CFLAGS = -Wall -Wextra -foptimize-sibling-calls -g -I ${SRC_DIR}

# Arquivo lex que vai ser processado pelo FLEX
LEX_SRC = ${SRC_DIR}/lexer/lexer.l

# Arquivo fonte gerado pelo Flex
LEX_CXX_SRC = lex.yy.c

# Interpretador BISON
BISON = /opt/homebrew/opt/bison/bin/bison

# Arquivo YACC que vai ser processado pelo BISON
YACC_SRC = ${SRC_DIR}/parser/parser.y
#
# Arquivo fonte gerado pelo Bison
YACC_CXX_SRC = parser.tab.c

all: clean build run

clean-build: clean build

# Regras principais
build: $(TARGET)

# Regra para compilar o executável
$(TARGET): ${BIN_DIR}/parser.tab.o ${BIN_DIR}/lex.yy.o ${BIN_DIR}/token.o ${BIN_DIR}/symbol.o ${BIN_DIR}/cminus.o ${BIN_DIR}/syntax.o ${BIN_DIR}/errors.o ${BIN_DIR}/expression.o
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar o código-fonte do lexer em C a partir do Flex
${BIN_DIR}/$(LEX_CXX_SRC):
	flex -o $@ $(LEX_SRC)

# Regra para gerar o código-fonte do parser em C a partir do Bison
${BIN_DIR}/$(YACC_CXX_SRC):
	$(BISON) -o $@ -v --debug -dy $(YACC_SRC)

# Regras para compilar os arquivos objeto
${BIN_DIR}/symbol.o: src/data/symbol.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/token.o: src/data/token.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/cminus.o: src/cminus.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/errors.o: src/error/error.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/expression.o: src/data/expression.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/syntax.o: src/syntax/syntax.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/lex.yy.o: ${BIN_DIR}/$(LEX_CXX_SRC)
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/parser.tab.o: ${BIN_DIR}/$(YACC_CXX_SRC)
	$(CC) $(CFLAGS) -o $@ -c $<

# Regra para executar o binário
run: $(TARGET)
	./$(TARGET) < examples/reject/prg18.c-

# Limpeza dos arquivos gerados
clean:
	rm -f $(BIN_DIR)/*.o ${BIN_DIR}/*.c ${BIN_DIR}/*.h $(TARGET) ${BIN_DIR}/.output **/*~
