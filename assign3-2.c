#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 100
#define NUM_THREAD 2

void *producer();
void *consumer();

FILE* pfile;
int index;
int j;
char song[SIZE][SIZE];

int main()
{
  int rc = 0, i = 0;
  int result[NUM_THREAD];
  pthread_t thread[NUM_THREAD];

  index = 0;
  j = 0;

  pfile = fopen("song.txt", "r");

  if(pfile != NULL) {

    while(!feof(pfile)) {
      fgets(song[j], sizeof(song[j]), pfile);
      j++;
    }
    fclose(pfile);
  }

  pthread_create(&thread[0], NULL, &producer, NULL);
  pthread_create(&thread[1], NULL, &consumer, NULL);

  for(i=0; i<NUM_THREAD; i++) {
    rc = pthread_join(thread[i], (void**)result[i]);
    if(rc != 0) {
      printf("Error in thread[%d] : %d\n", i, rc);
      exit(1);
    }
  }

  return 0;

}

void *producer()
{
  while(index < j) {
    printf("%d producer: %s\n", index, song[index++]);
    usleep(1000000);
  }
}

void *consumer()
{
  while(index < j) {
    printf("%d consumer: %s\n", index, song[index++]);
    usleep(1000000);
  }
}
