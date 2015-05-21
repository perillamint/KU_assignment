#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __x86_64__
#define __NR_set_task_tag 302
#define __NR_get_task_tag 303
#else // Assume X86_32
#define __NR_set_task_tag 340
#define __NR_get_task_tag 341
#endif

int main(int argc, char **argv) {
  int i;
  char *tagstr = malloc(strlen("NOTNOTKILL") + 1);
  long pid;
  int ret;

  if((pid = fork()) == 0) {
    sleep(3600);
  }

  syscall(__NR_set_task_tag, pid, "NOTKILL", strlen("NOTKILL") + 1);
  syscall(__NR_get_task_tag, pid, tagstr, strlen("NOTKILL") + 1);
  printf("%s\n", tagstr);

  kill(pid, 9); //SIGKILL

  syscall(__NR_set_task_tag, pid, "NOTNOTKILL", strlen("NOTNOTKILL") + 1);
  syscall(__NR_get_task_tag, pid, tagstr, strlen("NOTNOTKILL") + 1);
  printf("%s\n", tagstr);

  kill(pid, 9); //SIGKILL

  waitpid(pid, &ret, 0);

  printf("%d\n", ret);
  return 0;
}
