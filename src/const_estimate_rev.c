#include <stdio.h>
#include <stdlib.h>
#include "isqrt.h"


void find_best_const_estimate_rev(int64_t startx, int64_t *outendx, int64_t *outconstmin, int64_t *outconstmax) {
    int32_t x = (int32_t)startx;
    int32_t sqrtx = isqrt(x);
    int32_t sqrtnext = x - sqrtx*sqrtx;
    int32_t lower = herons_lowerbound(x,sqrtx);
    int32_t upper = herons_upperbound(x,sqrtx);
    int32_t t;
    for (x=x-1; x>=0; x--) {
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx--;
            sqrtnext = sqrtx<<1;
        }
        if (!herons_method_check(x,lower,sqrtx)) {
            t = herons_lowerbound(x,sqrtx);
            if (upper < t || t < lower) break;
            lower = t;
        }
        if (!herons_method_check(x,upper,sqrtx)) {
            upper = herons_upperbound(x,sqrtx);
        }
    }
    *outendx = (int64_t)(x+1);
    *outconstmin = (int64_t)lower;
    *outconstmax = (int64_t)upper;
}

/*
herons_loop = 3;

if x matches 263250625..2147483647 : estimate = 28431
if x matches 22953681..263250624 : estimate = [9260~9262]
if x matches 1216609..22953680 : estimate = 2480
if x matches 28900..1216608 : estimate = 494
if x matches 121..28899 : estimate = [59~65]
if x matches 0..120 : estimate = [3~15]
*/