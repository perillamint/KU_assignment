#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "driver.h"
#include "queue.h"

/**
 * Static function - private method.
 * Get current time using DRIVER objects elapsed time and openTime.
 * Returns current time in minute.
 */
static int _getCurTime (DRIVER *driver) {
  return driver -> elapsedTime + driver -> openTime;
}

/**
 * Static function - private method.
 * Generate customer with given probablity.
 * custGenRate is reciprocal of probablity.
 */
static void _genCustomer (DRIVER *driver) {
  if (_getCurTime(driver) > driver -> closeTime)
    return;
  
  if (rand() % driver -> custGenRate == 0) {
    driver -> custGenCnt ++;

    CUSTOMER *customer = malloc(sizeof(CUSTOMER));
    
    customer -> custNum = driver -> custGenCnt;
    customer -> arriveTime = _getCurTime (driver);
    
    enQueue(driver -> custQueue, customer);
  }
}

/**
 * This function returns initialized driver object.
 */
DRIVER *createDriver (int openTime, int closeTime, int custGenRate,
		      int svcTimeMin, int svcTimeMax, int serverCnt) {
  DRIVER *driver = malloc(sizeof(DRIVER));
  DRIVERSTAT *dstat = malloc(sizeof(DRIVER));
  QUEUE *cqueue = createQueue();
  int i;

  for (i = 0; i < serverCnt; i++) {
    driver -> serverArr[i] = createServer(svcTimeMin, svcTimeMax);
  }

  dstat -> numCust = 0;
  dstat -> totSvcTime = 0;
  dstat -> totWaitTime = 0;
  dstat -> maxQueueSize = 0;
  
  driver -> runStats = dstat;
  driver -> custQueue = cqueue;
  driver -> serverCnt = serverCnt;
  driver -> openTime = openTime;
  driver -> closeTime = closeTime;
  driver -> custGenRate = custGenRate;
  driver -> custGenCnt = 0;
  driver -> elapsedTime = 0;

  return driver;
}

/**
 * This function destorys customer and free it.
 */
void destroyCustomer (CUSTOMER *customer) {
  free(customer);
}

/**
 * This function destorys driver and free it.
 */
void destroyDriver (DRIVER *driver) {
  CUSTOMER **cust = NULL;
  
  while (!isEmpty(driver -> custQueue)) {
    deQueue(driver -> custQueue, (void*)cust);
    destroyCustomer(*cust);
  }

  destroyQueue(driver -> custQueue);
  free(driver -> runStats);
  free(driver);
}

/**
 * This function do one tick of simulation.
 * it returns true when simulation is completed.
 */
bool driverDoTick(DRIVER *driver) {
  //Do simulation stuff.
  CUSTOMER *cust = NULL;
  int ret;
  bool isDone = true;
  int i;

  _genCustomer(driver);

  if (queueCount(driver -> custQueue) > driver -> runStats -> maxQueueSize)
    driver -> runStats -> maxQueueSize = queueCount(driver -> custQueue);

  for (i = 0; i < driver -> serverCnt; i++) {
    if (isEmpty(driver -> custQueue))
      break;

    queueFront(driver -> custQueue, (void**)&cust);
    
    ret = serverServe(driver -> serverArr[i], cust,
		      queueCount(driver -> custQueue), _getCurTime(driver));

    if(ret == 0) {
      deQueue(driver -> custQueue, NULL);
    } else {
      isDone = false;
    }
    
    ret = serverComplete(driver -> serverArr[i], driver -> runStats,
		   _getCurTime(driver));

    if(ret == -1) {
      isDone = false;
    }
  }

  if (_getCurTime(driver) < driver -> closeTime) {
    isDone = false;
  }

  if (!isEmpty(driver -> custQueue)) {
    isDone = false;
  }

  driver -> elapsedTime ++;
  
  return isDone;
}
