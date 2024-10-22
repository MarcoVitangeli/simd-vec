#include <stdio.h>
#include <time.h>
#include "simd_vec.h"

#define ARRAY_SIZE 100000000

float RandomFloat(float min, float max){
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

int main() {
    srand(time(NULL));
    simd_vec_t v1, v2, res;


    printf("START\n");
    simd_vec_new(&v1, ARRAY_SIZE);
    simd_vec_new(&v2, ARRAY_SIZE);
    simd_vec_new(&res, ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        float f1 = RandomFloat(0.0f, 1000000.0f);
        float f2 = RandomFloat(0.0f, 1000000.0f);
        simd_vec_append(&v1, f1);
        simd_vec_append(&v2, f2);
    }

    simd_vec_add(&v1, &v2, &res);

    float f;
    simd_vec_sum(&res, &f);

    printf("SUM: %f\n", f);
    printf("END\n");

    return 0;
}