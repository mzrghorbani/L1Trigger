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
#include <ap_fixed.h>
#include <cmath>
#else
#include <ap_fixed.h>
#include <cmath>
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

enum {
    B14 = 14, B13 = 13, B4 = 4, B3 = 3, B1 = 1
};
enum {
    WB = 35, IB = 28, FB = WB - IB
};

// Native variables used in SW
//typedef int int13_t;
//typedef int int14_t;
//typedef int uint4_t;
//typedef int uint3_t;
//typedef bool uint1_t;
//typedef float dtf_t;

// Fixed-point variables used in HW
typedef ap_uint<B1> uint1_t;
typedef ap_uint<B3> uint3_t;
typedef ap_uint<B4> uint4_t;
typedef ap_int<B13> int13_t;
typedef ap_int<B14> int14_t;
typedef ap_fixed<WB,IB> dtf_t;

struct StubHLS{
    int13_t r;
    int14_t phi;
    int14_t z;
    uint3_t layerId;
    uint1_t valid;
};

template<int I>
struct TrackHLS {
	StubHLS stubs[I];
};

struct LRTrack {
	dtf_t sp;
	dtf_t ip;
	dtf_t sz;
	dtf_t iz;
};

struct LRSums {
	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;
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

template<typename T1, typename T2>
dtf_t slope(const dtf_t &n, const T1 &x, const T2 &y) {
	dtf_t xy = dtf_t(x * y);
	dtf_t nxy = dtf_t(n * xy);
	dtf_t yy = dtf_t(y * y);
	dtf_t nyy = dtf_t(n * yy);
	dtf_t xx = dtf_t(x * x);
	dtf_t nxy_xy = dtf_t(nxy - xy);
	dtf_t nyy_xx = dtf_t(nyy - xx);
	return dtf_t(nxy_xy / nyy_xx);
}

template<typename T1, typename T2>
dtf_t residual(const T1 &x, const T2 &y, const dtf_t &slope, const dtf_t &intercept) {
	dtf_t tmp1 = dtf_t(slope * x);
	dtf_t tmp2 = dtf_t(tmp1 + intercept);
	return dtf_t(y - tmp2);
}

template<typename T1, typename T2>
dtf_t intercept(dtf_t n, T1 x, T2 y, dtf_t slope) {
	return dtf_t(dtf_t(y - dtf_t(slope * x)) / n);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
