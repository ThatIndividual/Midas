BIN = bin
SRC = src/cmidas

CC     = gcc
CFLAGS = -std=c99 -pedantic -Wextra -Wall -g
SOURCE = main.c lexer.c parser.c interpreter.c vector.c hash.c error.c
OBJ    = $(addprefix ./src/cmidas/, $(SOURCE))
TARGET = $(addprefix $(BIN)/, cmidas.elf)

default: cmidas

cmidas: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

clean:
	rm -r bin/*

.PHONY: cmidas clean
