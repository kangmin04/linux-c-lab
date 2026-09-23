// filestat.c 핵심 부분
#include<dtdio.h> 


struct stat infobuf;
stat(argv[1], &infobuf);

printf("mode: %o\n", infobuf.st_mode);         // 8진수로 출력해야 비트가 눈에 들어온다
printf("links: %ld\n", infobuf.st_nlink);
printf("user: %d\n", infobuf.st_uid);          // 아직 숫자 그대로 (이름 변환은 다음 절)
printf("group: %d\n", infobuf.st_gid);
printf("size: %ld\n", infobuf.st_size);
printf("last modification time: %ld\n", infobuf.st_mtime);  // 아직 raw 타임스탬프