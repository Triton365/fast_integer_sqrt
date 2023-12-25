#include <stdio.h>
#include "isqrt.c"


void find_best_const_estimate(int64_t x, int64_t *outendx, int64_t *outconstmin, int64_t *outconstmax) {
    int64_t sqrtx = isqrt(x);
    int64_t sqrtnext = (sqrtx<<1) - x + sqrtx*sqrtx;
    int64_t lower = herons_lowerbound(x,sqrtx);
    int64_t upper = herons_upperbound(x,sqrtx);
    int64_t t;
    for (x=x+1; x<=2147483647; x++) {
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx++;
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
    *outendx = x-1;
    *outconstmin = lower;
    *outconstmax = upper;
}

int main(void) {
    int64_t startx, endx, constmin, constmax;

    herons_loop = 3;
    startx = 0;

    while (startx <= 2147483647) {
        find_best_const_estimate(startx,&endx,&constmin,&constmax);
        if (constmin == constmax)
            printf("if x matches %lld..%lld : estimate = %lld\n",startx,endx,constmin);
        else
            printf("if x matches %lld..%lld : estimate = [%lld~%lld]\n",startx,endx,constmin,constmax);
        startx = endx+1;
    }
    printf("end");
    return 0;
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