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

#include "unistd.h"
#include "queue.h"

#define TCB_POOL_SIZE 8
typedef void (*func_t) ();

enum thread_state {NOT_USED, RUNNING , BLOCKED, ZOMBIE , READY, EXITED} ;

typedef struct {
    uthread_t           tid ;                       // tid, just for debug
    //   uthread_t           parent_tid ;                
    uthread_ctx_t       uctx;                       
    int                 retval ;
    void                *retval_ptr ;                // pointer of returned value
    uthread_t           joined_tid;                  // joiner
    void                *joined_tid_retval;          // pointer to pointer to joiner's return valud
    int                 valid;                       // valid,
    int                 is_joined ;
    enum thread_state   state ;
    void                *stack ;     // stack segment
} tcb_t ;


static     tcb_t         tcb_array[TCB_POOL_SIZE+1] ;

static     uthread_t     tid_map[TCB_POOL_SIZE+1] ;

static     queue_t       free_tid_queue ;
static     queue_t       ready_queue ;
static     queue_t       zombie_queue ;

static     uthread_t     running_tid ;
//static     uthread_ctx_t   main_uctx ;

static void put_into_free_tid_queue(uthread_t  tid) {
    
    printf("DEBUG: put tid %0d into free tid llist  \n", tid) ;
    // set state
    // free stack space
    uthread_ctx_destroy_stack(tcb_array[tid].stack) ;
    queue_enqueue(free_tid_queue, (void *) &tid_map[tid]) ;
    //
}

uthread_t  get_from_free_tid_queue() {
    uthread_t  *tid_ptr ;
    queue_dequeue(free_tid_queue, (void **) &tid_ptr) ;
    //   tid_ptr = (uthread_t *) tid_ptr ;
    //    printf("DEBUG: get tid %0d out of free tid queue \n", *tid_ptr) ;
    return(*tid_ptr) ;
}

void put_into_ready_queue(uthread_t tid) {
    
    //printf("DEBUG: put tid %0d into ready_queue.\n", tid) ;
    queue_enqueue(ready_queue, (void *) &tid_map[tid]) ;
}

void put_into_zombie_queue(uthread_t tid) {   
    //printf("DEBUG: put tid %0d into zombie_queue. \n", tid) ;
    queue_enqueue(zombie_queue, (void *) &tid_map[tid]) ;		  
}

uthread_t get_from_ready_queue() {
    uthread_t *tid_ptr ;
    queue_dequeue(ready_queue, (void **) &tid_ptr) ;
    //  tid_ptr = (uthread_t *) tid_ptr ;
    //printf("DEBUG: get tid %0d from ready_queue.\n", (*tid_ptr)) ;
    return(*tid_ptr) ;
}

void get_tid_out_of_zombie_queue(uthread_t tid) {
    tcb_array[tid].state = EXITED ;
    queue_delete(zombie_queue, &tid_map[tid]) ;
}

uthread_t  get_running_tid() {
    return(running_tid) ;
}

static void schedule_next (void) {
    uthread_t       previous_tid ;
    uthread_t       next_tid ;

    if (queue_length(ready_queue) == 0) {
	sleep(2) ;
	//printf ("DEBUG: No more threads, stop.\n ") ;
	//	uthread_ctx_switch(&tcb_array[0].uctx, &main_uctx) ;
    } else {
	previous_tid   = get_running_tid() ;
	next_tid       = get_from_ready_queue() ;
	while (tcb_array[next_tid].state != READY ||
	       (next_tid == 0 && queue_length(ready_queue) != 0) 
	       ) {
	    //printf("DEBUG: next_id %0d is not READY. Get next one.\n", next_tid) ;
	    put_into_ready_queue(next_tid) ;
	    next_tid = get_from_ready_queue() ;
	}
	//printf("DEBUG: Switch from tid %0d to tid %0d \n", previous_tid, next_tid) ;
	//	uthread_ctx_switch(  NULL, &tcb_array[next_tid].uctx) ;
	if (previous_tid != next_tid) {
	    running_tid = next_tid ;
	    uthread_ctx_switch(  &tcb_array[previous_tid].uctx, &tcb_array[next_tid].uctx) ;
	}
    }
}

//uthread_func_t null_func(void) {
//}
    
int uthread_start(int preempt)
{
    uthread_t tid ;

   // printf("DEBUG: Start uthread library with preempt %0d", preempt) ;
    
    // setup free tid queue
    free_tid_queue = queue_create() ;
    ready_queue    = queue_create() ;
    zombie_queue   = queue_create() ;
    // put tid_in
    for (tid = 0 ; tid < TCB_POOL_SIZE; tid ++ ) {
	tid_map[tid] = tid ;
	put_into_free_tid_queue(tid) ;
    }
    
    printf("DEBUG: Start uthread stops.\n") ;
    // initial tcb array fields
    for (tid = 0 ; tid < TCB_POOL_SIZE; tid ++) {
       	tcb_array[tid].retval_ptr = (void *) & tcb_array[tid].retval ;
	// for debug
	tcb_array[tid].tid   = tid ;
	tcb_array[tid].state = NOT_USED ;
	tcb_array[tid].valid = 0 ;
    }
    //  Real Main thread
    
    tid = get_from_free_tid_queue() ;
    tcb_array[tid].tid         = tid ;
    tcb_array[tid].state       = READY ;
    tcb_array[tid].is_joined   = 0 ;
    tcb_array[tid].joined_tid   = 0 ;
    uthread_ctx_init(&tcb_array[tid].uctx, uthread_ctx_alloc_stack(), NULL) ;
    //
    running_tid = tid ;
    //   uthread_create(NULL) ;
    // Create a scheduling main thread
    //
    
    //
    return(0) ; // set it to zero for now, will add fail condition later.
}

int uthread_stop(void)
{
    int df, dr, dz ;
       
    while (queue_length(ready_queue) > 0) {
	printf("DEBUG: Unfinished threads\n") ;
	schedule_next() ;
	//printf("queue_length = %0d", queue_length(ready_queue)) ;
    }
    //printf("DEBUG: uthread stops.\n") ;
     df = queue_destroy(free_tid_queue) ;
     dr = queue_destroy(ready_queue) ;
     dz = queue_destroy(zombie_queue) ;
     if (df * dr * dz == 0) {
	 return 0 ;
     } else {
	 return -1;
     }
}

int uthread_create(uthread_func_t func)
{
    uthread_t          tid ;
    //   uthread_t          parent_tid ;
    int                out_of_tid ;
    int                ctx_init_fail ;
    
    tid = get_from_free_tid_queue() ;
    //    child_tid  = get_from_free_tid_queue() ;

    out_of_tid = (tid == TCB_POOL_SIZE) ? 1 : 0 ;

    ctx_init_fail        = uthread_ctx_init(&tcb_array[tid].uctx, uthread_ctx_alloc_stack(), func) ;
    
    //
    tcb_array[tid].valid      = 1 ;
    tcb_array[tid].state      = READY ;
    tcb_array[tid].is_joined  = 0 ;

    //printf("DEBUG: Thread tid %0d is created.\n", tid) ;
    put_into_ready_queue(tid) ;
    
    if ( (out_of_tid * ctx_init_fail == 0) )
	return  tid  ;
    else
	return -1;
}

void uthread_yield(void)
{
    uthread_t    running_tid ;
    
    running_tid = get_running_tid() ;
    //printf("DEBUG: Running tid %0d yield. \n", running_tid) ; 
    put_into_ready_queue(running_tid) ;
    schedule_next() ;

}

uthread_t uthread_self(void)
{
    return (get_running_tid()) ;
}

void uthread_exit(int retval)
{
    uthread_t       tid ;
    uthread_t       join_tid ;
    
    tid = get_running_tid() ;
    //printf("DEBUG: Thread tid %0d exits with retval value %0d.\n", tid, retval) ;
    tcb_array[tid].state = ZOMBIE ;
    tcb_array[tid].retval = retval ;
    put_into_zombie_queue(tid) ;
    
    if (tcb_array[tid].is_joined) { // This tid is joined by another tid
	join_tid = tcb_array[tid].joined_tid ;
	tcb_array[join_tid].state = READY ;
	//      retval of join process had to be added
	//	tcb_array[join_tid].
	//printf("DEBUG: Thread tid %0d is UNBLOCKED.\n", join_tid) ; 
    } 
    schedule_next() ;
}

int uthread_join(uthread_t tid, int *retval)
{
    uthread_t   joining_tid ;

    
    joining_tid = get_running_tid() ;

    if (tcb_array[tid].state == ZOMBIE) { // Joined tid is dead already.
	retval = & tcb_array[tid].retval ;
	get_tid_out_of_zombie_queue(tid) ;
	tcb_array[tid].state = EXITED ;
	put_into_free_tid_queue(tid) ;
	printf("DEBUG: Thread tid %0d joins a zombie thread %0d return value %p. \n", joining_tid, tid, retval) ;
    } else {                              // Joined tid is not dead yet.
	tcb_array[tid].is_joined         = 1  ;
	tcb_array[tid].joined_tid        = joining_tid ;
	//      retval of join process had to be added
	//	retval = tcb_array[tid].joined_tid_retval ;
	if (tid == joining_tid) {
	//    printf ("DEBUG: Same thread %0d, %0d, can't block itself.\n", tid, joining_tid) ;
	} else {
	    tcb_array[joining_tid].state = BLOCKED ;
	  //  printf("DEBUG: Running thread tid %0d joins thread %0d return value %p.\n", joining_tid, tid, retval) ;
	}
	put_into_ready_queue(running_tid) ;
       	schedule_next() ;
    }
    return (0) ;
}

