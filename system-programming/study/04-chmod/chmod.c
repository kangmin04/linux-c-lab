#include <stdio.h> 
#include <sys/stat.h> 
#include <stdlib.h> 
#include <string.h> 

mode_t octal_to_mode(char* ); 
mode_t letter_to_mode(char* ); 

int main(int ac, char* av[]){
    if(ac != 3) exit(1); 
    struct stat stat_buf; 
    mode_t newmode; 

    if(stat(av[2], &stat_buf) == -1){
        fprintf(stderr, "err"); 
        exit(1); 
    }

    if((av[1][0] >= '0') && (av[1][0] <= '7')){
        newmode = octal_to_mode(av[1]); 
    }else if((strlen(av[1]) == 9) && ((av[1][0] == '-') || (av[1][0] == 'r'))){ 
        newmode = letter_to_mode(av[1]); 
    }else{
        fprintf(stderr, "error"); exit(1);
    }

    // mode_t before = stat_buf.st_mode; 
    // int chmod(const char *path, mode_t mode); 성공 시 0, 실패 시 -1
    if(chmod(av[2], newmode) == -1) {perror("error in ch테mod"); exit(1); }
    // printf("before mode: %s -> ", before); 
    printf("%o\n", newmode); 
    return 0;
}

mode_t octal_to_mode(char* str){
    mode_t mode = 0; ; 
    // const char* octTable[] = {"---", "--x", "010", "011", "100", "101", "110", "111"}
    for(int i = 0; str[i] != '\0'; i++){
        // if(str[i] == '0') continue; 
        // int number = str[i] - '0'; 
        if (str[i] < '0' || str[i] > '7') { fprintf(stderr, "bad digit\n"); exit(1); }
        mode = mode * 8 + (str[i] - '0'); 
    }
    return mode; 
}

mode_t letter_to_mode(char* s){
    const mode_t bits[9] = {
        S_IRUSR, S_IWUSR, S_IXUSR, 
        S_IRGRP, S_IWGRP, S_IXGRP, 
        S_IROTH, S_IWOTH, S_IXOTH, 
    }; 

    char* target= "rwxrwxrwx"; //  포인터에 [i]를 붙이면 그 주소에서 i칸 떨어진 값 하나를 꺼냅니다. 배열이든 포인터든 같음. 즉 수정가능하려면 []로. (값 복사하니까) 수정 필요없이 readonly면 포인터로 
    mode_t mode = 0; 
    for(int i = 0; i < 9; i++){
        if(target[i] == s[i])  mode |= bits[i]; 
        else if(s[i] != '-') fprintf(stderr, "error"); 
    }
    return mode; 

    
}
