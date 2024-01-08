#include "const_estimate.c"
#include "const_estimate_rev.c"
#include "linear_estimate.c"
#include "linear_estimate_rev.c"
#include "rational_estimate.c"
#include "rational_estimate_rev.c"
#include <string.h>


int main(int argc, char *argv[]) {
    int64_t startx, endx, a,b,cmin,cmax;
    int64_t endx2, a2,b2,cmin2,cmax2;
    bool is_div;
    if (argc < 3) return 1;
    startx = (int64_t)atoi(argv[2]);

    if (strcmp(argv[1],"const_estimate") == 0) {
        find_best_const_estimate(startx,&endx,&cmin,&cmax);
        printf("{\"HERONS_LOOP\":%d,\"type\":\"const_estimate\",\"startx\":%lld,\"endx\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,endx,cmin,cmax);
    } else if (strcmp(argv[1],"const_estimate_rev") == 0) {
        find_best_const_estimate_rev(startx,&endx,&cmin,&cmax);
        printf("{\"HERONS_LOOP\":%d,\"type\":\"const_estimate_rev\",\"startx\":%lld,\"endx\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,endx,cmin,cmax);
    } else if (strcmp(argv[1],"linear_estimate") == 0) {
        find_best_linear_estimate(startx,&endx,&a,&cmin,&cmax);
        printf("{\"HERONS_LOOP\":%d,\"type\":\"linear_estimate\",\"startx\":%lld,\"endx\":%lld,\"div\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,endx,a,cmin,cmax);
    } else if (strcmp(argv[1],"linear_estimate_rev") == 0) {
        find_best_linear_estimate_rev(startx,&endx,&a,&cmin,&cmax);
        printf("{\"HERONS_LOOP\":%d,\"type\":\"linear_estimate_rev\",\"startx\":%lld,\"endx\":%lld,\"div\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,endx,a,cmin,cmax);
    } else if (strcmp(argv[1],"rational_estimate") == 0) {
        #if HERONS_LOOP == 1
            is_div = false;
            if (startx < 50) {
                find_best_rational_estimate(16384,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 500) {
                find_best_rational_estimate(1024,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 25000) {
                find_best_rational_estimate(16,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 50000) {
                find_best_rational_estimate(16,startx,false,&endx,&a,&b,&cmin,&cmax);
                find_best_rational_estimate(16,startx,true,&endx2,&a2,&b2,&cmin2,&cmax2);
                if (endx < endx2) {
                    a = a2; b = b2; cmin = cmin2; cmax = cmax2;
                    is_div = true;
                }
            } else {
                find_best_rational_estimate(16,startx,true,&endx,&a,&b,&cmin,&cmax);
                is_div = true;
            }
            printf("{\"HERONS_LOOP\":%d,\"type\":\"rational_estimate\",\"startx\":%lld,\"is_div\":%s,\"endx\":%lld,\"a\":%lld,\"b\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,is_div?"true":"false",endx,a,b,cmin,cmax);
        #elif HERONS_LOOP == 0
            is_div = false;
            if (startx < 500) {
                find_best_rational_estimate(16384,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 5000) {
                find_best_rational_estimate(1024,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 250000) {
                find_best_rational_estimate(16,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 450000) {
                find_best_rational_estimate(16,startx,false,&endx,&a,&b,&cmin,&cmax);
                find_best_rational_estimate(16,startx,true,&endx2,&a2,&b2,&cmin2,&cmax2);
                if (endx < endx2) {
                    a = a2; b = b2; cmin = cmin2; cmax = cmax2;
                    is_div = true;
                }
            } else {
                find_best_rational_estimate(16,startx,true,&endx,&a,&b,&cmin,&cmax);
                is_div = true;
            }
            printf("{\"HERONS_LOOP\":%d,\"type\":\"rational_estimate\",\"startx\":%lld,\"is_div\":%s,\"endx\":%lld,\"a\":%lld,\"b\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,is_div?"true":"false",endx,a,b,cmin,cmax);
        #endif
    } else if (strcmp(argv[1],"rational_estimate_rev") == 0) {
        #if HERONS_LOOP == 1
            is_div = false;
            if (startx < 79512) {
                find_best_rational_estimate_rev(16384,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 201599) {
                find_best_rational_estimate_rev(1024,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 2010644) {
                find_best_rational_estimate_rev(16,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 3079935) {
                find_best_rational_estimate_rev(16,startx,false,&endx,&a,&b,&cmin,&cmax);
                find_best_rational_estimate_rev(16,startx,true,&endx2,&a2,&b2,&cmin2,&cmax2);
                if (endx < endx2) {
                    a = a2; b = b2; cmin = cmin2; cmax = cmax2;
                    is_div = true;
                }
            } else {
                find_best_rational_estimate_rev(16,startx,true,&endx,&a,&b,&cmin,&cmax);
                is_div = true;
            }
            printf("{\"HERONS_LOOP\":%d,\"type\":\"rational_estimate_rev\",\"startx\":%lld,\"is_div\":%s,\"endx\":%lld,\"a\":%lld,\"b\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,is_div?"true":"false",endx,a,b,cmin,cmax);
        #elif HERONS_LOOP == 0
            is_div = false;
            if (startx < 5475) {
                find_best_rational_estimate_rev(16384,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 23408) {
                find_best_rational_estimate_rev(1024,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 582168) {
                find_best_rational_estimate_rev(16,startx,false,&endx,&a,&b,&cmin,&cmax);
            } else if (startx < 737873) {
                find_best_rational_estimate_rev(16,startx,false,&endx,&a,&b,&cmin,&cmax);
                find_best_rational_estimate_rev(16,startx,true,&endx2,&a2,&b2,&cmin2,&cmax2);
                if (endx < endx2) {
                    a = a2; b = b2; cmin = cmin2; cmax = cmax2;
                    is_div = true;
                }
            } else {
                find_best_rational_estimate_rev(16,startx,true,&endx,&a,&b,&cmin,&cmax);
                is_div = true;
            }
            printf("{\"HERONS_LOOP\":%d,\"type\":\"rational_estimate_rev\",\"startx\":%lld,\"is_div\":%s,\"endx\":%lld,\"a\":%lld,\"b\":%lld,\"cmin\":%lld,\"cmax\":%lld}",HERONS_LOOP,startx,is_div?"true":"false",endx,a,b,cmin,cmax);
        #endif
    } else {
        return 1;
    }
    return 0;
}