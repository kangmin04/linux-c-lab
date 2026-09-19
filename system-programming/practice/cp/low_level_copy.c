#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }

    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1) { perror("open source"); exit(1); }

    int out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) { perror("open destination"); close(in_fd); exit(1); }

    char buf[BUFSIZE];
    ssize_t n_read;
    while ((n_read = read(in_fd, buf, BUFSIZE)) > 0) {
        ssize_t n_written = 0;
        while (n_written < n_read) {
            ssize_t w = write(out_fd, buf + n_written, n_read - n_written);
            if (w == -1) { perror("write"); close(in_fd); close(out_fd); exit(1); }
            n_written += w;
        }
    }
    if (n_read == -1) perror("read");

    close(in_fd);
    close(out_fd);
    return 0;
}
