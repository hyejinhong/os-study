#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 100

int main()
{
  char buffer[SIZE];
  char song[SIZE][SIZE]; // 텍스트 파일 읽어와서 줄별로 저장할 배열
  int fd[2]; // fd[0]: 읽기, fd[1]: 쓰기
  int j = 0;
  int pindex = 0; // 부모 프로세스가 출력할 index
  int cindex = 1; // 자식 프로세스가 출력할 index
  FILE* pfile;

  pfile = fopen("song.txt", "r");

  // 텍스트 파일 읽어와서 배열에 저장
  if(pfile != NULL) {
    while(!feof(pfile)) {
      fgets(song[j], sizeof(song[j]), pfile);
      j++;
    }
    fclose(pfile);
  }

  if(pipe(fd) == -1) {
    perror("pipe error\n");
    exit(0);
  }

  pid_t pid;
  pid = fork();

  if(pid == -1) {
    perror("fork error\n");
    exit(0);
  }
  // 자식 프로세스
  else if(pid == 0) {
    // 자식 프로세스에서는 cindex번째 줄을 부모 프로세스로 전송하고,
    // 부모 프로세스에서 전송한 가사를 출력한다.
    while(cindex < j) {
      write(fd[1], song[cindex], SIZE);
      cindex += 2;
      sleep(1);
      read(fd[0], buffer, SIZE);
      printf("%d child: %s\n", cindex, buffer);
    }
  }
  // 부모 프로세스
  else {
    // 부모 프로세스에서는 pindex번째 줄을 부모 프로세스로 전송하고,
    // 자식 프로세스에서 전송한 가사를 출력한다.
    while(pindex < j) {
      write(fd[1], song[pindex], SIZE);
      pindex += 2;
      sleep(1);
      read(fd[0], buffer, SIZE);
      printf("%d parent: %s\n", pindex, buffer);
    }
  }
  exit(0);
}
