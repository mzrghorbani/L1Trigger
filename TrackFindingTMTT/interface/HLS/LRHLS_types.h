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

typedef struct {
    int13_t r;
    int14_t phi;
    int14_t z;
    uint3_t layerId;
    uint1_t valid;
} StubHLS;

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

template<typename T>
dtf_t sums(T r[10], uint3_t layerId[10], uint1_t valid[10], int WIN_LEN, int LAYERS) {
	dtf_t out = 0;
	for(int i = 0; i < LAYERS; i++) {
		dtf_t layerMinPos = 8191;
		dtf_t layerMaxPos = -8192;
		dtf_t layerPos = 0;
		for(int j = 0; j < WIN_LEN; j++) {
			if(valid[j]) {
				dtf_t tmp = r[j];
				if(layerId[j] == i) {
					layerMinPos = min_t(layerMinPos, tmp);
					layerMaxPos = max_t(layerMaxPos, tmp);
					layerPos = dtf_t(layerMinPos + layerMaxPos);
					layerPos = dtf_t(layerPos / 2);
				}
			}
		}
		out = dtf_t(out + layerPos);
	}
	return out;
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
	dtf_t out = dtf_t(nxy_xy / nyy_xx);
	return out;
}

template<typename T1, typename T2>
dtf_t intercept(dtf_t n, T1 x, T2 y, dtf_t slope) {
	dtf_t tmp1 = dtf_t(slope * x);
	dtf_t tmp2 = dtf_t(y - tmp1);
	dtf_t out = dtf_t(tmp2 / n);
	return out;
}

template<typename T1, typename T2>
dtf_t residual(const T1 &x, const T2 &y, const dtf_t &slope, const dtf_t &intercept) {
	dtf_t tmp1 = dtf_t(slope * x);
	dtf_t tmp2 = dtf_t(tmp1 + intercept);
	dtf_t out = dtf_t(y - tmp2);
	return out;
}

template<typename T1, typename T2, typename T3>
uint4_t largestResid(T1 x[10], T2 y[10], T3 z[10], uint1_t valid[10], dtf_t sp, dtf_t ip, dtf_t sz, dtf_t iz, int WIN_LEN, int LAYERS) {
	uint4_t out = 0;
	dtf_t largest = 0;
	for(int i = WIN_LEN-1; i >= 0; i--) {
		if(valid[i]) {
			dtf_t r_tmp = x[i];
			dtf_t phi_tmp = y[i];
			dtf_t z_tmp = z[i];
			dtf_t phi_resid = residual(r_tmp, phi_tmp, sp, ip);
			dtf_t z_resid = residual(r_tmp, z_tmp, sz, iz);
			dtf_t phiz_resid = abs_t(phi_resid + z_resid);

			if(phiz_resid > largest) {
				largest = phiz_resid;
				out = i;
			}
		}
	}
	return out;
}

template<typename T>
uint1_t exit_t(T population[7], int LAYERS) {
	for(int i = 0; i < LAYERS; i++)
		if(population[i] > 1)
			return 0;
	return 1;
}

template<typename T>
uint3_t foundLayers(T population[7], int LAYERS) {
	uint3_t out = 0;
	for(int i = 0; i < LAYERS; i++)
	   if(population[i] > 0)
		   out += 1;
	return out;
}

template<typename T1, typename T2, typename T3>
void killLargestResid(T1 population[7], T2 layerId[10], T3 valid[10], uint4_t idx) {
    population[layerId[idx]] -= 1;
    valid[idx] = 0;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
