#include <stdio.h>
#include <stdlib.h>
#include "isqrt.h"
#include "libdivide.h"


bool rational_touch_ceil(bool is_div, int64_t ina, int64_t inb, int64_t startx, int64_t *outendx, int64_t *outcmin, int64_t *outcmax) {
    int32_t a = (int32_t)ina;
    int32_t b = (int32_t)inb;
    int32_t x = (int32_t)startx;
    int32_t ma = -a;
    int32_t sqrtx = isqrt(x);
    int32_t sqrtnext = (sqrtx<<1) - x + sqrtx*sqrtx;
    int32_t estimate;
    struct libdivide_s32_t fast_a;
    if (is_div) {
        fast_a = libdivide_s32_gen(a);
        estimate = libdivide_s32_do(x,&fast_a) + b;
        estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
    } else {
        estimate = x+b;
        estimate = ma/estimate - (ma%estimate!=0);
    }
    int32_t upper = herons_upperbound(x,sqrtx)-estimate;
    int32_t lower = herons_lowerbound(x,sqrtx)-estimate;
    int32_t t;
    for (x=x+1; x<=2147483647; x++) {
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx++;
            sqrtnext = sqrtx<<1;
        }
        if (is_div) {
            estimate = libdivide_s32_do(x,&fast_a) + b;
            estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
        } else {
            estimate = x+b;
            estimate = ma/estimate - (ma%estimate!=0);
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
    *outendx = x-1;
    *outcmin = lower;
    *outcmax = upper;
    if (is_div) {
        estimate = libdivide_s32_do(x,&fast_a) + b;
        estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
    } else {
        estimate = x+b;
        estimate = ma/estimate - (ma%estimate!=0);
    }
    estimate += upper;
    upper = herons_upperbound(x,isqrt(x));
    return (estimate > upper);
}


void find_best_rational_estimate_with_a(bool is_div, int64_t a, int64_t startx, int64_t *outendx, int64_t *outb, int64_t *outcmin, int64_t *outcmax) {
    int64_t minb = 1;
    int64_t maxb = 2147483647;
    int64_t endx,cmin,cmax;
    int64_t bestendx=-1,bestb=-1,bestcmin=-1,bestcmax=-1;
    // binary search
    while (1) {
        int64_t b = (minb+maxb)>>1;
        bool check = rational_touch_ceil(is_div,a,b,startx,&endx,&cmin,&cmax);
        if (bestendx < endx) {
            bestendx=endx; bestb=b; bestcmin=cmin; bestcmax=cmax;
            if (endx >= 2147483647) break;
        }
        if (check) {
            minb = b+1;
            if (minb == maxb) {
                rational_touch_ceil(is_div,a,minb,startx,&endx,&cmin,&cmax);
                if (bestendx < endx) {
                    bestendx=endx; bestb=minb; bestcmin=cmin; bestcmax=cmax;
                }
                break;
            }
        } else {
            maxb = b;
            if (minb == maxb) break;
        }
    }
    *outendx = bestendx;
    *outb = bestb;
    *outcmin = bestcmin;
    *outcmax = bestcmax;
    return;
}



#define min(x,y) ((x)>(y) ? (y) : (x))
#define max(x,y) ((x)<(y) ? (y) : (x))

void find_best_rational_estimate(int64_t test_range, int64_t startx, bool is_div, int64_t *outendx, int64_t *outa, int64_t *outb, int64_t *outcmin, int64_t *outcmax) {
    int64_t a, a_left, a_right;
    int64_t endx,b,cmin,cmax;
    int64_t bestendx=-1,besta=-1,bestb=-1,bestcmin=-1,bestcmax=-1;
    if (is_div) {
        a_left = 1;
        a_right = 16777215;
    } else {
        a_left = 1;
        a_right = 2147483647;
    }

    
    int64_t a_best_left=-1,a_best_right=-1,a_best_endx=-1;
    // printf("initial search a=[%lld~%lld]\n",a_left,a_right);
    for (a=test_range; a<=a_right; a=(int64_t)(a*1.5)+test_range) {
        // printf("a=%lld\n",a);
        int64_t a_endx = -1;
        for (int64_t i=0; i<test_range; i++) {
            find_best_rational_estimate_with_a(is_div,a-i,startx,&endx,&b,&cmin,&cmax);
            if (a_endx < endx)
                a_endx = endx;
            if (bestendx < endx) {
                // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a-i,b,cmin,cmax,endx);
                bestendx=endx; besta=a-i; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx >= 2147483647) goto lastreturn;
            }
        }
        if (a_best_endx < a_endx) {
            a_best_endx = a_endx;
            a_best_left = ((a-test_range)/1.5-test_range)/1.5;
            a_best_right = ((a*1.5)+test_range)*1.5+test_range;
        }
    }
    a_left = max(1,a_best_left);
    a_right = min(2147483647,a_best_right);
    // int64_t a_best_left = ((514-test_range)/1.5-test_range)/1.5;
    // int64_t a_best_right = ((514*1.5)+test_range)*1.5+test_range;
    // a_left = max(0,a_best_left);
    // a_right = min(2147483647,a_best_right);


    int64_t golden_offset = (int64_t)floor((a_right - a_left)*0.381966011250105);
    int64_t a_left_golden = a_left + golden_offset;
    int64_t a_right_golden = a_right - golden_offset;
    
    int64_t a_left_golden_left = max(a_left, a_left_golden - (test_range>>1));
    int64_t a_left_golden_right = a_left_golden_left + test_range - 1;
    int64_t a_right_golden_right = min(a_right, a_right_golden + (test_range>>1) - 1);
    int64_t a_right_golden_left = a_right_golden_right - test_range;

    if (a_left_golden_right+1 >= a_right_golden_left) {
        // printf("final search a=[%lld~%lld]\n",a_left,a_right);
        for (a=a_left; a<=a_right; a++) {
            find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (bestendx < endx) {
                // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx >= 2147483647) goto lastreturn;
            }
        }
    } else {
        // printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);

        int64_t a_left_golden_endx = -1;
        for (a=a_left_golden_left; a<=a_left_golden_right; a++) {
            find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (a_left_golden_endx < endx)
                a_left_golden_endx = endx;
            if (bestendx < endx) {
                // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx >= 2147483647) goto lastreturn;
            }
        }

        int64_t a_right_golden_endx = -1;
        for (a=a_right_golden_left; a<=a_right_golden_right; a++) {
            find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (a_right_golden_endx < endx)
                a_right_golden_endx = endx;
            if (bestendx < endx) {
                // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx >= 2147483647) goto lastreturn;
            }
        }

        while (1) {
            // printf("%lld vs %lld\n",a_left_golden_endx,a_right_golden_endx);
            if (a_left_golden_endx <= a_right_golden_endx) {
                a_left = a_left_golden;
                a_left_golden = a_right_golden;
                a_left_golden_left = a_right_golden_left;
                a_left_golden_right = a_right_golden_right;
                a_left_golden_endx = a_right_golden_endx;
                golden_offset = (int64_t)floor((a_right - a_left)*0.381966011250105);
                a_right_golden = a_right - golden_offset;
                a_right_golden_right = min(a_right, a_right_golden + (test_range>>1) - 1);
                a_right_golden_left = a_right_golden_right - test_range;
                if (a_left_golden_right+1 >= a_right_golden_left) {
                    // printf("final search a=[%lld~%lld]\n",a_left,a_right);
                    for (a=a_left; a<a_left_golden_left; a++) {
                        find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (bestendx < endx) {
                            // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx >= 2147483647) goto lastreturn;
                        }
                    }
                    for (a=a_left_golden_right+1; a<=a_right; a++) {
                        find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (bestendx < endx) {
                            // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx >= 2147483647) goto lastreturn;
                        }
                    }
                    goto lastreturn;
                }
                // printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);
                a_right_golden_endx = -1;
                for (a=a_right_golden_left; a<=a_right_golden_right; a++) {
                    find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                    if (a_right_golden_endx < endx)
                        a_right_golden_endx = endx;
                    if (bestendx < endx) {
                        // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                        bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                        if (bestendx >= 2147483647) goto lastreturn;
                    }
                }
            } else {
                a_right = a_right_golden;
                a_right_golden = a_left_golden;
                a_right_golden_left = a_left_golden_left;
                a_right_golden_right = a_left_golden_right;
                a_right_golden_endx = a_left_golden_endx;
                golden_offset = (int64_t)floor((a_right - a_left)*0.381966011250105);
                a_left_golden = a_left + golden_offset;
                a_left_golden_left = max(a_left, a_left_golden - (test_range>>1));
                a_left_golden_right = a_left_golden_left + test_range - 1;
                if (a_left_golden_right+1 >= a_right_golden_left) {
                    // printf("final search a=[%lld~%lld]\n",a_left,a_right);
                    for (a=a_left; a<a_right_golden_left; a++) {
                        find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (bestendx < endx) {
                            // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx >= 2147483647) goto lastreturn;
                        }
                    }
                    for (a=a_right_golden_right+1; a<=a_right; a++) {
                        find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (bestendx < endx) {
                            // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx >= 2147483647) goto lastreturn;
                        }
                    }
                    goto lastreturn;
                }
                // printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);
                a_left_golden_endx = -1;
                for (a=a_left_golden_left; a<=a_left_golden_right; a++) {
                    find_best_rational_estimate_with_a(is_div,a,startx,&endx,&b,&cmin,&cmax);
                    if (a_left_golden_endx < endx)
                        a_left_golden_endx = endx;
                    if (bestendx < endx) {
                        // printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                        bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                        if (bestendx >= 2147483647) goto lastreturn;
                    }
                }
            }
        }
    }

    lastreturn:
    *outendx = bestendx;
    *outa = besta;
    *outb = bestb;
    *outcmin = bestcmin;
    *outcmax = bestcmax;
    return;
}



/*
herons_loop = 1;

startx=0; is_div=false;
    if score x matches 0..19310 : estimate = -594039/(x+4095)+149
startx=19311; is_div=false;
    if score x matches 19311..1713395 : estimate = -1585275779/(x+909823)+1863
startx=1713396; is_div=true;
    if score x matches 1713396..39589079 : estimate = -2147483648/(x/142+224224)+10450
startx=39589080; is_div=true;
    if score x matches 39589080..457275375 : estimate = -2147483648/(x/7817+60312)+39254
startx=455779651; is_div=true;
    if score x matches 455779651..2147483647 : estimate = -2147483648/(x/100000+26111)+[91181~91228]



herons_loop = 0;

startx=0; is_div=false;
    if score x matches 0..288 : estimate = 3660/(x+159)+25
startx=289; is_div=false;
    if score x matches 289..3968 : estimate = 428671/(x+4095)+116
startx=3969; is_div=false;
    if score x matches 3969..23715 : estimate = 9436801/(x+32767)+321
startx=23716; is_div=false;
    if score x matches 22801..84099 : estimate = 82353454/(x+140287)+657
*/