CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iinclude -std=c11 -O2 -g
LDFLAGS := 

SRC_DIR := src
BIN_DIR := bin
INC_DIR := include

SOURCES := $(wildcard $(SRC_DIR)/*.c)
TARGET  := $(BIN_DIR)/heap


all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilation Completed: $@"

clean:
	@rm -rf $(BIN_DIR)/*
	@echo "bin/ cleaned"

run: all
	@./$(TARGET)

.PHONY: all clean run
