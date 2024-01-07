#include <stdio.h>
#include <stdlib.h>
#include "isqrt.h"


bool linear_touch_ceil_rev(int64_t startx, int64_t div, int64_t *bestendx, int64_t *bestdiv, int64_t *bestaddmin, int64_t *bestaddmax) {
    int32_t divisior = (int32_t)div;
    if (divisior == 0) divisior = 1;
    int32_t divm1 = divisior-1;
    int32_t x = (int32_t)startx;
    int32_t estimate = x/divisior;
    int32_t estimatenext = x%divisior;
    int32_t sqrtx = isqrt(x);
    int32_t sqrtnext = x - sqrtx*sqrtx;
    int32_t lower = herons_lowerbound(x,sqrtx)-estimate;
    int32_t upper = herons_upperbound(x,sqrtx)-estimate;
    int32_t t;
    
    while (1) {
        x--;
        if (x == -1) break;
        estimatenext--;
        if (estimatenext == -1) {
            estimate--;
            estimatenext = divm1;
        }
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx--;
            sqrtnext = sqrtx<<1;
        }
        if (!herons_method_check(x,estimate+lower,sqrtx)) {
            t = herons_lowerbound(x,sqrtx)-estimate;
            if (upper < t || t < lower) break;
            lower = t;
        }
        if (!herons_method_check(x,estimate+upper,sqrtx)) {
            upper = herons_upperbound(x,sqrtx)-estimate;
        }
    }
    x++;
    if (x < (*bestendx)) {
        *bestendx = (int64_t)x;
        *bestdiv = (int64_t)div;
        *bestaddmin = (int64_t)lower;
        *bestaddmax = (int64_t)upper;
        if (x==0) return true;
    }
    x--;
    estimate = x/divisior + upper;
    upper = herons_upperbound(x,isqrt(x));
    return (estimate > upper);
}

void find_best_linear_estimate_rev(int64_t startx, int64_t *outendx, int64_t *outdiv, int64_t *outaddmin, int64_t *outaddmax) {
    int64_t divmin = 1;
    int64_t divmax = 2147483647;
    int64_t divmid,bestendx=startx,bestdiv,bestaddmin,bestaddmax;
    while (1) {
        // printf("searching div=%lld~%lld\n",divmin,divmax);
        divmid = (divmin+divmax)>>1;
        bool check = linear_touch_ceil_rev(startx,divmid,&bestendx,&bestdiv,&bestaddmin,&bestaddmax);
        if (bestendx <= 0) break;
        if (check)
            divmax = divmid-1;
        else
            divmin = divmid+1;
        if (divmin > divmax)
            break;
    }
    *outdiv = bestdiv;
    *outaddmin = bestaddmin;
    *outaddmax = bestaddmax;
    *outendx = bestendx;
    return;
}

/*
#define HERONS_LOOP 2
if score x matches 93605625..2147483647 : estimate = x/50704 + 9628
if score x matches 1669264..93605624 : estimate = x/9424 + 1538
if score x matches 4624..1669263 : estimate = x/1048 + 122
if score x matches 0..4623 : estimate = x/64 + [4~7]

#define HERONS_LOOP 1
if score x matches 1090518529..2147483647 : estimate = x/79088 + 19493
if score x matches 520068025..1090518528 : estimate = x/55598 + 13666
if score x matches 229734649..520068024 : estimate = x/37772 + 9251
if score x matches 92428996..229734648 : estimate = x/24614 + 5999
if score x matches 33028009..92428995 : estimate = x/15240 + 3689
if score x matches 10118761..33028008 : estimate = x/8836 + 2117
if score x matches 2515396..10118760 : estimate = x/4700 + 1109
if score x matches 462400..2515395 : estimate = x/2220 + 510
if score x matches 52441..462399 : estimate = x/882 + 193
if score x matches 2401..52440 : estimate = x/264 + 52
if score x matches 9..2400 : estimate = x/48 + 8
if score x matches 0..8 : estimate = x/1073741824 + [2~3]
*/