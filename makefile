# Nome do executável
TARGET = lexer.exe

# Compilador
CC = gcc

# Sources directory
SRC_DIR = src

# Binaries and intermediaries directory
BIN_DIR = bin

# Flags de compilação
CFLAGS = -foptimize-sibling-calls -g -I ${SRC_DIR}

# Arquivo lex que vai ser processado pelo FLEX
LEX_SRC = ${SRC_DIR}/lexer.l

# Arquivo fonte gerado pelo Flex
LEX_CXX_SRC = lex.yy.c

all: clean build run

clean-build: clean build

# Regras principais
build: $(TARGET)

# Regra para compilar o executável
$(TARGET): ${BIN_DIR}/lex.yy.o  ${BIN_DIR}/token.o ${BIN_DIR}/symbol.o ${BIN_DIR}/cminus.o
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar o código-fonte em C a partir do Flex
${BIN_DIR}/$(LEX_CXX_SRC):
	flex -o $@ $(LEX_SRC)

# Regras para compilar os arquivos objeto
${BIN_DIR}/symbol.o: src/symbol.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/token.o: src/token.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/cminus.o: src/cminus.c
	$(CC) $(CFLAGS) -o $@ -c $<

${BIN_DIR}/lex.yy.o: ${BIN_DIR}/$(LEX_CXX_SRC)
	$(CC) $(CFLAGS) -o $@ -c $<

# Regra para executar o binário
run: $(TARGET)
	./$(TARGET) < examples/prg1.c-

# Limpeza dos arquivos gerados
clean:
	rm -f $(BIN_DIR)/*.o ${BIN_DIR}/lex.yy.c $(TARGET)