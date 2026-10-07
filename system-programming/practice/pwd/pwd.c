#include <stdio.h> 
#include <dirent.h> // 디렉토리에서 st_inode 통해서 d_name 가져옴 
#include <sys/stat.h> // stat 구조체에서 st_inode가쟈옴
#include <stdlib.h>  // exit
#include <unistd.h> // chdir 
#include <string.h>  // strncpy
#include <stdbool.h>

ino_t getInode(char* ); 
void searchDirName(ino_t, bool); 

int main(){
    bool first_run = true; 
    /*
        여기선 inode를 구하는 기준과 chdir(..)으로 올라가는 기준이 둘다 cwd라서 일치했음. 
    */
    ino_t startInode = getInode("."); 
    // if(startInode == )
    searchDirName(startInode, first_run); 
    printf("\n"); 
    return 0; 
}
ino_t getInode(char* path){
    struct stat s; 
    if(stat(path, &s) == -1){ // success : return 0 
        perror("stat"); 
        exit(1); 
    }
    return s.st_ino;
}

// void printPath(ino_t inode){
//     // first, check path's inode. 
//     // ino_t curInode = getInode(path);  // current . when i gave (".") as parameter, inode result : . 
//     ino_t parentInode = searchDirName(inode);  // 내 부모의 inode로 올라가서 인자로 준 inode(얘의 파일 이름을 찾으려는것.) 
//     printf("test: parentinode: %llu", (unsigned long long)parentInode); 
//     if(parentInode == inode){ // check parent node. whether it reached root.
//         printf("done\n"); 
//         return; 
//     }else{
//         searchDirName(parentInode); 
//     }

//     // printf("test: inode: %llu", (unsigned long long)curInode); 
//     //inode 를 

//     //second, readdir -> and retrive struct dirent. there is d_ino (type : ino_t) which tells Inode number
//     // if the inode is equal to what we been search for, get d_name (type : char 256)
// }


void searchDirName(ino_t inode, bool first_run){
    
    // since it's searching, i will find .. in here
    //1. move .. 
    ino_t parentInode = getInode(".."); 
    if(inode == parentInode){
        if(first_run){
            printf("/");
            return; 
        }
        return ;  
    }

    if((chdir("..")) == -1){
        perror("chdir"); 
        exit(1); 
    }

    //search dir in parent(which is . for now. )
    DIR* dir =  opendir("."); 
    if(dir == NULL){
        fprintf(stderr, "error in openning current directory"); 
        exit(1); 
    }

    //readdir while(inode == dirent.inode)
    struct dirent* dir_pointer; 
    char foundname[256] = {0}; 

    while((dir_pointer = readdir(dir)) != NULL){
        // printf("dirname: %s\n", dir_pointer->d_name); 
        // printf("---------------test------------------\n");
        if(dir_pointer->d_ino == inode){ // inode기준으로 이 디렉토리의 부모에서 자식들을 읽었을때, inode랑 같으면 그 이름이 자기 자신의 이름
            // printf("/%s\n", dir_pointer->d_name); 
            // return dir_pointer->d_ino; // 상위 디렉토리의 inode. 얠 다시 search에 넣어서 얘의 inode로 얘의 파일이름을 찾아야함. 
            strncpy(foundname, dir_pointer->d_name, sizeof(foundname) -1); 
            break; 
            // return  getInode("."); 
        }
    }
    // while 루프가 끝났을때 교재코드에선 while loop 나오면 못찾은거니 error 처리해줬는데(별도 함수에서 node_name 찾는 로직해줌,)
    // 여기선 밑에 바로 searchDir으로 재귀로 들어가니 어케할지 고민이었다. 결국 dir_pointer의 값이 정답이었다. dir_pointer이 Null이면 그대로 나온것이니 이때만 error 처리해주면된다. 
    if(dir_pointer == NULL) {
        fprintf(stderr, "error looking for inode: %ld\n", (long int)inode);
	    exit(1);
    }
    closedir(dir); 

    searchDirName(getInode("."), false); 
    printf("/%s", foundname);   
}
