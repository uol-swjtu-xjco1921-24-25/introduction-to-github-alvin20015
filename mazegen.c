#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100
#define MIN_SIZE 5
#define MAX_SIZE 100

#define WALL_CHAR '#'
#define PATH_CHAR ' '
#define START_CHAR 'S'
#define END_CHAR 'E'


// 定义迷宫结构体
typedef struct {
    char **grid;
    int width;
    int height;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
} Maze;

// 定义边结构体
typedef struct {
    int x1, y1;
    int x2, y2;
} Edge;

// 并查集数据结构
int parent[MAX_WIDTH * MAX_HEIGHT];

// 查找元素所在集合的根节点
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// 合并两个集合
void union_sets(int x, int y) {
    int root_x = find(x);
    int root_y = find(y);
    if (root_x != root_y) {
        parent[root_x] = root_y;
    }
}

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
            maze->grid[i][j] = WALL_CHAR;
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

// 随机设置起点和终点，且起点和终点不重叠，并有一定距离
void set_start_end(Maze *maze) {
    srand(time(NULL));
    int start_x, start_y, end_x, end_y;

    // 随机选择起点
    do {
        start_x = rand() % (maze->width - 2) + 1;
        start_y = rand() % (maze->height - 2) + 1;
    } while (maze->grid[start_y][start_x] != ' ');

    // 随机选择终点，确保与起点不重叠且有一定距离
    do {
        end_x = rand() % (maze->width - 2) + 1;
        end_y = rand() % (maze->height - 2) + 1;
    } while (maze->grid[end_y][end_x] != ' ' || 
             (start_x == end_x && start_y == end_y) || 
             (abs(start_x - end_x) + abs(start_y - end_y) < 3));

    // 设置起点和终点
    maze->start_x = start_x;
    maze->start_y = start_y;
    maze->end_x = end_x;
    maze->end_y = end_y;
    maze->grid[start_y][start_x] = START_CHAR;
    maze->grid[end_y][end_x] = END_CHAR;

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

void shuffle_edges(Edge *edges, int edge_count) {
    for (int i = edge_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Edge temp = edges[i];
        edges[i] = edges[j];
        edges[j] = temp;
    }
}


// 使用Kruskal算法生成迷宫
void kruskal_maze(Maze *maze) {
    srand(time(NULL));
    int num_edges = (maze->width - 2) * (maze->height - 2) * 2;
    Edge *edges = (Edge *)malloc(num_edges * sizeof(Edge));
    int edge_count = 0;

    // 初始化并查集
    for (int i = 0; i < maze->width * maze->height; i++) {
        parent[i] = i;
    }

    // 初始化所有可能的边
    for (int y = 1; y < maze->height - 1; y += 2) {
        for (int x = 1; x < maze->width - 1; x += 2) {
            if (x + 2 < maze->width - 1) {
                edges[edge_count].x1 = x;
                edges[edge_count].y1 = y;
                edges[edge_count].x2 = x + 2;
                edges[edge_count].y2 = y;
                edge_count++;
            }
            if (y + 2 < maze->height - 1) {
                edges[edge_count].x1 = x;
                edges[edge_count].y1 = y;
                edges[edge_count].x2 = x;
                edges[edge_count].y2 = y + 2;
                edge_count++;
            }
        }
    }

    // 随机打乱边的顺序
    shuffle_edges(edges, edge_count);


    // 遍历所有边
    for (int i = 0; i < edge_count; i++) {
        int x1 = edges[i].x1;
        int y1 = edges[i].y1;
        int x2 = edges[i].x2;
        int y2 = edges[i].y2;

        int index1 = y1 * maze->width + x1;
        int index2 = y2 * maze->width + x2;

        if (find(index1) != find(index2)) {
            maze->grid[y1][x1] = PATH_CHAR;
            maze->grid[y2][x2] = PATH_CHAR;
            maze->grid[(y1 + y2) / 2][(x1 + x2) / 2] = PATH_CHAR;
        }
    }

    free(edges);
}

void print_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            putchar(maze->grid[i][j]);
        }
        putchar('\n');
    }
}

// 生成迷宫
int generate_maze(const char *filename, int width, int height) {
    if (filename == NULL || filename[0] == '\0') {
        fprintf(stderr, "错误：文件名不能为空。\n");
        return -1;
    }

    if (width < MIN_SIZE || width > MAX_SIZE || height < MIN_SIZE || height > MAX_SIZE) {
        fprintf(stderr, "错误：宽度和高度必须在 %d 到 %d 之间\n", MIN_SIZE, MAX_SIZE);
        return -1;
    }

    Maze maze;
    if (init_maze(&maze, width, height) != 0) {
        return -1;
    }

    kruskal_maze(&maze);
    set_start_end(&maze);
    print_maze(&maze);

    if (save_maze(&maze, filename) != 0) {
        free_maze(&maze);
        return -1;
    }
    free_maze(&maze);
    return 0;
}


// 主函数，用于生成迷宫文件
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, " Usage: %s <filename> <width> <height>\n", argv[0]);
        return 1;
    }
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (width % 2 == 0  || height % 2 == 0) {
        if (width % 2 == 0) width--;
        if (height % 2 == 0) height--;
        printf("Note: The width and height of the maze have been adjusted to odd numbers, and the actual generated size is %d x %d\n", width, height);
    }
    


    if (generate_maze(argv[1], width, height) != 0) {
        return 1;
    }

    return 0;
}
    