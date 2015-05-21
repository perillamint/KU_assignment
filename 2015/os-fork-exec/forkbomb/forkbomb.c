#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char **argv) {
  int i;
  int res;

  printf("Forkbomb!\n");
  
  for(i = 0; res = fork(); i++) {
    if(res == -1) {
      printf("Fork failed! %s\n", strerror(errno));
      printf("Fork count: %d\n", i);
      exit(0);
    }
  }

  return 0;
}
