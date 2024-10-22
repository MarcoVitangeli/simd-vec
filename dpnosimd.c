#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE 500000000

clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define PRINTTIME printf("%6.3f seconds used by the processor by nosimd version.\n", ((double)stopm-startm)/CLOCKS_PER_SEC);

float RandomFloat(float min, float max){
  return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

int main() {
  srand(time(NULL));

  float* f1 = calloc(sizeof(float), ARRAY_SIZE);
  float* f2 = calloc(sizeof(float), ARRAY_SIZE);

  for (int i = 0; i < ARRAY_SIZE; i++) {
    f1[i] = RandomFloat(0.0f, 100000.0f);
    f2[i] = RandomFloat(0.0f, 100000.0f);
  }

  START;
  float acc = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    acc += f1[i] * f2[i];
  }

  STOP;
  printf("DP = %f\n", acc);
  PRINTTIME

  return 0;
}