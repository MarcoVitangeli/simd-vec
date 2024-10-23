#ifndef SIMD_STRING_H
#define SIMD_STRING_H

#define SIMD_STRING_TRUE 1
#define SIMD_STRING_FALSE 0
#define SIMD_BOOL unsigned char

#include <immintrin.h>
#include <stdint.h>

#define BYTE_LENGTH 16 // 128bits

typedef struct
{
    __m128i *data;
    size_t len;
} simd_str;

void simd_str_init(simd_str *, const char *, size_t);
void simd_str_free(simd_str *s);
void simd_str_c_str(simd_str *s, char *buff);
SIMD_BOOL simd_str_eq(simd_str *s1, simd_str *s2);

SIMD_BOOL simd_str_eq(simd_str *s1, simd_str *s2)
{
    if (s1->len != s2->len)
    {
        return SIMD_STRING_FALSE;
    }

    size_t fc = s1->len / BYTE_LENGTH;
    size_t rmd = s1->len % BYTE_LENGTH;
    size_t i;

    for (i = 0; i < fc; i++)
    {
        __m128i vcmp = _mm_cmpeq_epi8(s1->data[i], s2->data[i]);
        int mask = _mm_movemask_epi8(vcmp);
        if (mask != 0xFFFF)
        {
            return SIMD_STRING_FALSE;
        }
    }

    if (rmd)
    {
        for (size_t j = 0; j < rmd; j++)
        {
            if (s1->data[i][j] != s2->data[i][j])
            {
                return SIMD_STRING_FALSE;
            }
        }
    }

    return SIMD_STRING_TRUE;
}

void simd_str_c_str(simd_str *s, char *buff)
{
    size_t fc = s->len / BYTE_LENGTH;
    size_t rm = s->len % BYTE_LENGTH;

    for (size_t i = 0; i < fc; i++)
    {
        _mm_storeu_si128((__m128i *)buff, s->data[i]);
        buff += BYTE_LENGTH;
    }

    if (rm)
    {
        char tmp[BYTE_LENGTH];
        _mm_storeu_si128((__m128i *)tmp, s->data[fc]);
        for (size_t j = 0; j < rm; j++)
        {
            buff[j] = tmp[j];
        }
    }
}

void simd_str_free(simd_str *s)
{
    free(s->data);
}

void simd_str_init(simd_str *s, const char *src, size_t len)
{
    size_t cap = (len + BYTE_LENGTH - 1) / BYTE_LENGTH;
    s->data = _mm_malloc(sizeof(__m128i) * cap, 32);
    s->len = len;

    size_t i;
    for (i = 0; i < len / BYTE_LENGTH; i++)
    {
        s->data[i] = _mm_loadu_si128((__m128i const *)src);
        src += BYTE_LENGTH;
    }

    if (len % BYTE_LENGTH)
    {
        char tmp[BYTE_LENGTH] = {0};
        size_t remaining = len % BYTE_LENGTH;
        for (size_t j = 0; j < remaining; j++)
        {
            tmp[j] = src[j];
        }
        s->data[i] = _mm_loadu_si128((__m128i const *)tmp);
    }
}

#endif // SIMD_STRING_H
