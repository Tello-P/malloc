CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iinclude -std=c11 -O2 -g
LDFLAGS := 

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET  := $(BIN_DIR)/heap

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Linked: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

clean:
	@rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*
	@echo "Cleaned bin/ and obj/"

run: all
	@./$(TARGET)

.PHONY: all clean run
