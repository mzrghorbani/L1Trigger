#ifndef LRHLS_TYPES_H_
#define LRHLS_TYPES_H_

/**
 * This defines all types used in HLS code.
 *
 * Author: Maziar Ghorbani
 *
 */

#ifdef CMSSW_GIT_HASH
#include <iostream>
#include "ap_shift_reg.h"
#include <ap_fixed.h>
#include <cmath>
#else
#include "ap_shift_reg.h"
#include <ap_fixed.h>
#include <cmath>
#endif

#define STUBS 12
#define LAYERS 7
#define LIMIT 4
#define STUBWORD 45

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

enum {
    B14 = 14, B13 = 13, B4 = 4, B3 = 3, B1 = 1
};
enum {
    WB = 24, IB = 18, FB = WB - IB
};

// Native variables used in SW
// typedef int int13_t;
// typedef int int14_t;
// typedef int uint4_t;
// typedef int uint3_t;
// typedef bool uint1_t;
// typedef double dtf_t;

// Fixed-point variables used in HW
typedef ap_uint<B1> uint1_t;
typedef ap_uint<B3> uint3_t;
typedef ap_uint<B4> uint4_t;
typedef ap_int<B13> int13_t;
typedef ap_int<B14> int14_t;
typedef ap_fixed<WB,IB> dtf_t;

struct StubHLS {
    int13_t r = 0;
    int14_t phi = 0;
    int14_t z = 0;
    uint3_t layerId = 0;
    uint1_t valid = 0;
};

typedef StubHLS data_t;

struct LRTrack {
	dtf_t sp;
	dtf_t ip;
	dtf_t sz;
	dtf_t iz;
};

template<typename T>
T abs_t(const T &a) {
    if (a < 0)
        return -a;
    return a;
}

template<typename T>
T min_t(const T &a, const T &b) {
    if (a < b)
        return a;
    return b;
}

template<typename T>
T max_t(const T &a, const T &b) {
    if (a > b)
        return a;
    return b;
}

template<typename T1, typename T2, typename T3>
dtf_t slope(const T1 &n, const T2 &x, const T3 &y) {
	dtf_t nxy_xy = dtf_t(dtf_t(n * dtf_t(x * y)) - dtf_t(x * y));
	dtf_t nyy_xx = dtf_t(dtf_t(n * dtf_t(y * y)) - dtf_t(x * x));
	return dtf_t(nxy_xy / nyy_xx);
}

template<typename T1, typename T2, typename T3, typename T4>
dtf_t intercept(const T1 &n, const T2 &x, const T3 &y, const T4 &slope) {
	return dtf_t(dtf_t(y - dtf_t(slope * x)) / n);
}

template<typename T1, typename T2, typename T3>
dtf_t residual(const T1 &x, const T2 &y, const T3 &slope, const T3 &intercept) {
	return dtf_t(y - dtf_t(intercept + dtf_t(slope * x)));
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
