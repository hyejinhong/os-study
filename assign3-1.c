#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 100

int main()
{
  char buffer[SIZE];
  int fd[2];

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
  else if(pid == 0) { // 자식 프로세스
    write(fd[1], "This letter is from child", SIZE);
    sleep(1);
    read(fd[0], buffer, SIZE);
    printf("Output of child process: %s\n\n", buffer);
  }
  else {  // 부모 프로세스
    read(fd[0], buffer, SIZE);
    printf("Output of parent process: %s\n\n", buffer);
    write(fd[1], "This letter is from parent", SIZE);
  }

  exit(0);
}
