#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procmon.h"
#include "getmemstat.h"

int getmemstat(statm_entry_t *statm, int entrycnt)
{
  const int pagesize = sysconf(_SC_PAGESIZE);
  char buf[BUFSIZE];
  int ret;
  for (int i = 0; i < entrycnt; i++)
    {
      ret = lseek(statm[i].fd, 0, SEEK_SET);
      if(ret < 0)
	{
	  fprintf(stderr, "Error: lseek of %d failed!\n", statm[i].pid);
	}

      // /proc/[id]/statm is small file.
      // 1KB buffer is sufficient for do this task.
      // I don't think additional exception handling is needed
      // for /proc/[id]/statm is larger then BUFSIZE.
      
      ret = read(statm[i].fd, buf, BUFSIZE);
      if(buf < 0)
	{
	  fprintf(stderr, "Error: read of %d failed.\n", i);
	  return -1;
	}

      buf[ret] = 0; //Terminate it with NULL.
      
      sscanf(buf, "%ld %ld %ld %ld %ld %ld %ld",
	     &statm[i].size,
	     &statm[i].Rss,
	     &statm[i].share,
	     &statm[i].text,
	     &statm[i].lib,
	     &statm[i].data,
	     &statm[i].dt);

      statm[i].size  *= pagesize;
      statm[i].Rss   *= pagesize;
      statm[i].share *= pagesize;
      statm[i].text  *= pagesize;
      statm[i].lib   *= pagesize;
      statm[i].data  *= pagesize;
      statm[i].dt    *= pagesize;
    }

  return 0;
}
