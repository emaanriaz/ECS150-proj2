#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node{
    void *data;
    struct node *next;
} typedef node;

struct queue {
    node *head;
    node *tail;
    int length;
};

queue_t queue_create(void)
{
    queue_t q = (queue_t)malloc(sizeof(struct queue));
  q->length = 0;
  q->head = NULL;
  q->tail = NULL;
    return q;
}

int queue_destroy(queue_t queue)
{
 if ((queue == NULL) || (queue->head != NULL)){
    return -1;
 }

 free(queue);
 return 0;

}

int queue_enqueue(queue_t queue, void *data)
{
    if(queue == NULL || data == NULL){
    return -1;
  }

  node *new_node = malloc(sizeof(node));
  new_node->data = data;
  new_node->next = NULL;

  if (queue->head == NULL){
    queue->tail = queue->head = new_node;
  }

  else{
    queue->tail->next = new_node;
    queue->tail = new_node;
  }

  queue->length++;
  return 0;
    
}

int queue_dequeue(queue_t queue, void **data)
{
    if(queue == NULL || queue->head->data == NULL){
    printf("queue empty");
    return -1;
  }

  node *temp_head = queue->head;
  // *data = temp_head->data;
  queue->head = queue->head->next;

  if(queue->head == NULL){
    queue->tail = NULL;
    queue->length = 0;
  }

  free(temp_head);
    return -1;
}


int queue_delete(queue_t queue, void *data)
{
    /* TODO */
    return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO */
	return -1;
}

int queue_length(queue_t queue)
{
	/* TODO */
	return -1;
}




