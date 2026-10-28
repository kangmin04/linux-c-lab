#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#define PAGELEN 24
#define BUFFERSIZE 512

void do_more(FILE* fp);
int see_more(FILE* cmd);

int main(int ac, char* av[]){
    if (ac < 2) return 1;
    FILE* fp = fopen(av[1], "r");
    if (fp == NULL) return 1;
    do_more(fp);
    fclose(fp);
    return 0;
}

void do_more(FILE* fp){
    FILE* fp_tty = fopen("/dev/tty", "r");
    if (fp_tty == NULL) exit(1);
    int curpage = 0;
    char buf[BUFFERSIZE];
    int lineNum = 0;
    while (fgets(buf, BUFFERSIZE, fp) != NULL){
        lineNum++;
        if (curpage == PAGELEN){
            int reply = see_more(fp_tty);
            if (reply == 0) break;
            curpage -= reply;
        }
        if (printf("%d %s", lineNum, buf) < 0) exit(1);
        curpage++;
    }
    fclose(fp_tty);
}

int see_more(FILE* cmd){
    struct termios orig_mode, new_mode;
    int tty = fileno(cmd);
    if (tcgetattr(tty, &orig_mode) == -1) return 0;
    new_mode = orig_mode;
    new_mode.c_lflag &= ~(ICANON | ECHO);   // 엔터 없이 바로 입력, 입력 문자 화면에 안 찍기
    new_mode.c_cc[VMIN] = 1;                // 최소 1바이트 오면 read 반환
    new_mode.c_cc[VTIME] = 0;
    tcsetattr(tty, TCSANOW, &new_mode);

    printf("\033[7m more? \033[m");         // 반전 색상 프롬프트
    fflush(stdout);

    int c, reply = 0;
    while ((c = getc(cmd)) != EOF){
        if (c == 'q')  { reply = 0;       break; }
        if (c == ' ')  { reply = PAGELEN; break; }
        if (c == '\n') { reply = 1;       break; }
    }
    tcsetattr(tty, TCSANOW, &orig_mode);    // 어느 경로로 나가든 원래 모드로 복구
    printf("\r\033[K");                     // 프롬프트 지우기
    return reply;
}