#include <stdio.h>
#include <stdlib.h>
#include "isqrt.h"


void find_best_const_estimate(int64_t startx, int64_t *outendx, int64_t *outconstmin, int64_t *outconstmax) {
    int32_t x = (int32_t)startx;
    int32_t sqrtx = isqrt(x);
    int32_t sqrtnext = (sqrtx<<1) - x + sqrtx*sqrtx;
    int32_t lower = herons_lowerbound(x,sqrtx);
    int32_t upper = herons_upperbound(x,sqrtx);
    int32_t t;
    while (1) {
        if (x==2147483647) break;
        x++;
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx++;
            sqrtnext = sqrtx<<1;
        }
        if (!herons_method_check(x,lower,sqrtx)) {
            t = herons_lowerbound(x,sqrtx);
            if (upper < t || t < lower) { x--; break; }
            lower = t;
        }
        if (!herons_method_check(x,upper,sqrtx)) {
            upper = herons_upperbound(x,sqrtx);
        }
    }
    *outendx = (int64_t)x;
    *outconstmin = (int64_t)lower;
    *outconstmax = (int64_t)upper;
}

/*
herons_loop = 3;

if score x matches 0..2911 : estimate = 15
if score x matches 2912..226567 : estimate = 192
if score x matches 226568..5904860 : estimate = 1179
if score x matches 5904861..84015392 : estimate = 5006
if score x matches 84015393..798344891 : estimate = 16781
if score x matches 798344892..2147483647 : estimate = [28431~47572]
*/