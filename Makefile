# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Werror

# 可执行文件名称
GEN_EXEC = mazegen
PLAY_EXEC = main

# 源文件
GEN_SRCS = mazegen.c
PLAY_SRCS = main.c

# 目标文件
GEN_OBJS = $(GEN_SRCS:.c=.o)
PLAY_OBJS = $(PLAY_SRCS:.c=.o)

# 默认目标，编译所有代码
all: $(GEN_EXEC) $(PLAY_EXEC)

# 生成迷宫文件的可执行文件
$(GEN_EXEC): $(GEN_OBJS)
	$(CC) $(CFLAGS) -o $(GEN_EXEC) $(GEN_OBJS)

# 运行迷宫游戏的可执行文件
$(PLAY_EXEC): $(PLAY_OBJS)
	$(CC) $(CFLAGS) -o $(PLAY_EXEC) $(PLAY_OBJS)

# 编译每个源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标文件和可执行文件
clean:
	del $(GEN_OBJS) $(PLAY_OBJS) $(GEN_EXEC) $(PLAY_EXEC)