#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREAD 2

void *myThread1();
void *myThread2();


int main()
{
    int num = 0;
    int rc = 0, i = 0;
    int result[NUM_THREAD];

    pthread_t thread[NUM_THREAD];
    pthread_create(&thread[0], NULL, &myThread1, (void *)&num);
    pthread_create(&thread[1], NULL, &myThread2, (void *)&num);

    for(i=0; i<NUM_THREAD; i++) {
      rc = pthread_join(thread[1], (void **)result[i]);
      if(rc != 0) {
        printf("Error in thread[%d] : %d\n", i, rc);
        exit(1);
      }
    }
    return 0;
}

void *myThread1() {
    while(1) {
      printf("This is thread1\n");
      usleep(1000000);
    }
}

void *myThread2() {
  while(1) {
    printf("This is thread2\n");
    usleep(2000000);
  }
}
