#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int main() {
    char filename[100];
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // 获取用户输入的文件名
    printf("请输入要读取的文件名: ");
    if (scanf("%99s", filename) != 1) {
        fprintf(stderr, "读取文件名失败\n");
        return EXIT_FAILURE;
    }

    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }

    // 读取并显示文件内容
    printf("\n文件内容如下:\n");
    printf("----------------------------------------\n");
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        printf("%s", line);
    }
    printf("----------------------------------------\n");

    // 关闭文件
    if (fclose(file) != 0) {
        perror("关闭文件失败");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}    