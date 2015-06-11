#ifndef __PROCMON_H_
#define __PROCMON_H_

typedef struct {
  pid_t pid;
  char *name; 
  int fd;
  int logfd;
  long size;
  long Rss;
  long share;
  long text;
  long lib;
  long data;
  long dt;
} statm_entry_t;

#define BUFSIZE 1024

#endif
