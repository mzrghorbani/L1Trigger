/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v5_H__
#define __LRHLS_v5_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

template<int SZ, int LAYERS, int LIMIT>
class LRHLS_v5 {
public:

	LRHLS_v5() {}

	~LRHLS_v5() {}

	void produce(const int13_t* r, const int14_t* phi, const int14_t* z, const uint3_t* layerId, uint1_t* valid);

public:

	unsigned int population_[LAYERS];

};

template<int SZ, int LAYERS, int LIMIT>
void LRHLS_v5<SZ, LAYERS, LIMIT>::produce(const int13_t* r, const int14_t* phi, const int14_t* z, const uint3_t* layerId, uint1_t* valid) {

    uint4_t nStubs = SZ;

	for(int i = 0; i < LAYERS; i++)
		population_[i] = 0;

	for(int i = 0; i < SZ; i++)
		population_[*(layerId+i)] += 1;

	while(true) {

		if((exit_t(population_, LAYERS)) && (nStubs <= LIMIT))
			break;

		uint3_t layers = foundLayers(population_, LAYERS);

		dtf_t rSum = sums(r, layerId, valid, SZ, LAYERS);
		dtf_t phiSum = sums(phi, layerId, valid, SZ, LAYERS);
		dtf_t zSum = sums(z, layerId, valid, SZ, LAYERS);

        dtf_t sp = slope(layers, rSum, phiSum);
        dtf_t ip = intercept(layers, phiSum, sp, rSum);

        dtf_t sz = slope(layers, rSum, zSum);
        dtf_t iz = intercept(layers, zSum, sz, rSum);

        uint4_t idx = largestResid(r, phi, z, valid, sp, ip, sz, iz, SZ, LAYERS);

        killLargestResid(population_, layerId, valid, idx);

        nStubs -= 1;
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
