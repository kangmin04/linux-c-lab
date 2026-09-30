#include <stdio.h> 
#include <dirent.h> // 디렉토리에서 st_inode 통해서 d_name 가져옴 
#include <sys/stat.h> // stat 구조체에서 st_inode가쟈옴
#include <stdlib.h>  // exit
#include <unistd.h> // chdir 
#include <string.h>  // strncpy

ino_t getInode(char* ); 
// void printPath(ch); 

// void printPath(ino_t );
// ino_t searchDirName(ino_t); 
void searchDirName(ino_t); 

int main(){
    ino_t startInode = getInode("."); 
    searchDirName(startInode); 
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


void searchDirName(ino_t inode){
    // since it's searching, i will find .. in here
    //1. move .. 
    ino_t parentInode = getInode(".."); 
    if(inode == parentInode){
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

    closedir(dir); 

    searchDirName(getInode(".")); 
    printf("/%s", foundname);   
}
