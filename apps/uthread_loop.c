/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include "unistd.h"

long int i ;

int thread3(void)
{
	uthread_yield();
	while (1) {
	    i++ ;
	    printf("thread%d\n", uthread_self());
	    sleep(3) ;
	}
	return 0;
}

int thread2(void)
{
	uthread_create(thread3);
	uthread_yield();
	while (1) {
	    i++ ;
	    printf("thread%d\n", uthread_self());
	    sleep(3) ;
	}    
	return 0;
}

int thread1(void)
{
	uthread_create(thread2);
	uthread_yield();
	while (1) {
	    i++ ;
	    printf("thread%d\n", uthread_self());
	    sleep(3) ;
	}
	uthread_yield();
	return 0;
}

int main(void)
{
    uthread_start(1);
    uthread_join(uthread_create(thread1), NULL);
    uthread_stop();
	return 0;
}
