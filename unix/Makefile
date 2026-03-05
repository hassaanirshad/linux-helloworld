CC      := gcc
CFLAGS  := -Wall -Wextra -Wno-unused-function -I.

BIN     := bin
SERVER  := $(BIN)/main_server
CLIENT  := $(BIN)/main_client

COMMON_SRCS := common/args.c \
               common/file_op.c

SERVER_SRCS := server/main.c \
               server/args.c \
               server/command/fd_transfer.c \
               $(COMMON_SRCS)

CLIENT_SRCS := client/main.c \
               client/args.c \
               client/command/fd_transfer.c \
               $(COMMON_SRCS)

SERVER_OBJS := $(patsubst %.c, $(BIN)/%.o, $(SERVER_SRCS))
CLIENT_OBJS := $(patsubst %.c, $(BIN)/%.o, $(CLIENT_SRCS))

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN)
