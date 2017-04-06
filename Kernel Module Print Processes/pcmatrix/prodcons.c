/*
 *  prodcons module
 *  Producer Consumer module
 * 
 *  Implements routines for the producer consumer module based on 
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

// Producer consumer data structures
int ** bigMatrix[MAX];
int fill_ptr = 0;
int use_ptr  = 0;
int count    = 0;
// Producer consumer locks and conditions
pthread_cond_t empty = PTHREAD_COND_INITIALIZER; 
pthread_cond_t fill = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
// PRODUCER-CONSUMER put() get()
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

// PRODUCER - return total of all arrays produced
void *prod_worker(void *arg)
{
  int producerSum = 0;
  while(CounterGet(prCount) < LOOPS) {

     pthread_mutex_lock(&mutex); 
    while (count == MAX) {
      if(CounterGet(prCount) == LOOPS) {
        break;
      }     
      pthread_cond_wait(&empty, &mutex); 
    }
    if (CounterGet(prCount) < LOOPS) {
      producerSum += put();
      CounterIncrement(prCount);

    }
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
  
  return producerSum;
}

// CONSUMER - return total of all arrays consumed
void *cons_worker(void *arg)
{
  int consumerSum = 0;

  while(CounterGet(coCount) < LOOPS) {

    pthread_mutex_lock(&mutex); 
    while (count == 0) {
      if(CounterGet(coCount) == LOOPS) {

        break;
      }
        pthread_cond_wait(&fill, &mutex);
    }
    if (CounterGet(coCount) < LOOPS) {

      int tmp = get(); 
      consumerSum += tmp;

      CounterIncrement(coCount);
    }
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&mutex); 
  }

   return consumerSum; 
}
