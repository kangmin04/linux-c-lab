#include <stdio.h> 
#include <sys/stat.h> //S_ISDIR(mode)
#include <dirent.h> 
#include <stdlib.h> 
#include <unistd.h>  // read write, close, chdir
#include <string.h> 

void print_path_to(ino_t inode); 
void find_directory_name(ino_t inode, char* , int );
ino_t get_inode(char* pathname);
void print_absolute_path(char* path);
void do_ls(char* dirname); 
void do_lstat(char* parent_dirname, char* child_dirname); 

int main(int argc, char* argv[]){
    if(argc == 1){
        // do_ls("."); 
        return 0;
    }
    char* path = argv[1]; 
    //절대경로 출력하기. 
    print_absolute_path(path); 
    do_ls("/home/kangmin/lab/system-programming/assignments"); 
    return 0; 
}


void do_ls(char* dirname){
    DIR* dir_ptr; 
    struct dirent* dirent_ptr; 

    if((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "failed to open directory"); 
        exit(1); 
    }

    while((dirent_ptr = readdir(dir_ptr)) != NULL){
        // stat구해서 st_mode 를 S_IFDIR에 넣는것. 
        if((strcmp(dirent_ptr->d_name, ".") == 0 ) || (strcmp(dirent_ptr->d_name, "..") == 0)) continue; 
        // printf("After do_lstat(%s, %s)\n", dirname, dirent_ptr->d_name);
        do_lstat(dirname, dirent_ptr->d_name); 
       
    }
    closedir(dir_ptr); 
}

void do_lstat(char* parent_dirname, char* child_dirname){   
    struct stat stat_buffer; 
    char directory_name[512]; 
    snprintf(directory_name, 512, "%s/%s",  parent_dirname, child_dirname );
    if(lstat(directory_name, &stat_buffer) == -1) {
        fprintf(stderr, "lstat error"); 
        exit(1); 
    }
    
    //check whether directory or not
    if(S_ISDIR(stat_buffer.st_mode)){ // 이거 계속 틀리는데 주의하자 !!! . 구조체가 포인턴지 그냥 변순지 잘 파악 
        // directory
        do_ls(directory_name);
        printf("----%s", child_dirname); 

    }

    //non-directory

}
void print_absolute_path(char* path){
    ino_t current_inode = get_inode(path); 
    print_path_to(current_inode); 
    printf("\n");
}

void print_path_to(ino_t inode){
    //root체크
    if(inode == get_inode("..")){
        return; 
    }
    char name[256]; 
    chdir(".."); 
    find_directory_name(inode, name, 256); 
    print_path_to(get_inode(".")); 
    printf("/%s" , name); 
}

void find_directory_name(ino_t inode, char* buf, int buffersize){
    //한칸 올라간상태임.!!! 
    struct dirent* dirent_ptr;
    DIR* dir_ptr = opendir("."); 
    if(dir_ptr == NULL) {
        fprintf(stderr, "failed to open directory"); 
        exit(1); 
    }

    while((dirent_ptr = readdir(dir_ptr)) != NULL){
        if(dirent_ptr->d_ino == inode){
            strncpy(buf, dirent_ptr->d_name, buffersize); 
            //PLEASE DONT FORGET TO CLOSE UR DIRECTORY. 
            closedir(dir_ptr); 
            buf[buffersize-1] = '\0'; 
            return ;  
           
        }
    }
    closedir(dir_ptr); 
    //올라간 디렉토리에 찾고자 한 inode가 없는경우
    fprintf(stderr, "Something wrong happends. there is no . that u are looking for: (inode)\n"); 
    exit(1); 
}


ino_t get_inode(char* pathname){
    struct stat stat_buf; 
    if(lstat(pathname, &stat_buf) == -1){
        perror("stat error"); 
        exit(1); 
    }
    return stat_buf.st_ino; //stat_buf IS NOT POINTER
}