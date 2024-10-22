#ifndef SIMD_VEC_H
#define SIMD_VEC_H
#include <immintrin.h>

#define SIMD_VEC_DATA_SIZE 8 // 8 floats in ___m256
#define SIMD_VEC_IDX(v) (v / SIMD_VEC_DATA_SIZE)
#define SIMD_VEC_ELEM_IDX(v) (v % SIMD_VEC_DATA_SIZE)
#define SIMD_256_CAP(cap) ((cap + SIMD_VEC_DATA_SIZE - 1) / SIMD_VEC_DATA_SIZE)
#define SIMD_VEC_ELEM(v, idx) (v[SIMD_VEC_IDX(idx)][SIMD_VEC_ELEM_IDX(idx)])

// ERROR CODES
typedef size_t simd_vec_ret_t;

static const simd_vec_ret_t SIMD_VEC_OK = 0;
static const simd_vec_ret_t SIMD_VEC_ERR_NO_MEM = 1;
static const simd_vec_ret_t SIMD_VEC_ERR_INVALID_ARG = 3;
static const simd_vec_ret_t SIMD_VEC_ERR_NOT_IMPLEMENTED = 4;
static const simd_vec_ret_t SIMD_VEC_ERR_MAX_CAPACITY = 5;
static const simd_vec_ret_t SIMD_VEC_ERR_INVALID_IDX = 6;

typedef struct {
    __m256* data;
    size_t cap;
    size_t len;
    size_t max_len;
} simd_vec_t;

simd_vec_ret_t simd_vec_new(simd_vec_t*, size_t cap);
void simd_vec_free(const simd_vec_t* vec);
void simd_vec_zero(simd_vec_t*);
simd_vec_ret_t simd_vec_append(simd_vec_t*,float);
simd_vec_ret_t simd_vec_insert_at(simd_vec_t*, size_t, float);
void simd_vec_reset(simd_vec_t*);
simd_vec_ret_t simd_vec_add(const simd_vec_t*,const simd_vec_t*,simd_vec_t*);
simd_vec_ret_t simd_vec_value_at(const simd_vec_t*,size_t,float*);
simd_vec_ret_t simd_vec_sum(const simd_vec_t*, float*);
simd_vec_ret_t simd_vec_dp(const simd_vec_t*,const simd_vec_t*, float*);

// DEBUG UTILITIES
void simd_vec_debug_stdout(const simd_vec_t*);

#endif // SIMD_VEC_H
