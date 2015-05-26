#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

static bool insert (linkedlist *this, void *dataptr);
static bool delete (linkedlist *this, void *dltkey, bool freedata);
static void* retrieve (linkedlist *this, void *key);
static void traverse (linkedlist *this, void process(void *data));
static bool empty (linkedlist *this);
static bool full (linkedlist *this);
static int count (linkedlist *this);

  
//BST impl
linkedlist *new_linkedlist (int (*compare)(void *arg1, void *arg2))
{
  linkedlist *list = malloc(sizeof (linkedlist));

  list -> _cnt = 0;
  list -> _head = NULL;

  list -> insert = insert;
  list -> delete = delete;
  list -> retrieve = retrieve;
  list -> traverse = traverse;
  list -> empty = empty;
  list -> full = full;
  list -> count = count;

  list -> _compare = compare;
  
  return list;
}

void delete_linkedlist (linkedlist *deltarg)
{
  //TODO: destructor stuff;
  free(deltarg);
}

// Methods are declared as static to prevent link error.
// Please access it using function pointer in object. 

static bool insert (linkedlist *this, void *dataptr)
{
  struct list_node *newptr = (struct list_node*)malloc(sizeof(struct list_node));

  if(newptr == NULL)
    return false;

  newptr -> data = dataptr;
  newptr -> prev = NULL;
  newptr -> next = NULL;

  if(this -> _tail == NULL)
    {
      this -> _head = newptr;
      this -> _tail = newptr;
    }
  else
    {
      this -> _tail -> next = newptr;
      newptr -> prev = this -> _tail;
      this -> _tail = newptr;
    }

  this -> _cnt ++;

  return true;
}

static bool _delete (linkedlist *list, struct list_node *node,
	             void *delkey, bool freedata)
{
  if(node == NULL)
    return false;

  if(list -> _compare(delkey, node -> data) == 0)
    {
      if(node -> prev != NULL)
	node -> prev -> next = node -> next;

      if(node -> next != NULL)
	node -> next -> prev = node -> prev;

      if(node == list -> _head)
	list -> _head = node -> next;
      
      if(node == list -> _tail)
	list -> _tail = node -> prev;

      if(freedata)
        free(node -> data);
      
      free(node);
      
      list -> _cnt --;
      return true;
    }

  return _delete(list, node -> next, delkey, freedata);
}

static bool delete (linkedlist *this, void *delkey, bool freedata)
{
  return _delete(this, this -> _head, delkey, freedata);
}

static void* _retrieve (linkedlist *list, void *key, struct list_node *node)
{
  if(node)
    {
      if(list -> _compare(key, node -> data) == 0)
	return node -> data;
      else
	return _retrieve(list, key, node -> next);
    }
  else
    return NULL;
}

static void* retrieve (linkedlist *this, void *key)
{
  return _retrieve(this, key, this -> _head);
}

static void _traverse (struct list_node *node, void process(void *data))
{
  if(node)
    {
      process(node -> data);
      _traverse(node -> next, process);
    }

  return;
}

static void traverse (linkedlist *this, void process(void *data))
{
  _traverse(this -> _head, process);
}

static bool empty (linkedlist *this)
{
  return this -> _cnt == 0;
}

static bool full (linkedlist *this)
{
  struct list_node *node = malloc(sizeof(node));

  if(!node)
    return true;

  free(node);
  return false;
}

static int count (linkedlist *this)
{
  return this -> _cnt;
}
