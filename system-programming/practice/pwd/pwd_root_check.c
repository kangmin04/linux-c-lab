#include <stdio.h> 
#include <dirent.h> // 디렉토리에서 st_inode 통해서 d_name 가져옴 
#include <sys/stat.h> // stat 구조체에서 st_inode가쟈옴
#include <stdlib.h>  // exit
#include <unistd.h> // chdir 
#include <string.h>  // strncpy
#include <stdbool.h>

ino_t getInode(char* ); 
void searchDirName(ino_t); 

int main(){
    ino_t startInode = getInode("."); 
    if(startInode == getInode("..")){
        printf("/\n");
        return 0;
    }
    searchDirName(startInode); 
    printf("\n"); 
    return 0; 
}
ino_t getInode(char* path){
    struct stat s; 
    if(stat(path, &s) == -1){ 
        perror("stat"); 
        exit(1); 
    }
    return s.st_ino;
}

void searchDirName(ino_t inode){
    ino_t parentInode = getInode(".."); 
    if(inode == parentInode){
        return ;  
    }

    if((chdir("..")) == -1){
        perror("chdir"); 
        exit(1); 
    }
    DIR* dir =  opendir("."); 
    if(dir == NULL){
        fprintf(stderr, "error in openning current directory"); 
        exit(1); 
    }
    struct dirent* dir_pointer; 
    char foundname[256] = {0}; 

    while((dir_pointer = readdir(dir)) != NULL){
       
        if(dir_pointer->d_ino == inode){ // inode기준으로 이 디렉토리의 부모에서 자식들을 읽었을때, inode랑 같으면 그 이름이 자기 자신의 이름
            strncpy(foundname, dir_pointer->d_name, sizeof(foundname) -1); 
            break;         
        }
    }
    // while 루프가 끝났을때 교재코드에선 while loop 나오면 못찾은거니 error 처리해줬는데(별도 함수에서 node_name 찾는 로직해줌,)
    // 여기선 밑에 바로 searchDir으로 재귀로 들어가니 어케할지 고민이었다. 결국 dir_pointer의 값이 정답이었다. dir_pointer이 Null이면 그대로 나온것이니 이때만 error 처리해주면된다. 
    if(dir_pointer == NULL) {
        fprintf(stderr, "error looking for inode: %ld\n", (long int)inode);
	    exit(1);
    }
    closedir(dir); 

    searchDirName(getInode(".")); 
    printf("/%s", foundname);   
}
