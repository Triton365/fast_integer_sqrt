#include <stdio.h>
#include <stdbool.h>
#include "isqrt.c"


bool linear_touch_ceil_rev(int64_t startx, int64_t divisior, int64_t *bestendx, int64_t *bestdiv, int64_t *bestaddmin, int64_t *bestaddmax) {
    int64_t divm1 = divisior-1;
    int64_t x = startx;
    int64_t estimate = x/divisior;
    int64_t estimatenext = x%divisior;
    int64_t sqrtx = isqrt(x);
    int64_t sqrtnext = x - sqrtx*sqrtx;
    int64_t lower = herons_lowerbound(x,sqrtx)-estimate;
    int64_t upper = herons_upperbound(x,sqrtx)-estimate;
    int64_t t;
    

    for (x=x-1; x>=0; x--) {
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
        if (sqrtx != herons_method(x,estimate+lower)) {
            t = herons_lowerbound(x,sqrtx)-estimate;
            if (upper < t || t < lower) break;
            lower = t;
        }
        if (sqrtx != herons_method(x,estimate+upper)) {
            upper = herons_upperbound(x,sqrtx)-estimate;
        }
    }
    x++;
    if (x < (*bestendx)) {
        *bestendx = x;
        *bestdiv = divisior;
        *bestaddmin = lower;
        *bestaddmax = upper;
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
        printf("searching div=%lld~%lld\n",divmin,divmax);
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


int main(void) {
    int64_t startx=0,endx=0,div,bestdiv,addmin,addmax;

    herons_loop = 2;
    startx = 2147483647;

    while (startx >= 0) {
        find_best_linear_estimate_rev(startx,&endx,&div,&addmin,&addmax);
        if (addmin == addmax)
            printf("if score x matches %lld..%lld : estimate = x/%lld + %lld\n",endx,startx,div,addmin);
        else
            printf("if score x matches %lld..%lld : estimate = x/%lld + [%lld~%lld]\n",endx,startx,div,addmin,addmax);
        startx = endx-1;
    }
    printf("end");
    return 0;
}

/*
herons_loop = 3;
if score x matches 0..1515359 : estimate = x/559 + 15
if score x matches 1515360..2147483647 : estimate = x/32768 + [2456~2677]

herons_loop = 2;
if score x matches 0..30967 : estimate = x/112 + 7
if score x matches 30968..5560119 : estimate = x/2029 + 267
if score x matches 5560120..232440383 : estimate = x/15392 + 2645
if score x matches 232440384..2147483647 : estimate = x/65536 + [12666~14199]

herons_loop = 1;
if score x matches 0..781 : estimate = x/23 + 3
if score x matches 782..24956 : estimate = x/172 + 32
if score x matches 24957..264135 : estimate = x/644 + 138
if score x matches 264136..1615382 : estimate = x/1742 + 396
if score x matches 1615383..7006574 : estimate = x/3856 + 904
if score x matches 7006575..24088280 : estimate = x/7464 + 1782
if score x matches 24088281..70123724 : estimate = x/13166 + 3179
if score x matches 70123725..179720752 : estimate = x/21632 + 5263
if score x matches 179720753..416771846 : estimate = x/33632 + 8227
if score x matches 416771847..891917930 : estimate = x/50054 + 12292
if score x matches 891917931..1786161014 : estimate = x/71860 + 17699
if score x matches 1786161015..2147483647 : estimate = x/98304 + [24193~24385]
*/