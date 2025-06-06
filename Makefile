# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Werror

# 可执行文件名称
GEN_EXEC = mazegen
PLAY_EXEC = maze

# 源文件
GEN_SRCS = mazegen.c
PLAY_SRCS = maze.c

# 目标文件
GEN_OBJS = $(GEN_SRCS:.c=.o)
PLAY_OBJS = $(PLAY_SRCS:.c=.o)

# 默认目标，编译所有代码
all: $(GEN_EXEC) $(PLAY_EXEC)

# 生成迷宫文件的可执行文件
$(GEN_EXEC): $(GEN_OBJS)
	$(CC) $(CFLAGS) -o $@ $(GEN_OBJS)

# 运行迷宫游戏的可执行文件
$(PLAY_EXEC): $(PLAY_OBJS)
	$(CC) $(CFLAGS) -o $@ $(PLAY_OBJS)

# 编译每个源文件生成目标文件（添加头文件依赖）
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标文件和可执行文件（兼容类 Unix 和 Windows）
clean:
	@echo "清理文件..."
	@rm -f $(GEN_OBJS) $(PLAY_OBJS) $(GEN_EXEC) $(PLAY_EXEC)
	@del /Q $(GEN_OBJS) $(PLAY_OBJS) $(GEN_EXEC) $(PLAY_EXEC) 2 || true