#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

extern void uthread_intr_proc(int) ;

static struct sigaction sigact ;
static sigset_t         orig_sigmask ;

static struct itimerval   utimer ;

void preempt_start(void)
{

    // setup signal masks
    sigact.sa_handler = uthread_intr_proc ;
    sigact.sa_flags = 0 ;
    //    sigact.sa_flags     = SA_RESTART ;

    // setup timers
    utimer.it_value.tv_sec   = 0 ;
    utimer.it_value.tv_usec  = 10000 ; // microsec to sec
    //  utimer.it_value.tv_usec  = 1000000 / HZ ; // microsec to sec
    utimer.it_interval = utimer.it_value ;

    // register timer
    if (setitimer(ITIMER_VIRTUAL, &utimer, NULL) != 0) {
	printf("Setting timer failed !") ;
    }

    // register signal handler
    sigemptyset(&sigact.sa_mask) ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("Registering sigact setup failed !") ;
    }

}

void preempt_stop(void)
{

    // unset signal mask
    sigact.sa_mask = orig_sigmask ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("Registering sigact setup failed !") ;
    }
    printf("Unset intr mask\n") ;

    // disable timer
    utimer.it_value.tv_sec   = 0 ;
    utimer.it_value.tv_usec  = 0 ;
    utimer.it_interval = utimer.it_value ;
    
    if (setitimer(ITIMER_VIRTUAL, &utimer, NULL) != 0) {
	printf("UnSetting timer failed !") ;
    }
}

void preempt_enable(void)
{
    // set mask to enable mask
    sigaddset(&sigact.sa_mask, SIGVTALRM) ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("preempt_enable failed") ;
    }
       
}

void preempt_disable(void)
{
    // set mask to ignore signal
    //   sigact.sa_mask = orig_sigmask ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("preempt_enable failed") ;
    }   
}
