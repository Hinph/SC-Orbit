#
# Compiler flags
#
CC      ?= gcc
CFLAGS  += -Wall -pedantic -MMD
LDFLAGS += -lm

BUILD ?= debug
ifdef RELEASE
    ifeq ($(RELEASE),1)
        BUILD = release
    endif
endif

ifdef STATIC
    CFLAGS += -static
endif

#
# Debug build settings
#
ifeq ($(BUILD),debug)
    CFLAGS += -g -fno-omit-frame-pointer
    ifeq ($(ASAN),1)
        CFLAGS += -fsanitize=address
    endif
endif

#
# Release build settings
#
ifeq ($(BUILD),release)
    CFLAGS += -O2 -s
endif

#
# Sources, Objects, and Dependencies
#
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=obj/%.o)
DEPS := $(OBJS:obj/%.o=obj/%.d)

#
# Targets
#
bin/scwrapper: $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf bin obj scwrapper.mister.tar.gz

#
# Docker Toolchains
#
.PHONY: toolchain-x86_64
toolchain-x86_64:
	docker build -f toolchains/Dockerfile.x86_64 -t $@ toolchains

.PHONY: toolchain-arm7vl
toolchain-arm7vl:
	docker build -f toolchains/Dockerfile.armv7l -t $@ toolchains

#
# Static Release Builds
#
UID := $(shell id -u)
GID := $(shell id -g)

.PHONY: scwrapper.x86_64
scwrapper.x86_64: toolchain-x86_64
	docker run -it -v "${PWD}:/project" --user "$(UID):$(GID)" --rm toolchain-x86_64 bash -c 'cd /project && make STATIC=1 RELEASE=1'

.PHONY: scwrapper.arm7vl
scwrapper.arm7vl: toolchain-arm7vl
	docker run -it -v "${PWD}:/project" --user "$(UID):$(GID)" --rm toolchain-arm7vl bash -c 'cd /project && make STATIC=1 RELEASE=1'

-include $(DEPS)
