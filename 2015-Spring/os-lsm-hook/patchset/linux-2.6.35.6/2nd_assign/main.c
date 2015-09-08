#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched.h>

asmlinkage long sys_set_task_tag (pid_t pid, const char __user *tagstr, long taglen)
{
  long i;
  long retval;
  struct task_struct *task;
  struct task_tag *tag;

  printk(KERN_DEBUG "syscall: sys_set_task_tag\n");

  read_lock(&tasklist_lock);
  task = find_task_by_vpid(pid);
  read_unlock(&tasklist_lock);

  if(task == NULL) {
    printk(KERN_ERR "Oops. pid %d is not a process!\n", pid);
    return -EINVAL;
  }
  
  printk(KERN_DEBUG "task found. %p\n", task);

  if(tagstr == NULL) {
    printk(KERN_DEBUG "tagstr is null. deleting tag..");
    if (task -> tag) {
      if(task -> tag -> tag) {
	kfree(task -> tag -> tag);
      }
      kfree(task -> tag);
    }

    task -> tag = NULL;
    return 0;
  }

  tag = kmalloc(sizeof(struct task_tag), GFP_KERNEL);

  if(tag == NULL) {
    printk(KERN_ERR "Oops. Not enough memory.\n");
    return -ENOMEM;
  }
  
  printk(KERN_DEBUG "tag_struct allocated. address: %p\n", tag);

  tag -> taglen = taglen;
  tag -> tag = kmalloc(taglen, GFP_KERNEL);
  
  if(tag -> tag == NULL) {
    printk(KERN_ERR "Oops. Not enough memory. Freeing tag.\n");
    kfree(tag);
    return -ENOMEM;
  }

  printk(KERN_DEBUG "tagstr allocated. address: %p\n", tag -> tag);
  
  retval = copy_from_user(tag -> tag, tagstr, taglen);

  if(retval != 0) {
    printk(KERN_ERR "Oops. copy_from_user cannot copy all of tag string.\n"
	   "copy_from_user could not copy %ld bytes.\n", retval);

    kfree(tag -> tag);
    kfree(tag);
    return -EINVAL;
  }

  for (i = 0; i < taglen; i++) {
    if(tag -> tag[i] == 0) {
      break;
    }
  }

  if(i == taglen) {
    printk(KERN_ERR "ERR: String does not ends with NULL.");
    kfree(tag -> tag);
    kfree(tag);

    return -EINVAL;
  }

  printk(KERN_DEBUG "copied tagstr to tag -> tag. value is %s\n", tag -> tag);

  task -> tag = tag;
  printk(KERN_DEBUG "tag assigned to pid %d.\n", pid);
  return 0;
}

asmlinkage long sys_get_task_tag(pid_t pid, const char __user *tagstr, long taglen) {
  long retval;
  struct task_struct *task;
  
  read_lock(&tasklist_lock);
  task = find_task_by_vpid(pid);
  read_unlock(&tasklist_lock);

  printk(KERN_DEBUG "syscall: sys_get_task_tag\n");

  if(task == NULL) {
    printk(KERN_ERR "Oops. pid %d is not a process!\n", pid);
    return -EINVAL;
  }
  
  printk(KERN_DEBUG "task found. %p\n", task);

  if(task -> tag == NULL) {
    printk(KERN_DEBUG "No tag found.\n");
    return 0;
  }

  if(tagstr == NULL || taglen < task -> tag -> taglen) {
    printk(KERN_DEBUG "Returning length %ld\n", task -> tag -> taglen);
    return task -> tag -> taglen;
  }

  retval = copy_to_user ((void*)tagstr, task -> tag -> tag, task -> tag -> taglen);

  if(retval != 0) {
    printk(KERN_ERR "Oops. copy_to_user cannot copy all of tag string.\n"
	   "copy_to_user could not copy %ld bytes.\n", retval);
    
    return task -> tag -> taglen;
  }

  printk(KERN_DEBUG "tag copied. tag is %s\n", task -> tag -> tag);

  return task -> tag -> taglen;
}
