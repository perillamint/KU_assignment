#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include <stdbool.h>

struct list_node {
  struct list_node *prev;
  struct list_node *next;
  void *data;
};

typedef struct linkedlist_s {
  // Public stuff.
  bool (*insert)(struct linkedlist_s *this, void *dataptr);
  bool (*delete)(struct linkedlist_s *this, void *delkey, bool freedata);
  void *(*retrieve)(struct linkedlist_s *this, void *key);
  void (*traverse)(struct linkedlist_s *this, void process(void *data));
  bool (*empty)(struct linkedlist_s *this);
  bool (*full)(struct linkedlist_s *this);
  int (*count)(struct linkedlist_s *this);

  // Private stuff
  int (*_compare)(void *arg1, void *arg2);
  int _cnt;
  struct list_node *_head;
  struct list_node *_tail;
} linkedlist;

linkedlist *new_linkedlist(int (*compare)(void *arg1, void *arg2));
void delete_list(struct linkedlist_s *deltarg);

#endif
