#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

// 定义迷宫结构体
typedef struct {
    char **grid;
    int width;
    int height;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int player_x;
    int player_y;
} Maze;

// 初始化迷宫
void init_maze(Maze *maze, int width, int height) {
    maze->width = width;
    maze->height = height;
    maze->grid = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        maze->grid[i] = (char *)malloc(width * sizeof(char));
    }
}

// 释放迷宫内存
void free_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
}

// 打印迷宫
void print_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (i == maze->player_y && j == maze->player_x) {
                printf("X");
            } else {
                printf("%c", maze->grid[i][j]);
            }
        }
        printf("\n");
    }
}

// 主函数，用于运行迷宫游戏
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mazefile>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 2;
    }

    // 读取迷宫文件
    Maze maze;
    int width = 0, height = 0;
    char line[MAX_WIDTH + 1];
    while (fgets(line, sizeof(line), file) != NULL) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[--len] = '\0';
        }
        if (width == 0) {
            width = len;
        } else if (width != len) {
            fprintf(stderr, "Invalid maze: inconsistent line lengths\n");
            fclose(file);
            return 3;
        }
        height++;
    }
    fseek(file, 0, SEEK_SET);

    // 初始化迷宫
    init_maze(&maze, width, height);

    // 填充迷宫数据
    for (int i = 0; i < height; i++) {
        fgets(line, sizeof(line), file);
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[--len] = '\0';
        }
        for (int j = 0; j < width; j++) {
            maze.grid[i][j] = line[j];
            if (line[j] == 'S') {
                maze.start_x = j;
                maze.start_y = i;
                maze.player_x = j;
                maze.player_y = i;
            } else if (line[j] == 'E') {
                maze.end_x = j;
                maze.end_y = i;
            }
        }
    }
    fclose(file);

    // 游戏循环
    char input;
    while (1) {
        printf("Enter command (W/A/S/D/Q/M): ");
        scanf(" %c", &input);

        switch (input) {
            case 'W':
            case 'w':
                if (maze.player_y > 0 && maze.grid[maze.player_y - 1][maze.player_x] != '#') {
                    maze.player_y--;
                } else {
                    printf("Invalid move!\n");
                }
                break;
            case 'A':
            case 'a':
                if (maze.player_x > 0 && maze.grid[maze.player_y][maze.player_x - 1] != '#') {
                    maze.player_x--;
                } else {
                    printf("Invalid move!\n");
                }
                break;
            case 'S':
            case 's':
                if (maze.player_y < maze.height - 1 && maze.grid[maze.player_y + 1][maze.player_x] != '#') {
                    maze.player_y++;
                } else {
                    printf("Invalid move!\n");
                }
                break;
            case 'D':
            case 'd':
                if (maze.player_x < maze.width - 1 && maze.grid[maze.player_y][maze.player_x + 1] != '#') {
                    maze.player_x++;
                } else {
                    printf("Invalid move!\n");
                }
                break;
            case 'Q':
            case 'q':
                free_maze(&maze);
                return 0;
            case 'M':
            case 'm':
                print_maze(&maze);
                break;
            default:
                printf("Invalid command!\n");
        }

        if (maze.player_x == maze.end_x && maze.player_y == maze.end_y) {
            printf("You win!\n");
            free_maze(&maze);
            return 0;
        }
    }

    return 0;
}