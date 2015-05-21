#include <stdio.h>
#include <stdbool.h>
#include "util.h"

/**
 * Convert minutes to hour/time expression.
 * if duration is false, str will get HH:MM format of timestamp.
 * if duration is true, str will get HH Hrs MM min format of elapsed time.
 */
void minuteToHrs(int min, char *str, bool duration) {
  if (duration)
    sprintf(str, "%d Hrs %d min", min / 60, min % 60);
  else
    sprintf(str, "%02d:%02d", min / 60, min % 60);
}
