/*
 *  prodcons header
 *  Function prototypes, data, and constants for producer/consumer module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// PRODUCER-CONSUMER data
counter_t *prCount;
counter_t *coCount;
// PRODUCER-CONSUMER put() get() function prototypes
int put();
int get();
// PRODUCER-CONSUMER thread method function prototypes
void *prod_worker(void *arg);
void *cons_worker(void *arg);
