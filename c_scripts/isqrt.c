#include <math.h>
#include <stdint.h>
#define isqrt(x) ((int64_t)sqrt(x))
#define floordiv(a,b) (((b)==0) ? (a) : ((((a)^(b))>0) ? ((a)/(b)) : ((a)/(b) - ((a)%(b)!=0))))

int64_t herons_loop = 3;

int64_t herons_method(int64_t x, int64_t estimate) {
    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Heron's_method
    switch (herons_loop) {
        default:
            for (int64_t i=herons_loop; i>4; i--)
                estimate = (estimate+floordiv(x,estimate))>>1;
        case 4: estimate = (estimate+floordiv(x,estimate))>>1;
        case 3: estimate = (estimate+floordiv(x,estimate))>>1;
        case 2: estimate = (estimate+floordiv(x,estimate))>>1;
        case 1: estimate = (estimate+floordiv(x,estimate))>>1;
        case 0:
    }
    return (floordiv(x,estimate) < estimate) ? (estimate-1) : (estimate);
}

int64_t herons_lowerbound(int64_t x, int64_t y) {
    int64_t left=1, right=y;
    if (left >= right) return left;
    int64_t mid = (left+right)>>1;
    while (1) {
        if (herons_method(x,mid) == y) {
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

int64_t herons_upperbound(int64_t x, int64_t y) {
    int64_t left=y+1, right=100000;
    int64_t mid = (left+right)>>1;
    while (1) {
        if (herons_method(x,mid) == y) {
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
