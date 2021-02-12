# User Level Thread Library

## Summary 

This program is a basic user level thread library for Linux. It provides the interface for applications to create and run independent threads concurrently. 

## Implementation

The implementation of the program follows three steps:

1. Creating the queue
2. Thread management
3. Preemption 

### Creating the queue

We implemented the queue as a singly linked list that uses two structs to store data relating to the queue. The first struct, ‘struct queue’ holds information about the head, tail, and length of the queue. The second struct, ‘struct node’ has two fields: a void pointer that will essentially contain the thread’s TCB and another struct that holds pointers to the next and previous nodes in the queue. 

We chose this data structure because it allows us to dynamically allocate memory and perform all operations, except for iterate and delete, in O(1). 


### Thread management


### Preemption 