#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 1000
#define NUM_THREAD 2

void *producer();
void *consumer();

int buffer[100];
sem_t mutex;    // 공유변수 상호배제를 위해 사용
sem_t full;     // 생산자 입장에서 볼 때 n개의 빈 셀이 없어지면 full
sem_t empty;    // 소비자 입장에서 볼 때 자료가 있는 셀이 0개이면 empty

int main()
{
  // 세마포어 초기화
  sem_init(&mutex, 0, 1);   // 초기화 value: 1
  sem_init(&full, 0, 100);  // 초기화 value: 100
  sem_init(&empty, 0, 0);   // 초기화 value: 0

  // 쓰레드 생성
  pthread_t thread[NUM_THREAD];

  pthread_create(&thread[0], NULL, &producer, NULL);
  pthread_create(&thread[1], NULL, &consumer, NULL);

  // 쓰레드 종료 대기
  pthread_join(&thread[0], NULL);
  pthread_join(&thread[1], NULL);

  return 0;
}

void *producer()
{
  int i, temp;
  for(i=0; i<SIZE; i++) {
    sem_wait(&full);  // 버퍼가 꽉 찼는지 확인
    sem_wait(&mutex); // lock을 건다.

    // 환형 버퍼에 데이터 삽입
    temp = i%100;
    buffer[temp] = i;

    sem_post(&mutex); // lock을 해제한다.
    sem_post(&empty); // 버퍼에 데이터가 있음을 알린다.

    printf("producer: %d\n", i);
  }
}

void *consumer()
{
  int i, data, temp;
  for(i=0; i<SIZE; i++) {
    sem_wait(&empty); // 버퍼가 비어있는지 확인
    sem_wait(&mutex); // lock을 건다.

    // 환형 버퍼에서 데이터 소비
    temp = i%100;
    data = buffer[temp];

    sem_post(&mutex); // lock을 해제한다.
    sem_post(&full);  // 버퍼에 공간이 있음을 알린다.
    
    printf("consumer: %d\n", data);
  }
}
