#include <stdio.h>
#include <stdlib.h>
#include "driver.h"
#include "server.h"
#include "util.h"

/**
 * Static function - private method.
 * Generate service time using svcTimeMax and svcTimeMin.
 * Generated result follows constant distribution.
 */
static int _getSvcTime(SERVER *server) {
  int timediff = server -> svcTimeMax - server -> svcTimeMin;
  int randval = rand() % (timediff + 1);

  return randval + server -> svcTimeMin;
}

/**
 * Create and initialize server object.
 */
SERVER *createServer(int svcTimeMin, int svcTimeMax) {
  SERVER *server = malloc(sizeof(SERVER));
  
  server -> custNum = 0;
  server -> svcTime = 0;
  server -> startTime = 0;
  server -> arriveTime = 0;
  server -> svcTimeMax = svcTimeMax;
  server -> svcTimeMin = svcTimeMin;
  server -> isServing = false;

  return server;
}

/**
 * Try to allocate customer to server.
 * If server is idle state, it accepts customer and free it then return 0.
 * If server is busy state, it returns -1.
 */
int serverServe(SERVER *server, CUSTOMER *customer, int queueSize, int curTime) {
  if (server -> isServing)
    return -1;

  server -> svcTime = _getSvcTime(server);
  server -> custNum = customer -> custNum;
  server -> arriveTime = customer -> arriveTime;
  server -> startTime = curTime;
  server -> queueSize = queueSize; 
  server -> isServing = true;

  destroyCustomer(customer);
  return 0;
}

/**
 * Check server finished their job.
 * If server is idle state, it records statics in driverstat and returns 0.
 * If server is busy state, it returns -1.
 */
int serverComplete(SERVER *server, DRIVERSTAT *stat, int curTime) {
  char arrived[128] = { 0 };
  char served[128] = { 0 };
  char leaved[128] = { 0 }; 
  char serviced[128] = { 0 };
  char waits[128] = { 0 };
  
  if (!server -> isServing)
    return 0;

  if (server -> startTime + server -> svcTime > curTime)
    return -1;

  stat -> numCust ++;
  stat -> totSvcTime += server -> svcTime;
  stat -> totWaitTime += server -> startTime - server -> arriveTime;

  minuteToHrs(server -> arriveTime, arrived, false);
  minuteToHrs(server -> startTime, served, false);
  minuteToHrs(server -> startTime + server -> svcTime, leaved, false);
  minuteToHrs(server -> svcTime, serviced, true);
  minuteToHrs(server -> startTime - server -> arriveTime, waits, true);
  
  printf("Customer #%d\n"
	 "Arrived at      : %s\n"
	 "Served at       : %s\n"
	 "Leaved at       : %s\n"
	 "Serviced during : %s\n"
	 "Waits during    : %s\n"
	 "Queue size      : %d\n\n",
	 server -> custNum, arrived, served, leaved, serviced, waits
	 , server -> queueSize);

  server -> svcTime = 0;
  server -> custNum = 0;
  server -> arriveTime = 0;
  server -> startTime = 0;
  server -> isServing = false;
  return 0;
}
