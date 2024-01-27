#ifndef ISQRT_H
#define ISQRT_H

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#define isqrt(x) ((int32_t)sqrt(x))
#define floordiv(x,y) (((y)==0) ? (x) : ( (x)/(y) - ((((x)^(y)) < 0) ? ((x)%(y) != 0) : (0) )))
#define ufloordiv(x,y) (((y)==0) ? (x) : (x/y))

#ifndef HERONS_LOOP
    #define HERONS_LOOP 2
#endif


static inline int32_t herons_method(int32_t x, int32_t estimate) {
    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Heron's_method
    #if HERONS_LOOP >= 5
        for (int32_t i=HERONS_LOOP; i>4; i--)
            estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 4
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 3
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 2
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 1
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    return (floordiv(x,estimate) < estimate) ? (estimate-1) : (estimate);
}

static inline bool herons_method_check(int32_t x, int32_t estimate, int32_t y) {
    #if HERONS_LOOP >= 5
    for (int32_t i=HERONS_LOOP; i>4; i--)
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 4
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 3
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 2
        estimate = (estimate+floordiv(x,estimate))>>1;
    #endif
    #if HERONS_LOOP >= 1
        estimate = estimate + floordiv(x,estimate) - (y<<1);
        return (0 <= estimate && estimate <= 3);
    #endif
    #if HERONS_LOOP == 0
        return (estimate == y || estimate-1 == y);
    #endif
}

int32_t herons_lowerbound(int32_t x, int32_t y) {
    int32_t left=1, right=y;
    if (left >= right) return left;
    int32_t mid = (left+right)>>1;
    while (1) {
        if (herons_method_check(x,mid,y)) {
            if (left==mid) return mid;
            right = mid;
            mid += left;
        } else {
            if (right==++mid) return mid;
            left = mid;
            mid += right;
        }
        mid >>= 1;
    }
}

int32_t herons_upperbound(int32_t x, int32_t y) {
    int32_t left=y+1, right=100000;
    int32_t mid = (left+right)>>1;
    while (1) {
        if (herons_method_check(x,mid,y)) {
            if (right==++mid) return mid-1;
            left = mid;
            mid += right;
        } else {
            if (left==mid) return mid-1;
            right = mid;
            mid += left;
        }
        mid >>= 1;
    }
}
#endif