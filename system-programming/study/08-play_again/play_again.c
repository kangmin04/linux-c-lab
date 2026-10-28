#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <termio.h>
#include <signal.h>

#define TRIES 3
#define SLEEPTIME 2
#define QUESTION "Do you want another transaction"
#define BEEP putchar('\a');
int get_ok_char();
int get_response(char *question, int max_tries);
void set_cr_noecho_mode();
void set_non_blocking_mode();
void tty_mode(int how);
void ctrlc_handler(int signum);  

int main(){
    int response = 0; 
    tty_mode(0); //백업
    set_cr_noecho_mode(); //ECHO, ICANON 끄기
    set_non_blocking_mode(); 

    signal(SIGINT, ctrlc_handler);
    signal(SIGQUIT, SIG_IGN); 

    response = get_response(QUESTION, TRIES); 
    tty_mode(1); 
    return response; 
}

void ctrlc_handler(int signum){
    printf("pressed num : %d", signum);
    tty_mode(1); 
    exit(130); 
}

int get_ok_char(){
    int c; 
    while((c = getchar()) != EOF && strchr("ynYN", c) ==NULL) continue; 
    return c; 
}
int get_response(char* question, int max_tries){
    int input; 
    printf("%s (y/n)", question);
    fflush(stdout);
    while(1){
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());
        max_tries -= 1; // already tried once
		if (input == (int)'y')
		{
			return 0;
		}
		if (input == (int)'n')
		{
			return 1;
		}
		if (max_tries == 0)
		{
			return 2;
		}
		BEEP; // alert user
	}
} 


void set_non_blocking_mode(){
    int state_flags = fcntl(0, F_GETFL); 
    state_flags |= O_NONBLOCK; 
    fcntl(0, F_SETFL, state_flags); 
}

void set_cr_noecho_mode(){
    struct termios state_termios; 
    if(tcgetattr(0, &state_termios) == -1){
        perror("err"); exit(1); 
    }
    state_termios.c_lflag &= ~(ECHO | ICANON); 
    state_termios.c_cc[VMIN] = 1; 
    //적용까지해줘야한다 !!!!! 
    if(tcsetattr(0, TCSANOW, &state_termios) == -1){
        perror("err"); exit(1); 
    }
}
void tty_mode(int how){
    static struct termios orig_termios; 
    static int fcntl_flags; 
    if(how == 0){
        //backup 설정
        tcgetattr(0, &orig_termios); 
        fcntl_flags = fcntl(0, F_GETFL); 
    }else if(how == 1){
        //backup 실행
        tcsetattr(0, TCSANOW, &orig_termios); 
        fcntl(0, F_SETFL, fcntl_flags); 
    }
}