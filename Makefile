CC = gcc
CFLAGS = -Wall -pedantic
LDFLAGS =

ifdef RELEASE
CFLAGS += -O2 -s
endif

ROOT_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BIN_DIR = $(abspath $(ROOT_DIR)/bin)
OBJ_DIR = $(abspath $(ROOT_DIR)/obj)
SRC_DIR = $(abspath $(ROOT_DIR)/src)

OBJS = $(OBJ_DIR)/main.o

$(BINDIR)/scwrapper: $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o bin/scwrapper

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(BIN_DIR):
	@mkdir -p "$(BIN_DIR)"

$(OBJ_DIR):
	@mkdir -p "$(OBJ_DIR)"

.PHONY: clean
clean:
	rm -rf "$(BIN_DIR)" "$(OBJ_DIR)" "$(ROOT_DIR)/mister.tar.gz"

