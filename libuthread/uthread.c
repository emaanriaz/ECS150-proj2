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

/* TODO */

int uthread_start(int preempt) {
	
  if () {      //if start succeeds
    return 0;
  } else
	return -1;
  /* TODO */
}

int uthread_stop(void)
{
	/* TODO */
	return -1;
}

int uthread_create(uthread_func_t func) {
	
	return -1;
	/* TODO */
}

void uthread_yield(void) {
  //yield condition:
  uthread_ctx_switch(*prev,  *next);    //both uthread_ctx_t
	/* TODO */
}

uthread_t uthread_self(void)
{
	/* TODO */
	return -1;
}

void uthread_exit(int retval) {
	/* TODO */
}

int uthread_join(uthread_t tid, int *retval) {
  while (1) {          //work in progress
    if () {
	return -1;
    } else {
      yield();
    }
		/* TODO */
}

