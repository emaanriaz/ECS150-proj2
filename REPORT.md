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
2. Stop: 
3. Create:
4. Yield: 
5. Self:
6. Exit:
7. Join:

In addition, over the course of the function, we added several of our own functions in 
uthread.c to help simplify writing repeated but important instructions:

### Preemption 
