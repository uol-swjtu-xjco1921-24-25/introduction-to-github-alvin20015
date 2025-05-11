# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Werror

# 可执行文件名称
PLAY_EXEC = main

# 源文件
PLAY_SRCS = main.c

# 目标文件
PLAY_OBJS = $(PLAY_SRCS:.c=.o)

# 默认目标，编译所有代码
all: $(GEN_EXEC) $(PLAY_EXEC)

# 运行迷宫游戏的可执行文件
$(PLAY_EXEC): $(PLAY_OBJS)
	$(CC) $(CFLAGS) -o $(PLAY_EXEC) $(PLAY_OBJS)

# 编译每个源文件生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标文件和可执行文件
clean:
	del $(PLAY_OBJS) $(PLAY_EXEC)