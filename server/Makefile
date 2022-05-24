# ----- directories -----

INC_DIR = inc
SRC_DIR = src
BUILD_DIR = build

# ----- compilation -----

CC = clang
CFLAGS = -O3 -I$(INC_DIR) -std=c11 -pedantic

# ----- sources -----

SOURCES = $(shell find $(SRC_DIR) -name *.c -or -name *.s)

# ----- output -----

BIN = server

# ----- targets -----

all: clean build
	@ echo "target 'all' complete"

clean:
	rm -r $(BUILD_DIR)/*
	@ echo "target 'clean' complete"

build: $(BUILD_DIR)/$(BIN)
	@ echo "target 'build' complete"

$(BUILD_DIR)/$(BIN): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^
