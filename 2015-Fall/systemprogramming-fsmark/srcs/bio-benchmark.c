#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/bio.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/spinlock.h>

struct proc_dir_entry *procfile;

#define LQ_SIZE 10485760
const int lq_size = LQ_SIZE;
char lq[LQ_SIZE];
int lq_start = 0;
int lq_end = 0;

spinlock_t bio_lq_spinlock;

int get_lq_size(void)
{
  return (lq_size - lq_start + lq_end) % lq_size;
}

void lq_write(char *log, int len)
{
  int written = 0;
  int lq_end_before = lq_end;

  spin_lock(&bio_lq_spinlock);

  do {
    //Truncate?
    if(lq_end + len - written > lq_size)
    {
      //Truncated
      memcpy(lq + lq_end, log + written, lq_size - lq_end);

      //Update lq_end and lq_start
      written += lq_size - lq_end;
      lq_end = 0;
      if(lq_start == lq_size - 1)
        lq_start = 1;
    }
    else
    {
      //Not truncated
      memcpy(lq + lq_end, log + written, len - written);

      //Update lq_end and lq_start
      lq_end += len - written;
      written += len - written;
      if(lq_end_before > lq_end && lq_end > lq_start)
        lq_start = lq_end + 1;
    }
  } while(written < len);

  spin_unlock(&bio_lq_spinlock);
}

int lq_read(char *buf, int len)
{
  int fetched = 0;

  spin_lock(&bio_lq_spinlock);
  printk("R lq size=%d lq_start=%d lq_end=%d len=%d\n",
         get_lq_size(), lq_start, lq_end, len);
  if(lq_end < lq_start)
  {
    memcpy(buf, lq + lq_start,
           (lq_size - lq_start < len)?lq_size - lq_start:len);

    if(lq_size - lq_start < len)
      memcpy(buf + lq_size - lq_end, lq, len - lq_size + lq_start);
  }
  else if(lq_end > lq_start)
  {
    memcpy(buf, lq + lq_start,
           (lq_end - lq_start < len)?lq_end - lq_start:len);
  }
  fetched = (get_lq_size() <= len)?get_lq_size():len;
  buf[fetched] = 0x00;
  lq_start += fetched;
  lq_start %= lq_size;

  spin_unlock(&bio_lq_spinlock);
  return fetched;
}

static ssize_t bio_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
  int ret;
  char buffer[count];
  if((file->f_flags & O_NONBLOCK) && get_lq_size() <= 0)
  {
    return -EAGAIN;
  }

  ret = lq_read(buffer, count);
  copy_to_user(buf, buffer, count);
  return ret;
}

asmlinkage void hook_submit_bio(int rw, struct bio *bio)
{
  int len;
  int count;
  char b[BDEVNAME_SIZE];
  char buf[512];

  count = bio_sectors(bio);

  //We want WRITE only
  if(!(rw & WRITE))
    jprobe_return();

  len = snprintf(buf, 512, "%s(%d): %s block %Lu on %s (%u sectors)\n",
                 current->comm, task_pid_nr(current),
                 (rw & WRITE) ? "WRITE" : "READ",
                 (unsigned long long)bio->bi_sector,
                 bdevname(bio->bi_bdev, b),
                 count);

  lq_write(buf, len);

  jprobe_return();
}

static struct jprobe my_jprobe = {
        .entry = (kprobe_opcode_t *) hook_submit_bio
};
static const struct file_operations proc_bio_operations = {
  .read = bio_read,
};

static int __init init_bio_benchmark(void)
{
  int ret;

  spin_lock_init(&bio_lq_spinlock);
  my_jprobe.kp.addr = (kprobe_opcode_t *)submit_bio;
  ret = register_jprobe(&my_jprobe);

  if(ret < 0)
  {
    printk("register_jprobe failed, returned %d\n", ret);
    return -1;
  }

  procfile = proc_create("bio_benchmark", 0444, NULL, &proc_bio_operations);

  procfile -> mode = 0444;
  procfile -> uid = 0;
  procfile -> gid = 0;

  return 0;
}

static void cleanup_bio_benchmark(void)
{
  unregister_jprobe(&my_jprobe);
  remove_proc_entry("bio_benchmark", NULL);
}

module_init(init_bio_benchmark);
module_exit(cleanup_bio_benchmark);

MODULE_LICENSE("GPL");
