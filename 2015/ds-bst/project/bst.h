#ifndef __BST_H_
#define __BST_H_

#include <stdbool.h>

struct bst_node {
  struct bst_node *left;
  struct bst_node *right;
  void *data;
};

typedef struct bst_s {
  // Public stuff.
  bool (*insert)(struct bst_s *this, void *dataptr);
  bool (*delete)(struct bst_s *this, void *delkey, bool freedata);
  void *(*retrieve)(struct bst_s *this, void *key);
  void (*traverse)(struct bst_s *this, void process(void *data));
  bool (*empty)(struct bst_s *this);
  bool (*full)(struct bst_s *this);
  int (*count)(struct bst_s *this);

  // Private stuff
  int (*_compare)(void *arg1, void *arg2);
  int _cnt;
  struct bst_node *_root;
} bst_tree;

bst_tree *new_bst(int (*compare)(void *arg1, void *arg2));
void delete_bst(struct bst_s *deltarg);

#endif
