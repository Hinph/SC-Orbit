CC = gcc
CFLAGS = -Wall -pedantic
LDFLAGS = -lm

ifdef RELEASE
CFLAGS += -O2 -s
else
CFLAGS += -g -fno-omit-frame-pointer
endif

ifdef ASAN
CFLAGS += -fsanitize=address
endif

ROOT_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BIN_DIR = $(abspath $(ROOT_DIR)/bin)
OBJ_DIR = $(abspath $(ROOT_DIR)/obj)
SRC_DIR = $(abspath $(ROOT_DIR)/src)

OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/gamepad.o \
       $(OBJ_DIR)/gamepad_active.o \
       $(OBJ_DIR)/sc_gamepad_state.o \
       $(OBJ_DIR)/stopwatch.o \
       $(OBJ_DIR)/triton.o \
       $(OBJ_DIR)/utils.o \
       $(OBJ_DIR)/virtual_gamepad.o \
       $(OBJ_DIR)/virtual_mouse.o

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

