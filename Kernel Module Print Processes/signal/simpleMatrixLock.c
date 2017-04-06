#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define MAX 200
#define LOOPS 6555
#define NUMWORK 6
#define COLS 5
#define ROWS 5

//official stuff
int ** bigMatrix[MAX];


typedef struct counter_t {
  int value;
  pthread_mutex_t lock;
} counter_t;

//the counter TODO make official data structure
counter_t *prCount;
counter_t *coCount;
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

//COUNTER METHODS////////////////////////////////////////////////

  
int CounterGet(counter_t *c) {
  pthread_mutex_lock(&c->lock);
  int rc = c->value;
  pthread_mutex_unlock(&c->lock);
  return rc;
}
    
void CounterInit(counter_t *c) {
  c->value = 0;
  pthread_mutex_init(&c->lock, NULL);
}

void CounterIncrement(counter_t *c) {
  pthread_mutex_lock(&c->lock);
  c->value++;
  pthread_mutex_unlock(&c->lock);
}

void CounterDecrement(counter_t *c) {
  pthread_mutex_lock(&c->lock);
  c->value--;
  pthread_mutex_unlock(&c->lock);
}
  
//END COUNTER METHODS//////////////////////////////////////////////////


//START MATRIX FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////////////////


int ** AllocMatrix(int r, int c)
{
  int ** a;
  int i;
  a = (int**) malloc(sizeof(int *) * r);
  assert(a != 0);
  for (i = 0; i < r; i++)
  {
    a[i] = (int *) malloc(c * sizeof(int));
    assert(a[i] != 0);
  }
  return a;
}

void FreeMatrix(int ** a, int r, int c)
{
  int i;
  for (i=0; i<r; i++)
  {
    free(a[i]);
  }
  free(a);
}

void GenMatrix(int ** matrix, const int height, const int width)
{
  int i, j;
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      int * mm = matrix[i];
      //matrix[i * width + j] = rand() % 10;
      mm[j] = rand() % 10;
#if OUTPUT
      printf("matrix[%d][%d]=%d \n",i,j,mm[j]);
#endif
    }
  }
}

int SumMatrix(int ** matrix, const int height, const int width)
{
  int sum = 0;
  int i, j;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
    {
      sum += matrix[i][j];
#if OUTPUT
      printf("[%d][%d]--%d x=%d ele=%d\n",i,j,mm[j],x,ele);
#endif
    }
  return sum; 
}
//END MATRIX FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////////////////

int put() {
  int **mat = AllocMatrix(ROWS, COLS);
  GenMatrix(mat, ROWS, COLS);
  int sum = SumMatrix(mat ,ROWS , COLS);
  
  bigMatrix[fill_ptr] = mat;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;

  return sum;
}

int get() {
  int sum = SumMatrix(bigMatrix[use_ptr], ROWS, COLS);
  FreeMatrix(bigMatrix[use_ptr], ROWS, COLS);

  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return sum;
}

void *producer(void *arg) {
  
  //char *name = arg;
  //printf("name: %c\n", name);
  
  int producerSum = 0;//malloc(sizeof(int));
 

    //while(producerTotal < LOOPS) {
  while(CounterGet(prCount) < LOOPS) {

     pthread_mutex_lock(&mutex); 
    while (count == MAX) {
      //if(producerTotal == LOOPS) {
      if(CounterGet(prCount) == LOOPS) {
        break;
      }     
      pthread_cond_wait(&empty, &mutex); 
    }
    if (CounterGet(prCount) < LOOPS) {
      producerSum += put();
      //producerTotal++;
      CounterIncrement(prCount);

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

  //while(consumerTotal < LOOPS) {
  while(CounterGet(coCount) < LOOPS) {

    pthread_mutex_lock(&mutex); 
    while (count == 0) {
      //if(consumerTotal == LOOPS) {
      if(CounterGet(coCount) == LOOPS) {

        break;
      }
        pthread_cond_wait(&fill, &mutex);
    }
    //if (consumerTotal < LOOPS) {
    if (CounterGet(coCount) < LOOPS) {

      int tmp = get(); 
      consumerSum += tmp;

      //consumerTotal++;
      CounterIncrement(coCount);
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
  
  int producerSumGlobal = 0;
  int consumerSumGlobal = 0;
  
  int prReturn[NUMWORK];
  int coReturn[NUMWORK];
  
  int i = 0;
  pthread_t prThread[NUMWORK];
  pthread_t coThread[NUMWORK];
  
  //Counter INIT
  prCount = malloc(sizeof(counter_t));
  coCount = malloc(sizeof(counter_t));

  CounterInit(prCount);
  CounterInit(coCount);


  
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

  
  
  
  //free counters
  free(prCount);
  free(coCount);
  
  
  
  
  
  
  
  
  
  
  return 0;
}
