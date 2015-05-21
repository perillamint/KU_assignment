#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue.h"

/**
 * Initializes QUEUE object and returns it.
 */
QUEUE *createQueue () {
  QUEUE *queue = malloc(sizeof(QUEUE));
  queue -> front = NULL;
  queue -> rear = NULL;
  queue -> count = 0;

  return queue;
}

/**
 * Destroy QUEUE object.
 * WARN: if queue contains object in it, Freeing it will occur MEMORY LEAK.
 */
void destroyQueue (QUEUE *queue) {
  free(queue);
}

/**
 * Insert item in queue.
 */
int enQueue (QUEUE *queue, void *itemPtr) {
  QUEUE_NODE *node = malloc(sizeof(QUEUE_NODE));

  if(node == NULL)
    return -1;

  node -> dataPtr = itemPtr;
  node -> next = NULL;

  if(queue -> rear != NULL) {
    queue -> rear -> next = node;
    queue -> rear = node;
  } else {
    queue -> front = node;
    queue -> rear = node;
  }
  
  queue -> count ++;

  return 0;
}

/**
 * Take item from queue.
 */
int deQueue (QUEUE *queue, void **itemPtr) {
  QUEUE_NODE *node = queue -> front;

  if(node == NULL)
    return -1;

  if(itemPtr != NULL)
    *itemPtr = node -> dataPtr;
  
  queue -> front = node -> next;

  if (queue -> front == NULL)
    queue -> rear = NULL;

  queue -> count --;

  free(node);

  return 0;
}

/**
 * Take item in front of queue.
 * Unlike deQueue, this method does not removes it from queue.
 */
void queueFront (QUEUE *queue, void **itemPtr) {
  if (queue -> front)
    *itemPtr = queue -> front -> dataPtr;
  else
    *itemPtr = NULL;
}

/**
 * Take item in rear of queue.
 * Unlike deQueue, this method does not removes it from queue.
 */
void queueRear (QUEUE *queue, void **itemPtr) {
  if (queue -> rear)
    *itemPtr = queue -> rear -> dataPtr;
  else
    *itemPtr = NULL;
}

/**
 * Return size of queue.
 */
int queueCount (QUEUE *queue) {
  return queue -> count;
}

/**
 * Checks if queue is empty.
 * Returns true if it is empty.
 */
bool isEmpty(QUEUE *queue) {
  if(queue -> front == NULL) {
    return true;
  }

  return false;
}
