/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v5_H__
#define __LRHLS_v5_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#else
#include "LRHLS_types.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

template<int WIN_LEN, int LAYERS, int LIMIT>
class LRHLS_v5 {
public:

	LRHLS_v5() {}

	~LRHLS_v5() {}

	void produce(StubHLS* stubIn, StubHLS* stubOut);

public:


};

template<int WIN_LEN, int LAYERS, int LIMIT>
void LRHLS_v5<WIN_LEN, LAYERS, LIMIT>::produce(StubHLS* stubIn, StubHLS* stubOut) {

    static int13_t r[WIN_LEN];
    static int14_t phi[WIN_LEN];
    static int14_t z[WIN_LEN];
    static uint3_t layerId[WIN_LEN];
    static uint1_t valid[WIN_LEN];

	for(int i = WIN_LEN-1; i > 0; i--) {
		r[i] = r[i-1];
		phi[i] = phi[i-1];
		z[i] = z[i-1];
		layerId[i] = layerId[i-1];
		valid[i] = valid[i-1];
	}
	r[0] = stubIn->r;
	phi[0] = stubIn->phi;
	z[0] = stubIn->z;
	layerId[0] = stubIn->layerId;
	valid[0] = stubIn->valid;

	static int population_[LAYERS];
#pragma HLS ARRAY_PARTITION variable=population_ complete dim=1
	static int nStubs = 0;

	if(valid[0]) {
		population_[layerId[0]] += 1;
		nStubs +=1;
	}

	if(nStubs == WIN_LEN) {

		while(!exit_t(population_, LAYERS)) {

			uint3_t layers = foundLayers(population_, LAYERS);

			dtf_t rSum = sums(r, layerId, valid, WIN_LEN, LAYERS);
			dtf_t phiSum = sums(phi, layerId, valid, WIN_LEN, LAYERS);
			dtf_t zSum = sums(z, layerId, valid, WIN_LEN, LAYERS);

			dtf_t sp = slope(layers, rSum, phiSum);
			dtf_t ip = intercept(layers, rSum, phiSum, sp);

			dtf_t sz = slope(layers, rSum, zSum);
			dtf_t iz = intercept(layers, rSum, zSum, sz);

			uint4_t idx = largestResid(r, phi, z, valid, sp, ip, sz, iz, WIN_LEN, LAYERS);

			killLargestResid(population_, layerId, valid, idx);
		}
		nStubs = 0;
	}

	stubOut->r = r[WIN_LEN-1];
	stubOut->phi = phi[WIN_LEN-1];
	stubOut->z = z[WIN_LEN-1];
	stubOut->layerId = layerId[WIN_LEN-1];
	stubOut->valid = valid[WIN_LEN-1];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
