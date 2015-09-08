#include <sys/syscall.h>
#include <unistd.h>

_start() {
  char *hello = "Hello, World!\n";

  syscall(__NR_write, 1, hello, 14);
  syscall(__NR_exit, 0);
}
