#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdbool.h>

typedef struct queue_node {
  void *dataPtr;
  struct queue_node *next;
} QUEUE_NODE;

typedef struct queue_struct {
  QUEUE_NODE *front;
  QUEUE_NODE *rear;
  int count;
} QUEUE;

QUEUE *createQueue();
void destroyQueue(QUEUE *queue);
int deQueue(QUEUE *queue, void **itemPtr);
int enQueue(QUEUE *queue, void *itemPtr);
void queueFront(QUEUE *queue, void **itemPtr);
void queueRear(QUEUE *queue, void **itemPtr);

int queueCount (QUEUE *queue);
bool isEmpty(QUEUE *queue);

#endif
