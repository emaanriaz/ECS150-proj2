#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

#define MAX_TCB_NUM 1023;
/* TODO */
//global data 
static tcb_t TCB_array[MAX_TCB_NUM];
static uthread_t TIDmap_array[MAX_TCB_NUM];
static queue_t freeTID_q;
static queue_t ready_q;

static uthread_t  getTIDout() {
    uthread_t  *tid_out_ptr ;
    uthread_t  tid_out ;
    queue_dequeue(free_tid_queue, (void **) & tid_out_ptr) ;
    tid_out = *tid_out_ptr ;
    printf("get tid %d out of free tid llist \n", tid_out) ;
}

int uthread_start(int preempt) {
  uthread_t TID;
  ready_q = queue_create();  //create ready and freeTID queues;
  freeTID_q = queue_create();
  for (TID = 0; TID < MAX_TCB_NUM ; TID++) {          //initialize array with pointers to free availible TIDs 
    TIDmap_array[TID] = tid;                          //to be given out to created threads
    queue_enqueue(freeTID_q, (void *) &TIDmap_array[tid_in]) ;
  }
  for (TID = 0; TID < MAX_TCB_NUM ; TID++) {          //optional loop for initializing debugging variables
    TCB_array[TID].tid = tid;
    TCB_array[TID].valid = 0;
  }
  
  /* TODO */
}

int uthread_stop(void) {
	/* TODO */
	return -1;
}

int uthread_create(uthread_func_t func) {
  uthread_t newTID;
  uthread_ctx_t uctx;
  newTID = getTIDout();  //get stack pointer, ucontext and TID
  
  passfail = uthread_ctx_init(&TCB_array[newTID].uctx, (void*) TCB_array[newTID].stack, func);
  TCB_array[newTID].valid = 1;                   //initialize other stuff in TCB
  TCB_array[newTID].state = READY ; 
  queue_enqueue(ready_queue, &tid_map[TID]);  //enqueue thread into ready queue

  if (passfail) {      //if start succeeds
    return passfail;
  } else
	return -1;
	/* TODO */
}

void uthread_yield(void) {
  uthread_t    running_tid ;
  uthread_t    previous_tid ;  //variables for running and previous thread's TIDs
  running_tid   = get_running_tid() ;  
  previous_tid  = running_tid ;
  running_tid   = get_ready_queue() ;  //iniitalize these variables
  
  if (queue_length(ready_queue) == 0) {  //yield condition: ready queue's length != 0
                                        // if 0, do nothing
  }  else {                             //else: perform context switch
    uthread_ctx_switch(&TCB_array[previous_tid], &TCBarray[running_tid] );
  }	/* TODO */
}

uthread_t uthread_self(void) {
  return (get_running_tid()) ;
	/* TODO */
	return -1;
}

void uthread_exit(int retval) {
  
	/* TODO */
}

int uthread_join(uthread_t tid, int *retval) {
  while (1) {          //work in progress
    if ( ) {
	return -1;
    } else {
      yield();    //paramenters: current thread context, next thread context
    }
		/* TODO */
}

