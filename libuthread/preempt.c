
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

extern void uthread_intr_proc(void) ;

static struct sigaction sigact ;
static sigset_t         orig_sigmask ;

static struct itimerval   utimer ;

void preempt_start(void)
{
    int timer_ret ;
    int irt_itval_ret ;
    
    sigact.sa_sigaction = &uthread_intr_proc ;
    sigact.sa_flags     = SA_RESTART ;

    utimer.it_value.tv_sec   = 0 ;
    utimer.it_value.tv_usec  = 10 ;
    utimer.it_interval = utimer.it_value ;
    
    if (setitimer(ITIMER_VIRTUAL, &utimer, NULL) != 0) {
	printf("Setting timer failed !") ;
    }
    
    sigprocmask(SIG_UNBLOCK, NULL, &orig_sigmask) ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("Registering sigact setup failed !") ;
    }

}

void preempt_stop(void)
{
    sigact.sa_mask = orig_sigmask ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("Registering sigact setup failed !") ;
    }
    printf("Unset intr mask\n") ;

    utimer.it_value.tv_sec   = 0 ;
    utimer.it_value.tv_usec  = 0 ;
    utimer.it_interval = utimer.it_value ;
    
    if (setitimer(ITIMER_VIRTUAL, &utimer, NULL) != 0) {
	printf("UnSetting timer failed !") ;
    }
}

void preempt_enable(void)
{
    sigaddset(&sigact.sa_mask, SIGVTALRM) ;
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("preempt_enable failed") ;
    }
       
}

void preempt_disable(void)
{
    if (sigaction(SIGVTALRM, &sigact, NULL) != 0) {
	printf("preempt_enable failed") ;
    }   
}
