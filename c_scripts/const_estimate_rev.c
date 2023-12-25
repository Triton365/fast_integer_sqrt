#include <stdio.h>
#include "isqrt.c"


void find_best_const_estimate_rev(int64_t x, int64_t *outendx, int64_t *outconstmin, int64_t *outconstmax) {
    int64_t sqrtx = isqrt(x);
    int64_t sqrtnext = x - sqrtx*sqrtx;
    int64_t lower = herons_lowerbound(x,sqrtx);
    int64_t upper = herons_upperbound(x,sqrtx);
    int64_t t;
    for (x=x-1; x>=0; x--) {
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx--;
            sqrtnext = sqrtx<<1;
        }
        if (sqrtx != herons_method(x,lower)) {
            t = herons_lowerbound(x,sqrtx);
            if (upper < t || t < lower) break;
            lower = t;
        }
        if (sqrtx != herons_method(x,upper)) {
            upper = herons_upperbound(x,sqrtx);
        }
    }
    *outendx = x+1;
    *outconstmin = lower;
    *outconstmax = upper;
}

int main(void) {
    int64_t startx, endx, constmin, constmax;

    herons_loop = 3;
    startx = 798344891;

    while (startx >= 0) {
        find_best_const_estimate_rev(startx,&endx,&constmin,&constmax);
        if (constmin == constmax)
            printf("if x matches %lld..%lld : estimate = %lld\n",endx,startx,constmin);
        else
            printf("if x matches %lld..%lld : estimate = [%lld~%lld]\n",endx,startx,constmin,constmax);
        startx = endx-1;
    }
    printf("end");
    return 0;
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