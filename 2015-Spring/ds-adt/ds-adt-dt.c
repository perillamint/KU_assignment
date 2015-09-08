#include <stdio.h>

int main (int argc, char **argv) {
  //Allocate 8 byte (= 64-bit) memory in stack.
  char chData[8] = {0x00, };

  //Cast it to other data types.

  int *nData = chData;
  float *fData = chData;
  //in AMD64, long means 64-bit int.
  long *lData = chData;

  //Set data in chData and read it.
  printf("All bits in chData is zero.\n");
  printf("int = %d, float = %f, long = %ld\n", *nData, *fData, *lData);

  chData[0] = 42;
  printf("nData will be 42. but fData will have different value then nData.\n");
  printf("It means each data type defines \"how to read the value\" from memory.\n");
  printf("int = %d, float = %f, long = %ld\n", *nData, *fData, *lData);

  chData[4] = 42;
  printf("Now, lData have different value then nData\n");
  printf("int = %d, float = %f, long = %ld\n", *nData, *fData, *lData);
  
  return 0;
}
