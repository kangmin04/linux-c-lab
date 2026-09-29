#include <stdio.h> 
#include <dirent.h> 
#include <sys/stat.h> // stat, lstat, 파일검사매크로(S_ISREG(mode), S_ISDIR(mode)), 권한 매트로(S_IRUSR, S_IRGRP)
#include <string.h> //strcpy
#include <pwd.h> 
#include <grp.h>
#include <time.h>
#include <limits.h>

void do_ls(char[]);
void do_stat(char *, char* );
void mode_to_letter(mode_t mode, char mode_str[11]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);
void show_file_info(char *, struct stat *);
// DIR* opendir(const char *name); 
// struct dirent *readdir(DIR *dirp); 
/* 
    readdir()가 NULL을 반환하는 경우는 두 가지다 — ① 디렉토리 끝에 정상적으로 도달했을 때, ② 실제 에러(예: 잘못된 DIR *, I/O 오류)가 발생했을 때. 리턴값(NULL)만 봐서는 이 둘을 구분할 수
    eaddir()는 정상적으로 끝에 도달했을 때는 errno를 건드리지 않으므로, 호출 전에 errno = 0으로 직접 초기화해두고 NULL을 받은 뒤 errno가 여전히 0이면 정상 종료, 0이 아니면(예: EBADF) 진짜 에러로 판단해야 한다.
*/

// C에서 !=는 =보다 우선순위가 높습니다

// 우리가 hello.txt라는 파일 하나를 만들면, 디스크에는 두 가지가 따로 저장됩니다.

//   - inode (몸통): 파일의 실제 정보입니다. 크기, 권한, 소유자, 수정 시각, 그리고 내용이 디스크의 어디에 있는지가 적혀 있습니다. 각 inode에는 번호가 붙습니다(예: 1234번).
//   - 이름 (명찰): hello.txt라는 문자열입니다. 이름은 inode 안에 없고, 디렉토리 안에 저장됩니다.
//   파일을 복사하지 않습니다. 디렉토리 목록에 같은 inode 번호를 가리키는 이름을 한 줄 더 추가
//   한쪽으로 내용을 고치면 다른 쪽에서도 바뀐 내용이 보임. 
//   하드링크는  디렉토리에는 걸 수 없습니다
//   심볼릭 링크는 자기만의 inode를 가진 별도의 작은 파일입니다. 그리고 그 내용은 "hello.txt"라는 경로 문자열뿐

void do_ls(char dirname[]){
    DIR * dir_ptr; 
    struct dirent *pdirent; 
    char path[PATH_MAX]; 

    if((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "ls: cannot open %s\n", dirname);  
        return; 
    }
    //기존 코드의 문제: opendir로 인자로 준 test디렉토리를 염. 그후 readdir까지 함. 이후 pdir->d_name으로 a.txt구함. a.txt를 바로 do_stat하는데 이때 stat은 현재 작업중인 디렉토리를 기준으로 파일/디렉토리의 상세정보를 탐색함. 
    while(((pdirent = readdir(dir_ptr) )!= NULL)){
        // if(pdirent->d_name[0] == '.') continue; // . .. 파일들 숨김
        // do_stat(pdirent->d_name);
        snprintf(path, sizeof path, "%s/%s", dirname, pdirent->d_name); 
        printf("[DEBUG] path : %s\n", path); 
        do_stat(path, pdirent->d_name);  
    }

    closedir(dir_ptr); 
}
// int stat(const char *pathname, struct stat *buf);
// int lstat(const char *pathname, struct stat *buf);
// 성공하면 0을 반환하며 두 번째 인자로 넘긴 buf가 채워지고, 실패하면 -1을 반환하며 errno가 설정된다

void do_stat(char* path, char* filename){
    struct stat buf; // 구조체 크기는 시스템 헤더가 정함. 
    // printf("filename: %s\n", filename); 
    if(stat(path, &buf) == -1){ //stat을통해 filename 경로 따라가며 상태를 얻음. 
        //perror는 전역변수 errno에 설정된 오류코드를 사람이 읽기 쉬운 표준오류 메시지로 출력. filename인자로 주면 먼저 filname: 출력하고 이후 error 값에 대응하는 시스템 오류 msg 출력 
        printf("대체왜\n");
        perror(path); 
    }else {
        show_file_info(filename, &buf); 
    }
}

void show_file_info(char* filename, struct stat* info_pointer){
    char mode[11];  // 이거 10으로 하려하니까 accessing 11bytes in a region of size 10이라고 함. 확인해봐야할듯. 
    mode_to_letter(info_pointer->st_mode, mode); 

    /*
        printf 서식
        - : 왼쪽 정렬 
        %-8 : 최소 8칸 차지하고 , 모자라면 공백으로 채우란것. 
    */
    printf("%s", mode);
	printf("%4d ", (int)info_pointer->st_nlink);
	printf("%-8s ", uid_to_name(info_pointer->st_uid));
	printf("%-8s ", gid_to_name(info_pointer->st_gid));
	printf("%8ld ", (long)info_pointer->st_size);
	printf("%.12s ", 4 + ctime(&info_pointer->st_mtime));
	printf("%s\n", filename);
}


/* 매크로 조합하여 타입1칸+ 권한9칸 + 여유1칸 만듦 */
void mode_to_letter(mode_t mode, char str[11]){
    //1. 11비트를 -로 초기화. 
    strcpy(str, "----------"); // "aa" 는 사실 3바이트임. (끝에 컴파일러가 자동으로 \0추가해줌. 즉 --dash는 10개만 줘야함. 내가 11개를 dash로 추면 str은 11바이트인데 우리가 넣을값은 11+ 1(널)로 12 바이트가 저장됨. 버퍼 오버플로우 )
    /* 
        sizeof(str) : 메모리에서 차지하는 바이트수. 널포함. 11
        strlen(str) : 널 앞까지의 글자수 10 
        sizeof/strlen은 둘다 printf 시 zu로 받음
    */
    
    if(S_ISDIR(mode)) str[0] = 'd'; 
    else if (S_ISLNK(mode)) str[0] = 'l'; // 심볼릭링크
    else if (S_ISCHR(mode)) str[0] = 'c'; // 문자장치파일
    else if (S_ISBLK(mode)) str[0] = 'b'; // 블록장치파일 

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

// st_uid, st_gid 모두 그냥 정수임 사용자명으로 바꾸려면 사용자 db 조회하는 함수 필요함. 
// struct passwd *getpwuid(uid_t uid);   // uid -> 사용자 정보 
// struct group  *getgrgid(gid_t gid);   // gid -> 그룹 정보

char* uid_to_name(uid_t uid){
    struct passwd *pw_ptr; 
    static char numstr[16]; //지역이 아니라 static으로 선언해야만 함
    // static 없이 char numstr[16]선언 시 지역배열은 스택에 있어서 함수가 return 하는 순간 그 공간이 무효가 됨. 무효가 된 주소를 반환하면 호출자가 읽을 때쯤엔
    // 다른 함수 호출이 그 자리를 덮어쓴 뒤일수도 있음. 
    //static 붙이면 배열이 스택이 아닌 데이터영역에 한번만 만들어지고, 프로그램이 끝날때까지 살아있음. 그래서 주소 밖으로 반환해도 안전함. 

    if((pw_ptr = getpwuid(uid)) == NULL){ //getpwuid() : uid를 struct passwd로 반환
        sprintf(numstr, "%u", uid); 
        return numstr; 
    }

    return pw_ptr->pw_name; 
} 

char* gid_to_name(gid_t gid){
    struct group *gr_ptr ; 
    static char numstr[16]; 

    if((gr_ptr = getgrgid(gid)) == NULL){
        sprintf(numstr, "%u", gid); 
        return numstr; 
    }
    return gr_ptr->gr_name; 
}
int main(int argc, char* argv[]){
    if(argc == 1){
        do_ls("."); 
        printf("ITIS CURRENT DIRECTORY RESULT\n");
    }else{
        while(--argc){
            printf("%s: \n", *++argv);
			do_ls(*argv);  
        }
    }

    return 0; 
}