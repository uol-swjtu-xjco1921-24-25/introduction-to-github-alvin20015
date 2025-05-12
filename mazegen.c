#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100
#define MIN_SIZE 5
#define MAX_SIZE 100

// 定义迷宫结构体
typedef struct {
    char **grid;
    int width;
    int height;
} Maze;

// 初始化迷宫
int init_maze(Maze *maze, int width, int height) {
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "宽度和高度必须为正整数\n");
        return -1;
    }
    maze->width = width;
    maze->height = height;
    maze->grid = (char **)malloc(height * sizeof(char *));
    if (maze->grid == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return -1;
    }
    for (int i = 0; i < height; i++) {
        maze->grid[i] = (char *)malloc(width * sizeof(char));
        if (maze->grid[i] == NULL) {
            fprintf(stderr, "内存分配失败\n");
            for (int j = 0; j < i; j++) {
                free(maze->grid[j]);
            }
            free(maze->grid);
            return -1;
        }
        for (int j = 0; j < width; j++) {
            maze->grid[i][j] = '#';
        }
    }
    return 0;
}

// 释放迷宫内存
void free_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
}

// 保存迷宫到文件
int save_maze(Maze *maze, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("无法打开文件");
        return -1;
    }
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            fputc(maze->grid[i][j], file);
        }
        fputc('\n', file);
    }
    fclose(file);
    return 0;
}

// 生成迷宫
int generate_maze(const char *filename, int width, int height) {
    if (width < MIN_SIZE || width > MAX_SIZE || height < MIN_SIZE || height > MAX_SIZE) {
        fprintf(stderr, "宽度和高度必须在 %d 到 %d 之间\n", MIN_SIZE, MAX_SIZE);
        return -1;
    }

    Maze maze;
    if (init_maze(&maze, width, height) != 0) {
        return -1;
    }

    if (save_maze(&maze, filename) != 0) {
        free_maze(&maze);
        return -1;
    }
    free_maze(&maze);
    return 0;
}

// 主函数，用于生成迷宫文件
int main(int argc, char *argv[]) {

    if (generate_maze("test.txt", 5, 5) != 0) {
        return 1;
    }

    return 0;
}
    