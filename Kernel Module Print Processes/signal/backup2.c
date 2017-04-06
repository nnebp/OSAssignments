#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


#define MAX 100
#define LOOPS 10000
#define NUMWORK 9222

//the counter TODO make official data structure
int producerTotal = 0;
int consumerTotal = 0;

int buffer[MAX];
int fill_ptr = 0;
int use_ptr  = 0;
int count    = 0;
int producerSum = 0;
int consumerSum = 0;
int loops    = LOOPS;

pthread_cond_t empty = PTHREAD_COND_INITIALIZER; 
pthread_cond_t fill = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void put(int value) {
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get() {
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}



void *producer(void *arg) {
  
  //char *name = arg;
  //printf("name: %c\n", name);
  
  int producerSum = 0;//malloc(sizeof(int));
  while(producerTotal < LOOPS) {
    pthread_mutex_lock(&mutex); 
    while (count == MAX) {
      if(producerTotal == LOOPS) {
        break;
      }
      pthread_cond_wait(&empty, &mutex); 
    }
    if (producerTotal < LOOPS) {
      int random = rand();
      //put(producerTotal);
      //producerSum += producerTotal;

      put(random); 
      producerSum += random;
      producerTotal++;

    }
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
  
      //printf("Producer TOTAL: %d NAME:%c\n", producerTotal, name);
  return producerSum;
}

void *consumer(void *arg) {
  int consumerSum = 0;//malloc(sizeof(int));

  
  //char *name = arg;
  //printf("name: %c\n", name);

  while(consumerTotal < LOOPS) {

    pthread_mutex_lock(&mutex); 
    while (count == 0) {
      if(consumerTotal == LOOPS) {
        break;
      }
        pthread_cond_wait(&fill, &mutex);
    }
    if (consumerTotal < LOOPS) {
      int tmp = get(); 
      consumerSum += tmp;

      consumerTotal++;
      //printf("TOTAL: %d NAME:%c\n", consumerTotal, name);

      
    }
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&mutex); 
    //printf("%d\n", tmp);
  }
      //printf("Consumer TOTAL: %d NAME:%c\n", consumerTotal, name);

   return consumerSum; 
}


//pcmatrix.c
int main() {
/*
  int producerSumGlobal = 0;
  int producerSumGlobal2 = 0;
  int producerSumGlobal3 = 0;

  
  int consumerSumGlobal = 0;
  int consumerSumGlobal2 = 0;
  int consumerSumGlobal3 = 0;


  pthread_t prThread, prThread2, prThread3;
  pthread_t coThread, coThread2, coThread3;
  
  char *prodA = 'a';
  char *prodB = 'b';
  char *prodC = 'c';

  
  pthread_create(&prThread, NULL, producer, prodA);      
  pthread_create(&prThread2, NULL, producer, prodB);
  pthread_create(&prThread3, NULL, producer, prodC);
  
  pthread_create(&coThread, NULL, consumer, prodA);
  pthread_create(&coThread2, NULL, consumer, prodB);
  pthread_create(&coThread3, NULL, consumer, prodC);
  
  pthread_join(prThread, &producerSumGlobal);
  pthread_join(prThread2, &producerSumGlobal2);
  pthread_join(prThread3, &producerSumGlobal3);
  
  pthread_join(coThread, &consumerSumGlobal);
  pthread_join(coThread2, &consumerSumGlobal2);
  pthread_join(coThread3, &consumerSumGlobal3);

  
  printf("producer: %d\nconsumer: %d", producerSumGlobal + producerSumGlobal2 + producerSumGlobal3, consumerSumGlobal + consumerSumGlobal2 + consumerSumGlobal3);  
*/
  
  int producerSumGlobal = 0;
  int consumerSumGlobal = 0;
  
  int prReturn[NUMWORK];
  int coReturn[NUMWORK];
  
  int i = 0;
  pthread_t prThread[NUMWORK];
  pthread_t coThread[NUMWORK];
  
  //seed rand
  srand(time(NULL)); 
  
  for(i = 0; i < NUMWORK; i++) {
    pthread_create(&prThread[i], NULL, producer, NULL); 
    pthread_create(&coThread[i], NULL, consumer, NULL);
  }
  
  for(i = 0; i < NUMWORK; i++) {
    pthread_join(prThread[i], &prReturn[i]); 
    pthread_join(coThread[i], &coReturn[i]); 
  }

  for (i = 0; i < NUMWORK; i++) {
    producerSumGlobal += prReturn[i];
    consumerSumGlobal += coReturn[i];
  }
  
  printf("producer: %d\nconsumer: %d\n", producerSumGlobal,
        consumerSumGlobal);

  /**
  Multi thread creation
  for i ... 10 {
    array[i] = thread
  }
  
  for i ..10 {
    join (array[i])
  }
  */
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  return 0;
}
