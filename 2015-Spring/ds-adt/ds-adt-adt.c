#include <stdio.h>
#include <stdlib.h>

//Some macro hack.
//new(constructor, object)
//delete(object)
#define new(x, o) o -> _constructor = x; o -> _constructor(o)
#define delete(x) x -> _destructor(x)

typedef struct queuenode_s {
  int data;
  struct queuenode_s *next;
} queuenode_t;

typedef struct queue_s {
  //Public section
  void (*push)(struct queue_s *this, int data);
  int (*pop)(struct queue_s *this);
  int (*getSize)(struct queue_s *this);

  //Private section - DO NOT MANIPULATE THIS DIRECTLY.
  int _size;
  queuenode_t *_front;
  queuenode_t *_rear;

  void (*_constructor)(struct queue_s *this);
  void (*_destructor)(struct queue_s *this);
} queue_t;

void queue_push(queue_t *this, int data) {
  queuenode_t *node = malloc(sizeof(queuenode_t));

  if(node == NULL) {
    exit(-1);
  }

  node -> next = NULL;
  node -> data = data;

  if(this -> _front != NULL)
    this -> _front -> next = node;

  this -> _front = node;

  if(this -> _rear == NULL)
    this -> _rear = this -> _front;

  this -> _size += 1;
}

int queue_pop(queue_t *this) {
  int ret = 0;

  if(this -> _rear == NULL)
    exit(-1);

  queuenode_t *delnode = this -> _rear;
  
  ret = this -> _rear -> data;
  this -> _rear = this -> _rear -> next;

  free(delnode);

  this -> _size -= 1;
  return ret;
}

int queue_getSize(queue_t *this) {
  return this -> _size;
}

void queue_destructor(queue_t *this) {
  while(this -> getSize(this) != 0)
    this -> pop(this);

  free(this);
}

void queue_constructor(queue_t *this) {
  //initialize queue object.
  this -> push = queue_push;
  this -> pop = queue_pop;
  this -> getSize = queue_getSize;
  this -> _destructor = queue_destructor;
  this -> _front = NULL;
  this -> _rear = NULL;
  this -> _size = 0;
}

int data[] = {1, 42, 73, 423, -1};
int dataSize = 5;

int main (int argc, char **argv) {
  int i;

  queue_t *queue = malloc(sizeof(queue_t));
  new(queue_constructor, queue);

  for (i=0; i<dataSize; i++) {
    printf("queue push: %d\n", data[i]);
    queue -> push(queue, data[i]);
  }

  for (i=0; i<dataSize; i++) {
    printf("queue pop: %d\n", queue -> pop(queue));
  }

  delete(queue);
  return 0;
}
