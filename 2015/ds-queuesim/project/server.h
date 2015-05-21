#ifndef __SERVER_H
#define __SERVER_H

#include "driver.h"

typedef struct server_struct {
  int custNum;
  int arriveTime;
  int startTime;
  int svcTime;

  //Private variables. DO NOT TOUCH IT.
  int svcTimeMin;
  int svcTimeMax;
  int queueSize;
  bool isServing;
} SERVER;

SERVER *createServer(int svcTimeMin, int svcTimeMax);
int serverServe(SERVER *server, CUSTOMER *customer, int queueSize, int curTime);
int serverComplete(SERVER *server, DRIVERSTAT *stat, int curTime);

#endif
