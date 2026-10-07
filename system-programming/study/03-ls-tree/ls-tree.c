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
void do_ls(char* dirname, int depth); 
void do_lstat(char* parent_dirname, char* child_dirname, int depth); 

int main(int argc, char* argv[]){
    if(argc == 1){
        do_ls(".", 0); 
        return 0;
    }
    //절대경로 출력하기. 
    print_absolute_path(argv[1]); 
    do_ls(argv[1], 1); 
    return 0; 
}

// do_ls에 depth 정보넣기 .
void do_ls(char* dirname, int depth){
    DIR* dir_ptr = NULL; 
    struct dirent* dirent_ptr = NULL; 

    if((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "failed to open directory"); 
        exit(1); 
    }

    while((dirent_ptr = readdir(dir_ptr)) != NULL){
        // stat구해서 st_mode 를 S_IFDIR에 넣는것. 
        if((strcmp(dirent_ptr->d_name, ".") == 0 ) || (strcmp(dirent_ptr->d_name, "..") == 0)) continue; 
        // printf("After do_lstat(%s, %s)\n", dirname, dirent_ptr->d_name);
        do_lstat(dirname, dirent_ptr->d_name, depth); 
       
    }
    closedir(dir_ptr); 
}

void do_lstat(char* parent_dirname, char* child_dirname, int depth){   
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
        for(int i = 0; i < depth ; i++){
            printf("----"); 
        }
        printf("%s\n", child_dirname); 
        printf("        "); 
        do_ls(directory_name, depth++);
       

    }

    //non-directory

}
void print_absolute_path(char* path){
    /*
        여기선 inode를 구하는 기준과 chdir(..)으로 올라가는 기준이 둘다 cwd라서 일치했음. 
        반면 여기선 argv[1]과 내 현재 폴더가(cwd)가 다를수있음 !!!!!
        argv[1]가 다른 디렉토리인 경우 여기로 chdir을 진행 한 후 inode를 구해야함. 

    */
    if(chdir(path) == -1){
        perror("chdir"); 
        exit(1); 
    }
    ino_t current_inode = get_inode("."); 
    print_path_to(current_inode); 
    printf("\n");
}

void print_path_to(ino_t inode){
    //root체크
    if(inode == get_inode("..")){
        return; 
    }
    char name[256]; 
    if( chdir("..") == -1){
        perror("chdir"); 
        exit(1); 
    }
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

    while(((dirent_ptr = readdir(dir_ptr))) != NULL){
        if(dirent_ptr->d_ino == inode){
            strncpy(buf, dirent_ptr->d_name, buffersize); 
            //PLEASE DONT FORGET TO CLOSE UR DIRECTORY. 
            closedir(dir_ptr); 
            buf[buffersize-1] = '\0'; 
            return ;  
           
        }
        // else{
        //     printf("[debug] There is no %s that match to what we are looking for\n", dirent_ptr->d_name); 
        // }
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