#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static int init_pcbsize(void) {
  printk("sizeof(struct task_struct) = %lu bytes.\n", sizeof(struct task_struct));

  return 0;
}

static void cleanup_pcbsize(void) {
  printk("Module unload\n");
}  

module_init(init_pcbsize);
module_exit(cleanup_pcbsize);

MODULE_LICENSE("GPL");
