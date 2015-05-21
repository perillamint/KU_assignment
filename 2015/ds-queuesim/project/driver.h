#ifndef __DRIVER_H
#define __DRIVER_H

#include "queue.h"

#define MAXSRVCNT 8

typedef struct driver_stats {
  int numCust;
  int totSvcTime;
  int totWaitTime;
  int maxQueueSize;
} DRIVERSTAT;

typedef struct customer_struct {
  int custNum;
  int arriveTime;
} CUSTOMER;

#include "server.h"

typedef struct driver_struct {
  DRIVERSTAT *runStats;
  QUEUE *custQueue;
  int serverCnt;
  int openTime;
  int closeTime;
  int custGenRate;
  int custGenCnt;
  int elapsedTime;
  SERVER *serverArr[MAXSRVCNT];
} DRIVER;

DRIVER *createDriver (int openTime, int closeTime, int custGenRate,
		      int svcTimeMin, int svcTimeMax, int serverCnt);
void destroyDriver (DRIVER *driver);
void destroyCustomer (CUSTOMER *customer);
bool driverDoTick(DRIVER *driver);
#endif
