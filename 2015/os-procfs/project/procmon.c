#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "procmon.h"
#include "getmemstat.h"

int main(int argc, char **argv)
{
  statm_entry_t statm [argc - 1];
  char buf[BUFSIZE];
  int ret;

  if(argc < 2)
    {
      printf("Usage: %s [pids] [to] [monitor]\n", argv[0]);
      return 1;
    }

  for (int i = 0; i < argc - 1; i++)
    {
      char *endptr;
      statm[i].pid = strtol(argv[i + 1], &endptr, 10);
      if(*endptr != 0)
	{
	  fprintf(stderr, "Error: invalid input: %s\n", argv[i + 1]);
	  return 2;
	}
      
      ret = snprintf(buf, BUFSIZE, "/proc/%d/statm", statm[i].pid);
      if(ret < 0)
	{
	  fprintf(stderr, "Error: failed to snprintf\n");
	  return 2;
	}

      statm[i].fd = open(buf, O_RDONLY);

      if(statm[i].fd == -1)
	{
	  fprintf(stderr, "Error: failed to open %s\n", buf);
	  return 2;
	}
    }


  for (int cnt = 0; cnt < 180; cnt++)
    {
      getmemstat(statm, argc - 1);

      for (int i = 0; i < argc - 1; i++)
	{
	  printf("%d %ld %ld %ld %ld %ld %ld %ld\n",
	         statm[i].pid,
 	         statm[i].size,
	         statm[i].Rss,
	         statm[i].share,
	         statm[i].text,
	         statm[i].lib,
	         statm[i].data,
	         statm[i].dt);
	}
      //sleep (1);
      usleep (500000);
    }

  return 0;
}
