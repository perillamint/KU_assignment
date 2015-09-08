#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bst.h"
#include "linkedlist.h"

#define TESTSIZE 16384
#define TESTDIV 16
#define TESTITER 10

int arr1[TESTSIZE];

clock_t _begin;

int compare (void *arg1, void *arg2)
{
  int *a = arg1;
  int *b = arg2;

  if(*a > *b) return 1;
  else if(*a < *b) return -1;
  else return 0;
}

void dummy(void *data)
{
  return;
}

extern inline void benchmark_start()
{
  _begin = clock();
}

extern inline clock_t benchmark_end()
{
  return clock() - _begin;
}

int main(int argc, char **argv)
{
  bst_tree *tree[TESTDIV];
  linkedlist *list[TESTDIV];

  srand(time(NULL));

  clock_t foo;
  
  for(int i = 0; i < TESTDIV; i++)
    {
      int testsize = TESTSIZE / TESTDIV * (i + 1);
      tree[i] = new_bst(compare);
      list[i] = new_linkedlist(compare);
      
      for(int j = 0; j < testsize; j++)
	{
	  arr1[j] = j+1;
	}
      
      for(int j = 0; j < testsize; j++)
	{
	  int tmp;
          int foo = j + rand() / (RAND_MAX / (testsize - j) + 1);

	  tmp = arr1[foo];
	  arr1[foo] = arr1[j];
	  arr1[j] = tmp;
	}

      for(int j = 0; j < testsize; j++)
	{
	  int *tmp = malloc(sizeof(int));

	  *tmp = arr1[j];

	  tree[i] -> insert(tree[i], tmp);
	  list[i] -> insert(list[i], tmp);
	}
    }
      

  printf("# Binary tree search\n"
         "# [data size] [time (nsec)]\n\n");
  for(int i = 1; i <= TESTDIV; i++)
    {
      int testsize = TESTSIZE / TESTDIV * i;

      foo = 0;
      for(int iter = 0; iter < TESTITER; iter++)
	{
	  benchmark_start();

	  for(int j = 1; j <= testsize; j++)
	    tree[i - 1] -> retrieve(tree[i - 1], &j);

	  foo += benchmark_end();
	}

      printf("%d %ld\n", testsize,
	     foo / (CLOCKS_PER_SEC / 1000 / 1000) * 1000 / testsize / TESTITER);
    }
  
  printf("# Linked list search\n"
         "# [data size] [time (nsec)]\n\n");
  for(int i = 1; i <= TESTDIV; i++)
    {
      int testsize = TESTSIZE / TESTDIV * i;

      foo = 0;
      for(int iter = 0; iter < TESTITER; iter++)
	{
	  benchmark_start();
      
	  for(int j = 1; j <= testsize; j++)
	    list[i - 1] -> retrieve(list[i - 1], &j);

	  foo += benchmark_end();
	}

      printf("%d %ld\n", testsize,
	     foo / (CLOCKS_PER_SEC / 1000 / 1000) * 1000 / testsize / TESTITER);
    }
  /* 
      for(int i = 0; i < TESTSIZE; i++)
	{
	  list -> delete(list, &arr2[i]);
	  tree -> delete(tree, &arr2[i]);
	}
  */
  /* 
  printf("%d\n", tree -> count(tree));
  printf("%d\n", list -> count(list));
  */
  
  return 0;
}
