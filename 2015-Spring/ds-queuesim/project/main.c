#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "driver.h"
#include "util.h"

//Remove this macro if you don't want to iterate libc PRNG.
#define PRNG_ITERATE

//This project uses some features of C99. Warn if it is not on C99 env.
#if __STDC_VERSION__ < 199901L
#pragma message("Some part of this source code use C99 standard.")
#pragma message("If it failes to build, please use modern compiler which supports C99 features.")
#endif

int main(int argc, char **argv) {
  char svcTime[128] = { 0 };
  char closeTime[128] = { 0 };
  //DRIVER *createDriver (int openTime, int closeTime, int custGenRate,
  //                      int svcTimeMin, int svcTimeMax, int serverCnt);
  DRIVER *driver = createDriver(60 * 9 + 0, 60 * 9 + 480, 3, 1, 7, 2);

  //Initialize PRNG with current UNIX time as seed.
  srand(time(NULL));

#ifdef PRNG_ITERATE
  //Iterate it approx 500 times to get good distribution.
  int i; //Caution: Mixed declarations and code is invalid in C90 spec.
         //Use GNU90 or C99 to compile it.
  for(i = 0; i < 500; i++) rand();
#endif

  //Do ticks until sim finishes.
  while(!driverDoTick(driver));

  int numCust = driver -> runStats -> numCust;
  minuteToHrs(driver -> runStats -> totSvcTime, svcTime, true);
  minuteToHrs(driver -> elapsedTime + driver -> openTime, closeTime, false);

  //Print sim result.
  printf("Total customers    : %d\n"
	 "Total svc time     : %s\n"
	 "Avg svc time       : %f\n"
	 "Avg wait time      : %f\n"
	 "Maximum queue size : %d\n"
	 "Closed time        : %s\n",
	 numCust, svcTime,
         (double)driver -> runStats -> totSvcTime / (double)numCust,
	 (double)driver -> runStats -> totWaitTime / (double)numCust,
	 driver -> runStats -> maxQueueSize, closeTime);
  
  destroyDriver(driver);
  return 0;
}
