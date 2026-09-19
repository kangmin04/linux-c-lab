#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }

    int count = 0;
    while (1) {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            printf("%d번째 open()에서 실패했다: ", count);
            perror("open");
            break;
        }
        count++;
        // 일부러 close(fd)를 호출하지 않는다 -> 여기가 "누수" 지점
    }
    printf("총 %d개의 fd를 연 뒤 더 이상 못 열었다.\n", count);
    return 0;
}
