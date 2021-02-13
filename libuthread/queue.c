#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct node{
    void *data;
    struct node *next, *prev;
} node;

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
    *data = temp_head->data;
    queue->head = queue->head->next;
    
    if(queue->head == NULL){
        queue->tail = NULL;
        queue->length = 0;
    } else {
	queue->length--;
    }
    
    free(temp_head);
    return -1;
}


int queue_delete(queue_t queue, void *data)
{
    if (queue->head->data == data){
        queue_dequeue(queue, data);
    }
    
    node *current_node = queue->head, *prev;
    
    while (current_node != NULL && current_node->data != data){
        prev = current_node;
        current_node = current_node->next;
    }
    if (current_node == NULL){
        return -1;
    }
    
    prev->next = current_node->next;
    free(current_node);
    queue->length--;
    
    return 0;
    // https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/
}


void printList(struct node* node)
{
    while (node != NULL) {
        printf(" %d ", *(int *)node->data);
        node = node->next;
    }
    printf("\n");
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    if (queue == NULL || func == NULL){
        return -1;
    }
    node *current_node = queue->head;
    while (current_node != NULL){
        if(func(queue, current_node->data, arg) == 1){
            // if data is different than NULL
            if (current_node->data != NULL){
                *data = current_node->data;
                return 0;
            }
        }
        current_node = current_node->next;
    }
    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL) {
        return -1;
    }
    return queue->length;
}
