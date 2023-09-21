CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

BUILD_DIR = ../build

all: graph

graph: ./graph.c ./stack.c ./parser.c ./notation.c ./s21_string.c ./draw.c ./input.c
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/graph

clean:
	rm -f $(BUILD_DIR)/*

rebuild: clean all
