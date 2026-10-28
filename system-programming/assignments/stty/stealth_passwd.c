//2023014913 김강민
#include <stdio.h>
#include <termios.h>  // tcflag_t(unsigned int) 타입의 c_lflag에서 주로 ISIG, ICANON, ECHO 등 조정함
#include <string.h>
#include <stdlib.h>

void turnOffEcho(struct termios* info);
void restoreTerminal(struct termios* original_state);
void getPasswd(char* );

void error(char* log, int exit_code){
    perror(log);
    exit(exit_code);
}

int main(){
    char* init_password = "secret123";
    char password[128] = {0};
    int passInThreeTimes = 0;
    struct termios original;

    turnOffEcho(&original);

    for(int cnt = 1; cnt <= 3; cnt++){
        getPasswd(password);
        if(strcmp(init_password, password) == 0){
            printf("\nCorrect Password. You are authenticated.\n");
            passInThreeTimes = 1;
            break;
        }
        printf("\nWrong Password(Failure count: %d): %s\n", cnt, password);
    }

    if(!passInThreeTimes){
        printf("Password entered incorrectly 3 times. Program terminated.\n");
    }

    restoreTerminal(&original);
    return 0;
}

void turnOffEcho(struct termios* original_state){
    struct termios info;
    if(tcgetattr(0, original_state) == -1) error("tcgetattr", 1);
    info = *original_state; // copy struct value -> seperate data
    info.c_lflag &= ~ECHO;
    if(tcsetattr(0, TCSANOW, &info) == -1) error("tcsetattr", 1);
}

void restoreTerminal(struct termios* original_state){
    if(tcsetattr(0, TCSANOW, original_state) == -1) error("tcsetattr", 1);
}

void getPasswd(char* password){
    printf("Enter Password: ");
    fflush(stdout);

    if(fgets(password, 128, stdin) == NULL){  
        password[0] = '\0';
        clearerr(stdin);
        return;
    }

    if(strchr(password, '\n') == NULL){       
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }

    password[strcspn(password, "\n")] = '\0';
}
