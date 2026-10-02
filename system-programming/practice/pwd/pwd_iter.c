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
    // if(startNode == get_inode("..")){
    //     printf("/\n"); 
    //     return 0; 
    // }
	print_path_to(startNode);
	

	return 0;
}


void print_path_to(ino_t inode)
{
    int depth = 0; 
    char fullpath[20][256]; 
    /*
        즉, while 반복을 돌리면서 
        1. 상위로 한 디렉토리 올라감. 
        2. 현재 inode를 상위 디렉토리에서 찾고 그 이름을  fullpath 배열에 0부터 저장함. (depth로 ++해주며 칸 세기)
        3. 
    */

	while (get_inode("..") != inode) // 여기서 inode는 현재 내 디렉토리 = 내가 찾고자하는 디렉토리
	{
        
        // printf(".의 inode : %llu\n", (long long int)get_inode(".")); 
        // printf("..의 inode : %llu\n", (long long int)get_inode("..")); 
		chdir(".."); // 여기서 한단계 올라옴 -> practice로 올라감.   // 이 practice에서 내 pwd(inode)를 찾는게 과제
        //한칸 올라온 곳에서 inode는 기존(startnode. 바로 직전단계의 inode를 구하는것임) 
        // printf("chdir 하고 난후 .의 inode : %llu\n", (long long int)get_inode(".")); 
        // printf("chdir 하고 난 후 ..의 inode : %llu\n", (long long int)get_inode("..")); 
		inode_to_name(inode, fullpath[depth], 256); // fullpath[depth]번쨰에 dirent->d_name 저장됨

        printf("[%s] -> ", fullpath[depth]); 
        depth++; 
        //여기까지도 현재 내 inode는 startnode. 즉. pwd의 inode임. 
        //
        //이후 한단계 올라감. 이젠 system-programming에서 practice를 찾을 차례. 
        // 즉, 내 
        inode = get_inode("."); //한칸 올라온 곳에서 inode를 찾는게 우리의 과제 
        // printf("내가 inode = get_inode(.)로 읽은 값: %llu\n", (long long int)get_inode(".")); 
        // printf("내가 inode = get_inode(..)로 읽은 값: %llu\n", (long long int)get_inode("..")); 
		
	}

	printf("/"); 
    printf("\n\n-----------------------\n"); 
    printf("Print working directory"); 
    printf("\n-----------------------\n"); 

    printf("/");
    for(int i = depth - 1; i >=0 ; i--){
        printf("%s/", fullpath[i]);
    }

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
            //char *strncpy(char *restrict buf, const char *restrict src, size_t size)
            // 만약 src가 size 보다 작은 경우엔 buf안에 src넣고 남은 크기는 다 null 넣어줌. 
            // src > size인 경우엔 buf가 src로 꽉 참. 별도의 null이 없어서 마지막에 null 표시해주는 것. 
            // 혹은 char buf[256] = {0}으로 초기화하고 strncpy(buf, src, sizeof(buf) -1 )로 하면 마지막은 항상 0으로 유지
            strncpy(namebuf, dirent_ptr->d_name, buflen);
            // printf("현재 폴더: %s\n", namebuf);
            // printf("현재폴더의 inode : %llu\n", (long long int)dirent_ptr->d_ino); 
            closedir(dir_ptr); 
            namebuf[buflen - 1] = '\0'; 
            return ; 
        }

    // 원래 여기에 선언했엇음 개미친놈 걍. ㅋ. ㅋㅋㅋ
    }
    //진짜개시발미친놈 ㅋㅋ 계속 while 문이 오륜줄 알고뒤져봤는데 fprintf를 while 내에서 한거였음. pwd는 그냥 맨처음 돌았기에 넘어갔던거고 
    fprintf(stderr, "error looking for inode: %llu\n", (long long int)this_inode);
	exit(1);

}

ino_t get_inode(char *filename)
{
	struct stat buf;
    // int stat(const char* path, struct stat* statbuf);
	if (stat(filename, &buf) == -1 )
	{
		fprintf(stderr, "cannot stat ");
		perror(NULL);
		exit(-1);
	}
	return buf.st_ino;
}

