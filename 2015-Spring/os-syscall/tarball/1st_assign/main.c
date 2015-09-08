#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/init.h>

typedef struct element_s {
  int data;
  struct element_s *next;
} element_t;

element_t *front;
element_t *rear;

asmlinkage void sys_my_enqueue(int a)
{
  element_t *elem = kmalloc(sizeof(element_t), GFP_KERNEL);

  printk(KERN_DEBUG "sys_my_enqueue\n");
  
  if(elem == NULL)
  {
    printk(KERN_ERR "sys_my_enqueue - kmalloc() failed!\n");
    return;
  }

  elem -> next = NULL;
  elem -> data = a;

  if(front != NULL)
    front -> next = elem;

  front = elem;
  
  if(rear == NULL)
  {
    rear = front;
  }
}

asmlinkage int sys_my_dequeue(void)
{
  int ret = 0;
  
  printk(KERN_DEBUG "sys_my_dequeue\n");
  if(rear != NULL)
  {
    element_t *delelem = rear;
    
    ret = rear -> data;
    rear = rear -> next;
    
    kfree(delelem);
  }
  else
  {
    //There is no way to express queue underflow "safely".
    //Crashing whole system using panic()? I don't think it is great.
    //Just return -1 in this case.

    printk(KERN_WARNING "sys_my_dequeue - Queue underflow!\n");
    ret = -1;
  }

  return ret;
}

void __init init_myqueue(void)
{
  front = NULL;
  rear = NULL;
}
