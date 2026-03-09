CC      := gcc
CFLAGS  := -Wall -Wextra -I.

BIN     := bin
TARGET  := $(BIN)/main_proc

SRCS := main.c \
        args.c \
        common/file_op.c \
        common/log.c \
        command/clone_single_thread.c \
        command/clone_multi_thread.c \
        command/clone_non_thread.c \
        command/fork.c \
        command/vfork.c \
        command/clone_single_thread_execve.c \
        command/clone_multi_thread_execve.c \
        command/clone_non_thread_execve.c

OBJS := $(patsubst %.c, $(BIN)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN)
