#include <stdio.h>
#include <time.h>
#include "simd_vec.h"

#define ARRAY_SIZE 100000000

clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define PRINTTIME printf("%6.3f seconds used by the processor for SIMD version.\n", ((double)stopm-startm)/CLOCKS_PER_SEC);

float RandomFloat(float min, float max) {
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

int main() {
    srand(time(NULL));
    simd_vec_t v1, v2, res;

    simd_vec_new(&v1, ARRAY_SIZE);
    simd_vec_new(&v2, ARRAY_SIZE);
    simd_vec_new(&res, ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        float f1 = RandomFloat(0.0f, 1000000.0f);
        float f2 = RandomFloat(0.0f, 1000000.0f);
        simd_vec_append(&v1, f1);
        simd_vec_append(&v2, f2);
    }

    START;
    float f;
    simd_vec_add(&v1, &v2, &res);
    simd_vec_sum(&res, &f);

    printf("SUM: %f\n", f);
    STOP;
    PRINTTIME;

    return 0;
}