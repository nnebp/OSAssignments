/*
 *  counter header
 *  Function prototypes, data, and constants for synchronized counter module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// SYNCHRONIZED COUNTER

// counter struct

typedef struct counter_t {
  int value;
  pthread_mutex_t lock;
} counter_t;

// counter methods

int CounterGet(counter_t *c);
    
void CounterInit(counter_t *c);

void CounterIncrement(counter_t *c);

void CounterDecrement(counter_t *c); 
