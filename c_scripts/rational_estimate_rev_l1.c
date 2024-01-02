#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "isqrt.c"
#include "libdivide.h"


int32_t herons_lowerbound_l1(int32_t x, int32_t y) {
    int32_t y2 = y<<1;
    int32_t left=1, right=y;
    if (left >= right) return left;
    int32_t mid = (left+right)>>1;
    while (1) {
        int32_t check = mid+x/mid-y2;
        if (0 <= check && check <= 3) {
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

int32_t herons_upperbound_l1(int32_t x, int32_t y) {
    int32_t y2 = y<<1;
    int32_t left=y+1, right=46773;
    int32_t mid = (left+right)>>1;
    while (1) {
        int32_t check = mid+x/mid-y2;
        if (0 <= check && check <= 3) {
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

bool rational_touch_ceil_rev_l1(bool is_div, int64_t ina, int64_t inb, int64_t startx, int64_t *outendx, int64_t *outcmin, int64_t *outcmax) {
    int32_t a = (int32_t)ina;
    int32_t b = (int32_t)inb;
    int32_t x = (int32_t)startx;
    int32_t ma = -a;
    int32_t sqrtx = (int32_t)sqrt(x);
    int32_t sqrtx2 = sqrtx<<1;
    int32_t sqrtnext = x - sqrtx*sqrtx;
    int32_t estimate,check;
    struct libdivide_s32_t fast_a;
    if (is_div) {
        fast_a = libdivide_s32_gen(a);
        estimate = libdivide_s32_do(x,&fast_a) + b;
        estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
    } else {
        estimate = x+b;
        estimate = ma/estimate - (ma%estimate!=0);
    }
    int32_t upper = herons_upperbound_l1(x,sqrtx)-estimate;
    int32_t lower = herons_lowerbound_l1(x,sqrtx)-estimate;
    int32_t t;
    for (x=x-1; x>=0; x--) {
        sqrtnext--;
        if (sqrtnext == -1) {
            sqrtx--;
            sqrtx2 = sqrtx<<1;
            sqrtnext = sqrtx2;
        }
        if (is_div) {
            estimate = libdivide_s32_do(x,&fast_a) + b;
            estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
        } else {
            estimate = x+b;
            estimate = ma/estimate - (ma%estimate!=0);
        }
        check = estimate + lower;
        check = check + x/check - sqrtx2;
        if (0 > check || check > 3) {
            t = herons_lowerbound_l1(x,sqrtx)-estimate;
            if (upper < t || t < lower) break;
            lower = t;
        }
        check = estimate + upper;
        check = check + x/check - sqrtx2;
        if (0 > check || check > 3) {
            upper = herons_upperbound_l1(x,sqrtx)-estimate;
        }
    }
    *outendx = (int64_t)(x+1);
    *outcmin = (int64_t)lower;
    *outcmax = (int64_t)upper;
    if (is_div) {
        estimate = libdivide_s32_do(x,&fast_a) + b;
        estimate = (-2147483468)/estimate - ((-2147483468)%estimate!=0);
    } else {
        estimate = x+b;
        estimate = ma/estimate - (ma%estimate!=0);
    }
    estimate += upper;
    upper = herons_upperbound_l1(x,(int32_t)sqrt(x));
    return (estimate > upper);
}



void find_best_rational_estimate_with_a_rev_l1(bool is_div, int64_t a, int64_t startx, int64_t *outendx, int64_t *outb, int64_t *outcmin, int64_t *outcmax) {
    int64_t minb = 0;
    int64_t maxb = 2147483647;
    int64_t endx,cmin,cmax;
    int64_t bestendx=2147483647,bestb=-1,bestcmin=-1,bestcmax=-1;
    // binary search
    while (1) {
        int64_t b = (minb+maxb)>>1;
        bool check = rational_touch_ceil_rev_l1(is_div,a,b,startx,&endx,&cmin,&cmax);
        if (endx < bestendx) {
            bestendx=endx; bestb=b; bestcmin=cmin; bestcmax=cmax;
            if (endx <= 0) break;
        }
        if (check) {
            maxb = b;
            if (minb == maxb) break;
        } else {
            minb = b+1;
            if (minb == maxb) {
                rational_touch_ceil_rev_l1(is_div,a,minb,startx,&endx,&cmin,&cmax);
                if (endx < bestendx) {
                    bestendx=endx; bestb=minb; bestcmin=cmin; bestcmax=cmax;
                }
                break;
            }
        }
    }
    *outendx = bestendx;
    *outb = bestb;
    *outcmin = bestcmin;
    *outcmax = bestcmax;
    return;
}


#define TEST_RANGE 16
#define min(x,y) ((x)>(y) ? (y) : (x))
#define max(x,y) ((x)<(y) ? (y) : (x))

void find_best_rational_estimate_rev_l1(int64_t startx, bool is_div, int64_t *outendx, int64_t *outa, int64_t *outb, int64_t *outcmin, int64_t *outcmax) {
    int64_t a, a_left, a_right;
    int64_t endx,b,cmin,cmax;
    int64_t bestendx=2147483647,besta=-1,bestb=-1,bestcmin=-1,bestcmax=-1;
    if (is_div) {
        a_left = 1;
        a_right = 16777215;
    } else {
        a_left = 1;
        a_right = 2147483647;
    }


    // int64_t a_best_left=-1,a_best_right=-1,a_best_endx=2147483647;
    // printf("initial search a=[%lld~%lld]\n",a_left,a_right);
    // for (a=TEST_RANGE; a<=a_right; a=(int64_t)(a*1.5)+TEST_RANGE) {
    //     printf("a=%lld\n",a);
    //     int64_t a_endx = 2147483647;
    //     for (int64_t i=0; i<TEST_RANGE; i++) {
    //         find_best_rational_estimate_with_a_rev_l1(is_div,a-i,startx,&endx,&b,&cmin,&cmax);
    //         if (endx < a_endx)
    //             a_endx = endx;
    //         if (endx < bestendx) {
    //             printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a-i,b,cmin,cmax,endx);
    //             bestendx=endx; besta=a-i; bestb=b; bestcmin=cmin; bestcmax=cmax;
    //             if (bestendx <= 0) goto lastreturn;
    //         }
    //     }
    //     if (a_endx < a_best_endx) {
    //         a_best_endx = a_endx;
    //         a_best_left = ((a-TEST_RANGE)/1.5-TEST_RANGE)/1.5;
    //         a_best_right = ((a*1.5)+TEST_RANGE)*1.5+TEST_RANGE;
    //     }
    // }
    // a_best_left = ((114420-TEST_RANGE)/1.5-TEST_RANGE)/1.5;
    // a_best_right = ((114420*1.5)+TEST_RANGE)*1.5+TEST_RANGE;
    // a_left = max(0,a_best_left);
    // a_right = min(2147483647,a_best_right);

    a_left = 95219;
    a_right = 96257;


    int64_t golden_offset = (int64_t)floor((a_right - a_left)*0.381966011250105);
    int64_t a_left_golden = a_left + golden_offset;
    int64_t a_right_golden = a_right - golden_offset;
    
    int64_t a_left_golden_left = max(a_left, a_left_golden - (TEST_RANGE>>1));
    int64_t a_left_golden_right = a_left_golden_left + TEST_RANGE - 1;
    int64_t a_right_golden_right = min(a_right, a_right_golden + (TEST_RANGE>>1) - 1);
    int64_t a_right_golden_left = a_right_golden_right - TEST_RANGE;

    if (a_left_golden_right+1 >= a_right_golden_left) {
        printf("final search a=[%lld~%lld]\n",a_left,a_right);
        for (a=a_left; a<=a_right; a++) {
            find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (endx < bestendx) {
                printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx <= 0) goto lastreturn;
            }
        }
    } else {
        printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);

        int64_t a_left_golden_endx = 2147483647;
        for (a=a_left_golden_left; a<=a_left_golden_right; a++) {
            find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (endx < a_left_golden_endx)
                a_left_golden_endx = endx;
            if (endx < bestendx) {
                printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx <= 0) goto lastreturn;
            }
        }

        int64_t a_right_golden_endx = 2147483647;
        for (a=a_right_golden_left; a<=a_right_golden_right; a++) {
            find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
            if (endx < a_right_golden_endx)
                a_right_golden_endx = endx;
            if (endx < bestendx) {
                printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                if (bestendx <= 0) goto lastreturn;
            }
        }

        while (1) {
            printf("%lld vs %lld\n",a_left_golden_endx,a_right_golden_endx);
            if (a_left_golden_endx > a_right_golden_endx) {
                a_left = a_left_golden;
                a_left_golden = a_right_golden;
                a_left_golden_left = a_right_golden_left;
                a_left_golden_right = a_right_golden_right;
                a_left_golden_endx = a_right_golden_endx;
                golden_offset = (int64_t)floor((a_right - a_left)*0.381966011250105);
                a_right_golden = a_right - golden_offset;
                a_right_golden_right = min(a_right, a_right_golden + (TEST_RANGE>>1) - 1);
                a_right_golden_left = a_right_golden_right - TEST_RANGE;
                if (a_left_golden_right+1 >= a_right_golden_left) {
                    printf("final search a=[%lld~%lld]\n",a_left,a_right);
                    for (a=a_left; a<a_left_golden_left; a++) {
                        find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (endx < bestendx) {
                            printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx <= 0) goto lastreturn;
                        }
                    }
                    for (a=a_left_golden_right+1; a<=a_right; a++) {
                        find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (endx < bestendx) {
                            printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx <= 0) goto lastreturn;
                        }
                    }
                    goto lastreturn;
                }
                printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);
                a_right_golden_endx = 2147483647;
                for (a=a_right_golden_left; a<=a_right_golden_right; a++) {
                    find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                    if (endx < a_right_golden_endx)
                        a_right_golden_endx = endx;
                    if (endx < bestendx) {
                        printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                        bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                        if (bestendx <= 0) goto lastreturn;
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
                a_left_golden_left = max(a_left, a_left_golden - (TEST_RANGE>>1));
                a_left_golden_right = a_left_golden_left + TEST_RANGE - 1;
                if (a_left_golden_right+1 >= a_right_golden_left) {
                    printf("final search a=[%lld~%lld]\n",a_left,a_right);
                    for (a=a_left; a<a_right_golden_left; a++) {
                        find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (endx < bestendx) {
                            printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx <= 0) goto lastreturn;
                        }
                    }
                    for (a=a_right_golden_right+1; a<=a_right; a++) {
                        find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                        if (endx < bestendx) {
                            printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                            bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                            if (bestendx <= 0) goto lastreturn;
                        }
                    }
                    goto lastreturn;
                }
                printf("golden section search a=[%lld~%lld~%lld~%lld]\n",a_left,a_left_golden,a_right_golden,a_right);
                a_left_golden_endx = 2147483647;
                for (a=a_left_golden_left; a<=a_left_golden_right; a++) {
                    find_best_rational_estimate_with_a_rev_l1(is_div,a,startx,&endx,&b,&cmin,&cmax);
                    if (endx < a_left_golden_endx)
                        a_left_golden_endx = endx;
                    if (endx < bestendx) {
                        printf("a=%lld;b=%lld;c=[%lld~%lld];endx=%lld;\n",a,b,cmin,cmax,endx);
                        bestendx=endx; besta=a; bestb=b; bestcmin=cmin; bestcmax=cmax;
                        if (bestendx <= 0) goto lastreturn;
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


int main(void) {
	int64_t startx,endx,a,b,cmin,cmax;
    bool is_div;
    
    startx=2147483647; is_div=true;

    find_best_rational_estimate_rev_l1(startx,is_div,&endx,&a,&b,&cmin,&cmax);
    if (is_div)
        printf("if score x matches %lld..%lld : estimate = -2147483648/(x/%lld+%lld)+",endx,startx,a,b);
    else
        printf("if score x matches %lld..%lld : estimate = -%lld/(x+%lld)+",endx,startx,a,b);
    if (cmin == cmax)
        printf("%lld\n",cmin);
    else
        printf("[%lld~%lld]\n",cmin,cmax);
    printf("end");
	return 0;
}


/*

startx=0; is_div=false;
    if score x matches 0..19310 : estimate = -594039/(x+4095)+149
startx=19311; is_div=false;
    if score x matches 19311..1705544 : estimate = -1568669636/(x+903167)+1857
startx=1705545; is_div=true;
    if score x matches 1705545..39400514 : estimate = -2147483648/(x/141+224767)+10425
startx=39400515; is_div=true;
    if score x matches 39400515..455779650 : estimate = -2147483648/(x/7775+60419)+39184
startx=455779651; is_div=true;
    if score x matches 455779651..2147483647 : estimate = -2147483648/(x/100000+26111)+[91181~91228]

a=10016
a=10016;b=31;c=[56052~56052];endx=2046929049;
a=15040
a=15040;b=7;c=[61077~61077];endx=1969673161;
a=22576
a=22576;b=12313;c=[66027~66027];endx=1402802116;
a=22574;b=12310;c=[66026~66026];endx=1402727209;
a=22573;b=12311;c=[66025~66025];endx=1402652304;
a=22572;b=12311;c=[66024~66024];endx=1402577401;
a=33880
a=33880;b=22678;c=[70991~70991];endx=1291971136;
a=33879;b=22682;c=[70989~70989];endx=1291539844;
a=33878;b=22683;c=[70988~70988];endx=1291467969;
a=50836
a=50836;b=26760;c=[77160~77160];endx=1142575204;
a=50835;b=26761;c=[77159~77159];endx=1142507601;
a=50830;b=26761;c=[77157~77157];endx=1142440000;
a=76270
a=76270;b=27181;c=[84846~84846];endx=900660121;
a=76266;b=27183;c=[84843~84843];endx=900600100;
a=76262;b=27183;c=[84842~84842];endx=900540081;
a=114421
a=114421;b=24924;c=[95471~95471];endx=300363561;
a=114420;b=24923;c=[95473~95473];endx=300155625;
a=171647
a=257486
a=386245
golden section search a=[50835~129768~178552~257485]
a=129760;b=23910;c=[99539~99539];endx=329858244;
329858244 vs 418202500
golden section search a=[50835~99618~129769~178552]
a=99610;b=26090;c=[91183~91183];endx=270767025;
270767025 vs 329858244
golden section search a=[50835~80985~99618~129769]
844309249 vs 270767025
golden section search a=[80985~99618~111136~129769]
270767025 vs 293642496
golden section search a=[80985~92501~99618~111136]
643028164 vs 270767025
golden section search a=[92501~99618~104019~111136]
270767025 vs 279825984
golden section search a=[92501~96900~99618~104019]
a=96892;b=26329;c=[90351~90351];endx=265396681;
a=96896;b=26328;c=[90354~90354];endx=265233796;
a=96898;b=26327;c=[90356~90356];endx=265136089;
265136089 vs 270767025
golden section search a=[92501~95219~96900~99618]
506295001 vs 265136089
golden section search a=[95219~96900~97938~99618]
265136089 vs 267387904
golden section search a=[95219~96257~96900~97938]
a=96249;b=26386;c=[90154~90154];endx=263932516;
a=96258;b=26385;c=[90157~90157];endx=263867536;
263867536 vs 265136089
golden section search a=[95219~95861~96257~96900]
a=95853;b=26423;c=[90029~90029];endx=263315529;
a=95855;b=26422;c=[90031~90031];endx=263218176;
a=95856;b=26422;c=[90031~90031];endx=263055961;
263055961 vs 263867536
golden section search a=[95219~95615~95861~96257]
a=95607;b=26445;c=[89953~89953];endx=262731681;
a=95609;b=26444;c=[89955~89955];endx=262472401;
262472401 vs 263055961
golden section search a=[95219~95464~95615~95861]
a=95456;b=26459;c=[89905~89905];endx=262342809;
262342809 vs 262472401
golden section search a=[95219~95370~95464~95615]
a=95363;b=26467;c=[89877~89877];endx=262180864;
262180864 vs 262342809
golden section search a=[95219~95312~95370~95464]
485937936 vs 262180864
golden section search a=[95312~95370~95406~95464]
262180864 vs 262278025
golden section search a=[95312~95347~95370~95406]
262213249 vs 262180864
final search a=[95347~95406]
a=95355;b=26467;c=[89876~89876];endx=262148481;
if score x matches 262148481..2147483647 : estimate = -2147483648/(x/95355+26467)+89876
*/