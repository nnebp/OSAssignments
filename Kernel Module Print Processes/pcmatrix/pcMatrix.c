/*
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 * 
 *  Creates producer and consumer threads and launches them to produce matrices
 *  and consume matrices.  Each thread produces a total sum of the value of
 *  randomly generated elements.  Producer sum and consumer sum must match.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int main (int argc, char * argv[])
{
  time_t t;
  int numw = NUMWORK;

  // Seed the random number generator with the system time
  srand((unsigned) time(&t));
  
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

  //create threads
  for(i = 0; i < NUMWORK; i++) {
    pthread_create(&prThread[i], NULL, prod_worker, NULL); 
    pthread_create(&coThread[i], NULL, cons_worker, NULL);
  }
  //wait for threads to finish
  for(i = 0; i < NUMWORK; i++) {
    pthread_join(prThread[i], &prReturn[i]); 
    pthread_join(coThread[i], &coReturn[i]); 
  }

  //add up all the sums
  for (i = 0; i < NUMWORK; i++) {
    producerSumGlobal += prReturn[i];
    consumerSumGlobal += coReturn[i];
  }
  
  printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
  printf("Using a shared buffer of size=%d\n", MAX);
  printf("With %d producer and consumer threads.\n",numw);

  printf("Produced = Consumed --> %d = %d\n",producerSumGlobal,consumerSumGlobal);

  //free counters
  free(prCount);
  free(coCount);

  return 0;
}
