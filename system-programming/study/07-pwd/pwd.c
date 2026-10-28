#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h> 
#include <dirent.h>
#include <unistd.h>  
#include <string.h> 


ino_t get_inode(char *);
void print_path_to_recursive(ino_t);
void inode_to_name(ino_t, char *buf, int size);
void print_path_to_iterate(ino_t inode); 

int main(int ac, char* av[]){
    ino_t current_inode = get_inode(".");
    if(ac != 2) {
        fprintf(stderr, "err"); 
        exit(1); 
    }
    if(strcmp(av[1] , "-r") == 0){
        print_path_to_recursive(current_inode); 
    }else if((strcmp(av[1] , "-i") == 0)){
        printf("test\n"); 
        print_path_to_iterate(current_inode); 
    }
    printf("\n"); 
}

//재귀버젼 
void print_path_to_recursive(ino_t inode){
    char buf[256] = {0}; 

    if(get_inode("..") != inode){
        if(chdir("..") == -1){
            fprintf(stderr, "failed to go up "); 
            exit(1); 
        }
        inode_to_name(inode, buf, 256); 
        print_path_to_recursive(get_inode(".")); 
        printf("%s", buf);

    }
}  

void print_path_to_iterate(ino_t inode){
    char buffer[20][256] ; 
    char its_name[256] ={0}; 
    memset(buffer, 0, sizeof(buffer)); 
    int depth = 0; 

    while(get_inode("..") != inode){
        if(chdir("..") == -1){
            fprintf(stderr, "failed to go up "); 
            exit(1); 
        }
        inode_to_name(inode, its_name, 256); 
        strcpy(buffer[depth], its_name);
        depth++; 
        inode = get_inode(".");
    }


    printf("depth: %d\n" , depth);
    for(int i = depth-1; i >= 0 ; i--){
        printf("%s", buffer[i]); 
    }
}

void inode_to_name(ino_t inode, char *buf, int size){
    DIR* dir_ptr = NULL; 
    struct dirent* dirent_ptr; 

    if((dir_ptr = opendir(".")) == NULL){ // 이건 확실히 실수다. 올라온 곳에서 opendir(".")해줘야함. inode를 주는게 아님 !!! chdir("..")으로 올라온 곳(상위 디렉토리) 에서 opendir()후 readdir으로 순회하면서 자식의 inode를 찾는것 
        perror("err");
        exit(1); 
    }
    while((dirent_ptr = readdir(dir_ptr)) != NULL){
        if(dirent_ptr->d_ino == inode){
            snprintf(buf, size, "/%s", dirent_ptr->d_name); 
            closedir(dir_ptr); 
            return; 
        }
    }

    //끝까지 못찾는 경우에 대해서도 에러처리와 closedir해줘야함 !! 주의 
    closedir(dir_ptr); 
   

}

ino_t get_inode(char *filename){
    struct stat stat_info; 
    if(stat(filename, &stat_info) == -1){
        perror("err"); 
        exit(1); 
    }
    return stat_info.st_ino; 

}