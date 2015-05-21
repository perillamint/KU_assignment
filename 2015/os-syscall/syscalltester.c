#include <unistd.h>
#include <stdio.h>

#if __x86_64__
#define __NR_my_enqueue 300
#define __NR_my_dequeue 301
#else // Assume X86_32
#define __NR_my_enqueue 338
#define __NR_my_dequeue 339
#endif

int arr[] = {1, 42, 72, 765, -1};
int arr_len = 5;

int main(int argc, char **argv) {
  int i;

  for(i=0; i<arr_len; i++) {
    printf("calling sys_my_enqueue(%d)\n", arr[i]);
    syscall(__NR_my_enqueue, arr[i]);
  }

  for(i=0; i<arr_len; i++)
    printf("sys_my_dequeue() returns %d.\n", syscall(__NR_my_dequeue)); 

  return 0;
}
