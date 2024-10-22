#include "simd_vec.h"

#include <stdint.h>
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))

simd_vec_ret_t simd_vec_new(simd_vec_t* v, const size_t cap) {
    if (!v) {
        return SIMD_VEC_ERR_INVALID_ARG;
    }
    v->cap = SIMD_256_CAP(cap);
    v->len = 0;
    v->max_len = cap;
    v->data = _mm_malloc(sizeof(__m256) * v->cap, 32);

    if (!v->data) {
        return SIMD_VEC_ERR_NO_MEM;
    }

    simd_vec_zero(v);

    return SIMD_VEC_OK;
}

void simd_vec_free(const simd_vec_t* vec) {
    free(vec->data);
}

void simd_vec_zero(simd_vec_t* v) {
    for(int i = 0; i < v->cap; i++) {
        v->data[i] = _mm256_setzero_ps();
    }
}

simd_vec_ret_t simd_vec_append(simd_vec_t* v, float val) {
    if (v->len == v->max_len) {
        return SIMD_VEC_ERR_MAX_CAPACITY;
    }

    SIMD_VEC_ELEM(v->data, v->len) = val;
    v->len++;
    return SIMD_VEC_OK;
}

simd_vec_ret_t simd_vec_insert_at(simd_vec_t* v, size_t idx, float val) {
    if (idx == 0 || idx >= v->len) {
        return SIMD_VEC_ERR_INVALID_IDX;
    }

    SIMD_VEC_ELEM(v->data, idx) = val;

    return SIMD_VEC_OK;
}

void simd_vec_reset(simd_vec_t* v) {
    v->len = 0;
}

simd_vec_ret_t simd_vec_add(const simd_vec_t* v1, const simd_vec_t* v2, simd_vec_t* dst) {
    if (v1->len != v2->len || v1->cap > dst->cap) {
        return SIMD_VEC_ERR_INVALID_IDX;
    }
    for (size_t i = 0; i < v1->cap; i++) {
        dst->data[i] = _mm256_add_ps(v1->data[i], v2->data[i]);
    }
    dst->len = v1->len;

    return SIMD_VEC_OK;
}

simd_vec_ret_t simd_vec_value_at(const simd_vec_t* v, size_t idx, float* val) {
    if (!v) {
        return SIMD_VEC_ERR_INVALID_ARG;
    }

    if (idx >= v->len) {
        return SIMD_VEC_ERR_INVALID_IDX;
    }

    *val = SIMD_VEC_ELEM(v->data, idx);

    return SIMD_VEC_OK;
}

simd_vec_ret_t simd_vec_sum(const simd_vec_t* v, float* f) {
    *f = 0.0f;
    for(size_t i = 0; i < v->cap; i++)
        *f = SIMD_VEC_ELEM(v->data, 0) + SIMD_VEC_ELEM(v->data, 1) + SIMD_VEC_ELEM(v->data, 2) + SIMD_VEC_ELEM(v->data, 3);

    return SIMD_VEC_OK;
}

__m256 simd_vec_reverse(__m256 v) {
    return _mm256_permutevar8x32_ps(v, _mm256_set_epi32(0,1,2,3,4,5,6,7));
}

uint8_t get_mask(size_t rmd) {
    // switch (rmd) {
    //     case 0:
    //         return 0x0F; // 0000 1111
    //     case 1:
    //         return 0x1F; // 0001 1111
    //     case 2:
    //         return 0x3F; // 0011 1111
    //     case 3:
    //         return 0x7F; // 0111 1111
    //     case 4:
    //         return 0xFF; // 1111 1111
    // }
    return (0xFF >> (4-rmd)) & 0xFF;
}

simd_vec_ret_t simd_vec_dp(const simd_vec_t* v1,const simd_vec_t* v2, float* res) {
    if (v1->len != v2->len) {
        return SIMD_VEC_ERR_INVALID_IDX;
    }
    float acc = 0;

    for (size_t i = 0; i < v1->cap; i++) {
        __m256 lh = _mm256_dp_ps(v1->data[i], v2->data[i], 0xFF);
        __m256 rh = _mm256_dp_ps(simd_vec_reverse(v2->data[i]), simd_vec_reverse(v1->data[i]), 0xFF);
        acc += lh[0] + rh[0];
    }

    *res = acc;
    return SIMD_VEC_OK;
}

// DEBUG UTILITIES
void simd_vec_debug_stdout(const simd_vec_t* v) {
    printf("CAPACITY: %lu\n", v->cap);
    printf("LEN: %lu\n", v->len);

    for (int i = 0; i < v->len; i++) {
        printf("INDEX: %d\t%d\t%d\n", i, SIMD_VEC_IDX(i), SIMD_VEC_ELEM_IDX(i));
        printf("%f\n", SIMD_VEC_ELEM(v->data, i));
    }
}