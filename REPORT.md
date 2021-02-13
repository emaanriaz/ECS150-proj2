# User Level Thread Library

## Summary 

This program is a basic user level thread library for Linux. It provides the interface for 
applications to create and run independent threads concurrently. 

## Implementation

The implementation of the program follows three steps:

1. Creating the queue
2. Thread management (libuthread)
3. Preemption 

### Creating the queue

We implemented the queue as a singly linked list that uses two structs to store data relating to 
the queue. The first struct, `struct queue` holds information about the head, tail, and length 
of the queue. The second struct, `struct node` has two fields: a void pointer that will 
essentially contain the thread’s TCB and another struct that holds pointers to the next and
previous nodes in the queue. 

We chose this data structure because it allows us to dynamically allocate memory and perform
all operations, except for iterate and delete, in O(1). 

### Thread management (Thread library)

Next, we had to create a thread library by creating our own TCB and completing each thread 
function defined in uthread.h. We decided to do the TCB first to determine what each thread 
would need as it went through its life cycle, since that's also what the libuthread functions
would have to recieve and manipulate as parameters. We first added basic essentials like the 
thread context and the TID, and added on the rest as the project proceeded and more needs 
became apparent. Several flags we also included to help with debugging, and were enumerated 
to initialize them before debugging.

One of the more important decisions we made was to represent the TIDs in an array connected to
a linked list (the free TID queue). That way, we could more easily retrieve and store threads 
than by using pointers (for a linked list); however, this method is limited by the size of the
array.

The method by how we implemented each uthread function is below:

1. Start: This function has to start up everything the library needs to run the uthreads, 
which includes the queues of the ready and zombie states as well as the queue with the free 
TIDs. So we decided to initialize the queues right there and map the TIDs in the TID map as 
well (a critical step to allow the threads to be mapped by the TIDs properly). 

2. Stop: To properly stop the library's execution, all the queues need to be destroyed 
successfully, so we decided to use the destruction's success (or lack thereof) to decide what
to return. The 3 local variables should all be 0 when the queues are successfully destroyed,
so we decided to add them up and compare the sum to 0 as the condition for an if-else statement 
to decide what to return. 

3. Create: This would prove to be a fairly sequential program, as our plan simply called for 
the thread to be created and have it's various TCB contents initialized. We decided to get 
the TID from the free tid queue (later adding a check to see whether there were any TIDs 
still availible) before initializing the flags. The returning value for this function was 
determined in a similar manner that it was determined for uthread_stop (to make things more
consistent), with the out_of_tid and ctx_init_fail flags, respectively.

4. Yield: The last thread must make way for the next thread and placed in the ready queue, 
so we used a same function as self to get the former and put it in its place. Initially we 
decided to simply yield to the next thread in the ready queue (the most intuitive decision),
but when scheduling issues appeared, we decided to write a dedicated scheduler function 
(described in a section further down the report).

5. Self: By far the simplest function in a long and extremely challenging program, it simply
calls a function we made ourselves to get the running TID. (Admittedly we could have made it
directly return the running TID, but time was getting a bit short, so this wasn't done.)

6. Exit:

7. Join:

In addition, over the course of the function, we added several of our own functions in 
uthread.c to help simplify writing repeated but important instructions in uthread.c:

put_into_free_tid_queue: This function was intended to recycle the threads of TIDs that 
finished by putting them back into the free tid queue, which is done after destroying the
threads' stacks (as they are no longer needed).

put_into_ready_queue: Puts the thread parameter into the ready queue

put_into_zombie_queue: Same as above, but for the zombie queue

get_from_ready_queue: Dequeues a thread out of the running queue and returns it. 

get_tid_out_of_zombie_queue: If a thread is collected, it's no longer a zombie, and its
TID is free to be used by other threads. This need is fulfilled by this function, which 
raises an exit flag to indicate that the thread is done, and removes the thread from
the zombie queue accordingly. 

get_running_tid: Gets the runnning thread's TID by calling get_running(TID).

schedule_next: This is by far the most complex self-created program in uthread.c we 
wrote, and it was created to schedule the next running thread in tricky situations 
that arose. It simply prints out a message indicating the test program's completion
if the ready queue is 0 (thus also acting as a check to see if there's anything 
left in that queue, and as an indicator of test program completion). However, if there's 
still something in the ready queue, the "else" statement handles the scheduling 
situations involving the currently running thread and next thread in the reay queue.

In addition to using GDB and tracing out the output, we put various printouts throughout 
the program (especially uthread.c) in order to debug the program. These printouts were 
placed strategically thoughout the program, especially in the self-created functions, 
to display important values, especially the currently running thread, what was coming in 
and out of the queues, and various debugging flags. While all of them were removed while 
cleaning out the code, it's safe to say that we couldn't have gotten this far without them, 
especially on resolving various segfaults and correcting incorrect pointer syntax.

### Preemption 

For the sake of time, I can only say I couldn't get the timer and interrupt right for this 
part, so I'll explain my thought process and my plans for what I *tried* to do instead.  

### Recommendations for the future

Even when compared to the first project, the second project was a significant challenge 
for us, even when compared to past assignments outside of this class. I reccommend that 
