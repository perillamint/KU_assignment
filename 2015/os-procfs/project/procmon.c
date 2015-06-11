#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
      int commfd;
      statm[i].pid = strtol(argv[i + 1], &endptr, 10);
      if(*endptr != 0)
	{
	  fprintf(stderr, "Error: invalid input: %s\n", argv[i + 1]);
	  return 2;
	}
      
      if(snprintf(buf, BUFSIZE, "/proc/%d/statm", statm[i].pid) < 0)
	{
	  fprintf(stderr, "Error: failed to snprintf\n");
	  return 2;
	}

      if((statm[i].fd = open(buf, O_RDONLY)) == -1)
	{
	  fprintf(stderr, "Error: failed to open %s\n", buf);
	  return 2;
	}

      if(snprintf(buf, BUFSIZE, "/proc/%d/comm", statm[i].pid) < 0)
	{
	  fprintf(stderr, "Error: failed to snprintf\n");
	  return 2;
	}

      if((commfd = open(buf, O_RDONLY)) == -1)
	{
	  fprintf(stderr, "Error: failed to open %s\n", buf);
	  return 2;
	}
      
      if((ret = read(commfd, buf, BUFSIZE)) < 0)
	{
	  fprintf(stderr, "Error: failed to read comm\n");
	  return 2;
	}

      if(NULL == (statm[i].name = malloc(ret)))
	{
	  fprintf(stderr, "Error: malloc failed!\n");
	  return 2;
	}

      memcpy(statm[i].name, buf, ret);
      statm[i].name[ret - 1] = 0x00;

      close(commfd);

      if(snprintf(buf, BUFSIZE, "memlog-%s.dat", statm[i].name) < 0)
	{
	  fprintf(stderr, "Error: snprintf failed!\n");
	  return 2;
	}

      if((statm[i].logfd = open(buf, O_WRONLY | O_CREAT, 0644)) == -1)
	{
	  fprintf(stderr, "Error: log file open failed!\n");
	  return 2;
	}
    }


  printf("%10s %5s %15s %15s %15s %15s %15s %15s %5s\n",
	 "TIME (ms)", "PID", "name", "size (KiB)", "Rss (KiB)",
	 "share (KiB)", "text (KiB)", "data (KiB)", "Wss");

  const char filehdr[] = "# TIME_(ms) size_(KiB) Rss_(KiB) share_(KiB)\n";

  for (int i = 0; i < argc - 1; i++)
    {
      snprintf(buf, BUFSIZE, "# memory log of %s\n%s",
	       statm[i].name, filehdr);
      
      write(statm[i].logfd, buf, strlen(buf));
    }
  
  for (int cnt = 0; cnt < 360; cnt++)
    {
      getmemstat(statm, argc - 1);

      for (int i = 0; i < argc - 1; i++)
	{	  
	  printf("%10d %5d %15s %15ld %15ld %15ld %15ld %15ld %5c\n",
		 cnt * 1000 / 2,
	         statm[i].pid,
		 statm[i].name,
 	         statm[i].size,
	         statm[i].Rss,
	         statm[i].share,
	         statm[i].text,
	         statm[i].data,
		 statm[i].Wss);
	  
	  sprintf(buf, "%d %ld %ld %ld\n",
		  cnt * 1000 / 2,
 	          statm[i].size,
	          statm[i].Rss,
		  statm[i].share);

	  write(statm[i].logfd, buf, strlen(buf));
	}

      //ANSI code to move cursor upward
      printf("%c[%dA", 0x1B, argc - 1);
      printf("%c0", 0x1B);
      usleep (500000);
    }

  //Reset cursor position to end of screen output.
  printf("%c[%dB", 0x1B, argc - 1);
  
  for (int i = 0; i < argc - 1; i++)
    {
      free(statm[i].name);
      close(statm[i].fd);
      close(statm[i].logfd);
    }

  return 0;
}
