//2023014913, 김강민

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void print_path_to(ino_t);
void inode_to_name(ino_t, char *, int);

int main(){
    ino_t startNode =  get_inode("."); 
	print_path_to(startNode); // 현재 디렉토리로 getinode 전달 
	return 0;
}


//조건부 컴파일 !! 
#ifdef DEBUG
    //코드
#else 
    //코드2
#endif  // 조건부컴파일. 
// gcc 파일.c -DDEBUG -o 실행파일
// (-D가 조건부 컴파일 옵션)


void print_path_to(ino_t inode)
{
    int depth = 0; 
    char fullpath[20][256]; // recursive애 따라서 tree가 됨. 
    memset() // 쓰레기값 들어가는거 막고자 이거 사용하심. 
    char ist_name[256] // 으로 its_name에 적으심  이후 strncpy로 ith인덱스에 할당. 
	while (get_inode("..") != inode) 
	{
		chdir("..");
		inode_to_name(inode, fullpath[depth], 256); 
        printf("[%s] -> ", fullpath[depth]); 
        depth++; 
        inode = get_inode(".");
	}

	printf("/"); 
    printf("\n\n-------------------------\n"); 
    printf("Print working directory"); 
    printf("\n-------------------------\n"); 

    for(int i = depth - 1; i >=0 ; i--){
        printf("/%s", fullpath[i]);
    }
    printf("\n"); 

}

void inode_to_name(ino_t this_inode, char* namebuf, int buflen){
    DIR* dir_ptr = NULL; 
    struct dirent* dirent_ptr = NULL; 

    if((dir_ptr = opendir(".")) == NULL){
        perror(NULL); 
        exit(-1); 
    }

    while((dirent_ptr = readdir(dir_ptr)) != NULL){
        if(dirent_ptr->d_ino == this_inode){
            strncpy(namebuf, dirent_ptr->d_name, buflen);
            closedir(dir_ptr); 
            namebuf[buflen - 1] = '\0'; 
            return ; 
        }
    }
    fprintf(stderr, "error looking for inode: %llu\n", (long long int)this_inode);
	exit(1);

}

ino_t get_inode(char *filename)
{
	struct stat buf;
	if (stat(filename, &buf) == -1 )
	{
		fprintf(stderr, "cannot stat ");
		perror(NULL);
		exit(-1);
	}
	return buf.st_ino;
}

