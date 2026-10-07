//2023014913 김강민
#include <stdio.h> 
#include <termios.h>  // tcflag_t(unsigned int) 타입의 c_lflag에서 주로 ISiG, ICANNON, ECHO 등 조정함 
#include <string.h> 
#include <stdlib.h> 

void turnOffEcho(struct termios* info);
// void turnOnEcho(struct termios* info); 
void restoreTerminal(struct termios* original_state); 
void getPasswd(char* ); 

void error(char* log, int exit_code){
    perror(log); 
    exit(exit_code); 
}

int main(){
    char* init_password = "secret123"; // 이때 마지막 문자열
    char password[128] = {0}; 
    int passInThreeTimes = 0; 
    struct termios original; // 원래대로 돌려주는걸 굳이 다시 |= ECHO 추가해도 되겠지만, 처음 변환할 때 original state를 저장해두고 마지막에 tcsetattr로 해당 original 저장하는게 더 깔끔하다 !!!! 

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
    if(tcgetattr(0, original_state) == -1) error("tcgetattr", 1); // tcgetattr(int fd, struct termios *info) : fd와 연결된 터미널 설정값을 info에 저장함. 성공 시 0, 에러 시 -1
    info = *original_state; // 구조체를 통째로 복사해주는 방법 !!! 
    info.c_lflag &= ~ECHO;  // ECHO기능을 끄고자, 기존엔 echo만 1이고 나머진 0일텐데, 이걸 ~로 echo만 0 나머진 1로 변경 -> and 연산이라 echo 비트는 무조건 꺼짐. 
    //!!변경하고 무조건 변경된속성을 설정해줘야함. 
    if(tcsetattr(0, TCSANOW, &info) == -1) error("tcsetattr", 1); 
}

// void turnOnEcho(struct termios* info){
//     info->c_lflag |= ECHO; 
//     if(tcsetattr(0, TCSANOW, info) == -1) error("tcsetattr", 1); 
// }
void restoreTerminal(struct termios* original_state){
    if(tcsetattr(0, TCSANOW, original_state) == -1) error("tcsetattr", 1); 
}

void getPasswd(char* password){
    printf("Enter Password: ");
    fgets(password, 128, stdin); //fgets는 전달받은 버퍼(포인터)의 첫 번째 바이트(index 0)부터 읽어온 문자열을 새로 채워 넣음 -> 오버라이트
    // password[strlen(password) - 1] = '\0'; 
    password[strcspn(password, "\n")] = '\0'; 
    // • size_t strcspn(const char *str1, const char *str2);
    //str1 문자열의 처음부터 시작하여 str2에 포함된 문 중 어느 하나라도 일치하는 문자가 나올 때까지 세어본 누적 개수를 리턴

    // printf("password : %s\n", password); 
    // printf("size: %d\n", (int)strlen(password)); 
}

// scanf("%s", ...)를 잘 쓰지 않는 이유
// 공백 문제: scanf("%s", ...)는 공백(스페이스, 탭, 줄바꿈)을 문자열의 끝으로 인식하므로, 띄어쓰기가 들어간 문장을 읽지 못합니다. (예: "hello world" 입력 시 "hello"만 읽음)
// 버퍼 오버플로우 위험: 버퍼 크기를 제한하지 않고 %s를 쓰면, 버퍼 크기를 초과하는 입력이 들어왔을 때 메모리 오염(Buffer Overflow)이 발생합니다.
// 입력 버퍼에 남는 개행 문자(\n): scanf 이후에 다른 입력을 받을 때 남겨진 \n 때문에 다음 입력이 스킵되는 버그가 자주 생깁니다.

// fgets(..., sizeof(...), stdin)가 권장되는 이유
// 공백 포함 읽기: 줄바꿈(\n)을 만날 때까지 한 줄 전체를 읽어옵니다.
// 버퍼 오버플로우 방지: 버퍼의 maximum 크기를 직접 지정하므로 안전합니다.
// Null 종단 보장: 지정한 크기만큼 안전하게 읽고 항상 마지막에 \0을 붙여줍니