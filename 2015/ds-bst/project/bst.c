#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"

static bool insert (bst_tree *this, void *dataptr);
static bool delete (bst_tree *this, void *dltkey, bool freedata);
static void* retrieve (bst_tree *this, void *key);
static void traverse (bst_tree *this, void process(void *data));
static bool empty (bst_tree *this);
static bool full (bst_tree *this);
static int count (bst_tree *this);

  
//BST impl
bst_tree *new_bst (int (*compare)(void *arg1, void *arg2))
{
  bst_tree *tree = malloc(sizeof (bst_tree));

  tree -> _compare = compare;
  tree -> _cnt = 0;
  tree -> _root = NULL;

  tree -> insert = insert;
  tree -> delete = delete;
  tree -> retrieve = retrieve;
  tree -> traverse = traverse;
  tree -> empty = empty;
  tree -> full = full;
  tree -> count = count;
  
  return tree;
}

void delete_bst (bst_tree *deltarg)
{
  //TODO: destructor stuff;
  free(deltarg);
}

// Methods are declared as static to prevent link error.
// Please access it using function pointer in object.

static struct bst_node **_insert(bst_tree *tree, struct bst_node *newnode,
			  struct bst_node **node)
{
  if(*node == NULL)
    return node;

  if(tree -> _compare(newnode -> data, (*node) -> data) < 0)
    return _insert(tree, newnode, &(*node) -> left);
  else
    return _insert(tree, newnode, &(*node) -> right);
}
     

static bool insert (bst_tree *this, void *dataptr)
{
  struct bst_node *newptr = (struct bst_node*)malloc(sizeof(struct bst_node));
  struct bst_node **tmpnode;

  if(newptr == NULL)
    return false;

  newptr -> data = dataptr;
  newptr -> right = NULL;
  newptr -> left = NULL;

  tmpnode = _insert(this, newptr, &this -> _root);
  *tmpnode = newptr;
  
  this -> _cnt ++;
  return true;
}

static struct bst_node *_delete (bst_tree *tree, struct bst_node *root,
				 void* dataptr, bool *found, bool freedata)
{
  struct bst_node *delptr;
  struct bst_node *xchgptr;
  struct bst_node *newroot;
  void *hold;

  if(!root)
    {
      *found = false;
      return NULL;
    }

  if(tree -> _compare(dataptr, root -> data) < 0)
    root -> left = _delete(tree, root -> left, dataptr, found, freedata);
  else if(tree -> _compare(dataptr, root -> data) > 0)
    root -> right = _delete(tree, root -> right, dataptr, found, freedata);
  else
    {
      delptr = root;
      if(!(root -> left))
	{
	  if(freedata)
  	    free(root -> data);
	  
	  newroot = root -> right;
	  free(delptr);
	  *found = true;
	  return newroot;
	}
      else if(!(root -> right))
	{
	  if(freedata)
	    free(root -> data);
	  newroot = root -> left;
	  free(delptr);
	  *found = true;
	  return newroot;
	}
      else
	{
	  xchgptr = root -> left;
	  while(xchgptr -> right)
	    xchgptr = xchgptr -> right;

	  hold = root -> data;
	  root -> data = xchgptr -> data;
	  xchgptr -> data = hold;
	  root -> left = _delete(tree, root -> left, xchgptr -> data, found, freedata);
	}
    }

  return root;
}

static bool delete (bst_tree *this, void *delkey, bool freedata)
{
  bool found = false;
  struct bst_node *newroot;

  newroot = _delete(this, this -> _root, delkey, &found, freedata);

  if(found)
    {
      this -> _root = newroot;
      this -> _cnt --;
      if(this -> _cnt == 0)
        this -> _root = NULL;
    }

  return found;
}

static void* _retrieve (bst_tree *tree, void *key, struct bst_node *node)
{
  if(node)
    {
      if(tree -> _compare(key, node -> data) < 0)
	return _retrieve(tree, key, node -> left);
      else if(tree -> _compare(key, node -> data) > 0)
	return _retrieve(tree, key, node -> right);
      else
	return node -> data;
    }
  else
    return NULL;
}

static void* retrieve (bst_tree *this, void *key)
{
  return _retrieve(this, key, this -> _root);
}

static void _traverse (struct bst_node *node, void process(void *data))
{
  if(node)
    {

      int i = -1, j = -1;

      if(node -> left)
	i = *(int*)node -> left -> data;
      if(node -> right)
	j = *(int*)node -> right -> data;

      assert(*(int*)node -> data > i || i == -1);
      assert(*(int*)node -> data < j || j == -1);
      _traverse(node -> left, process);
      process(node -> data);
      _traverse(node -> right, process);
    }

  return;
}

static void traverse (bst_tree *this, void process(void *data))
{
  _traverse(this -> _root, process);
}

static bool empty (bst_tree *this)
{
  return this -> _cnt == 0;
}

static bool full (bst_tree *this)
{
  struct bst_node *node = malloc(sizeof(node));

  if(!node)
    return true;

  free(node);
  return false;
}

static int count (bst_tree *this)
{
  return this -> _cnt;
}
