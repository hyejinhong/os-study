#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 1000
#define NUM_THREAD 2

void *producer();
void *consumer();

int count = 0;    // thread 공유자원
int buffer[100];  // 환형 버퍼

int in = -1, out = -1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

int main()
{
  int result[NUM_THREAD];

  // 쓰레드 생성
  pthread_t thread[NUM_THREAD];
  pthread_create(&thread[0], NULL, &producer, NULL);
  pthread_create(&thread[1], NULL, &consumer, NULL);

  // 쓰레드 종류 대기
  pthread_join(&thread[0], (void**)result[0]);
  pthread_join(&thread[1], (void**)result[1]);

  return 0;
}

void *producer()
{
  int i;
  for(i=0; i<SIZE; i++) {
    pthread_mutex_lock(&mutex); // critical section 생성
    if (count == 100) { // 버퍼가 가득 찼다면..
      pthread_cond_wait(&buffer_has_space, &mutex); // lock을 해제해 소비자가 소비할 수 있도록 한다.
    }

    // 환형 버퍼에 데이터를 삽입
    in++;
    in %= 100;
    buffer[in] = i;
    count++;
    sleep(0.5);

    pthread_cond_signal(&buffer_has_data); // 버퍼에 데이터가 존재한다는 것을 알림
    printf("producer: [%d] 생산\n", count);
    pthread_mutex_unlock(&mutex); // lock 해제
  }
}

void *consumer()
{
  int i, data;
  for(i=0; i<SIZE; i++) {
    pthread_mutex_lock(&mutex); // critical section 생성
    if(count == 0) {  // 버퍼가 비어있는 경우
      pthread_cond_wait(&buffer_has_data, &mutex); // lock을 해제해 생산자가 생산할 수 있도록 한다.
    }

    // 환형 버퍼에서 데이터를 소비
    out++;
    out %= 100;
    data = buffer[out];
    count--;
    sleep(0.5);

    pthread_cond_signal(&buffer_has_space); // 버퍼에 공간이 있다는 것을 알림
    printf("consumer: [%d] 소비\n", count);
    pthread_mutex_unlock(&mutex); // lock 해제
  }
}
