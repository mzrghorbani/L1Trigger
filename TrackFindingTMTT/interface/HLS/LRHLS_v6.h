/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v6_H__
#define __LRHLS_v6_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#else
#include "LRHLS_types.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

template<int STUBS, int LAYERS, int LIMIT>
class LRHLS_v6 {
public:

	LRHLS_v6(TrackHLS<STUBS> &trackIn, TrackHLS<STUBS> &trackOut);
	~LRHLS_v6() {}

	void produce();
	void initFit();
	void calcHelix();
	void calcResidual();
	void killResidual();
	uint1_t exit_t();

public:

	TrackHLS<STUBS> *trackIn_;
	TrackHLS<STUBS> *trackOut_;
	StubHLS stubs_[STUBS];
	uint3_t population_[LAYERS];
	dtf_t residuals_[STUBS];
	LRTrack parameters_;
	uint3_t foundLayers_;
	uint4_t nStubs_;
	LRSums sums_;
};

template<int STUBS, int LAYERS, int LIMIT>
LRHLS_v6<STUBS, LAYERS, LIMIT>::LRHLS_v6(TrackHLS<STUBS> &trackIn, TrackHLS<STUBS> &trackOut) : trackIn_(&trackIn), trackOut_(&trackOut), foundLayers_(0), nStubs_(0) {
	for(int i = 0; i < STUBS; i++) {
		stubs_[i] = trackIn_->stubs[i];
	}
}


template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::produce() {

	initFit();
	calcHelix();

	for(int i = 0; i < STUBS; i++) {

		if(exit_t() && (nStubs_ <= LIMIT))
			break;

		calcResidual();
		killResidual();
	}

	for(int i = 0; i < STUBS; i++) {
		trackOut_->stubs[i] = stubs_[i];
	}
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::initFit() {

	uint1_t foundLayers[LAYERS];

	nStubs_ = uint4_t(STUBS);

	for(int i = 0; i < LAYERS; i++) {
		foundLayers[i] = 0;
		population_[i] = 0;
	}

	for(int i = 0; i < STUBS; i++) {
		population_[stubs_[i].layerId] += 1;
		foundLayers[stubs_[i].layerId] = 1;
	}

	for(int i = 0; i < LAYERS; i++) {
		foundLayers_ += 1;
	}
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::calcHelix() {

	for(int i = 0; i < STUBS; i++) {
		sums_.rSum = dtf_t(sums_.rSum + (dtf_t(stubs_[i].r) >> LAYERS));
		sums_.phiSum = dtf_t(sums_.phiSum + (dtf_t(stubs_[i].phi) >> LAYERS));
		sums_.zSum = dtf_t(sums_.zSum + (dtf_t(stubs_[i].z) >> LAYERS));
	}

	parameters_.sp = slope(foundLayers_, sums_.rSum, sums_.phiSum);
	parameters_.ip = intercept(foundLayers_, sums_.rSum, sums_.phiSum, parameters_.sp);

	parameters_.sz = slope(foundLayers_, sums_.rSum, sums_.zSum);
	parameters_.iz = intercept(foundLayers_, sums_.rSum, sums_.zSum, parameters_.sz);
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::calcResidual() {

	dtf_t phi_resid = 0;
	dtf_t z_resid = 0;

	for(int i = 0; i < STUBS; i++) {
		residuals_[i] = 0;
	}

	for(int i = 0; i < STUBS; i++) {
		if(stubs_[i].valid) {
			phi_resid = residual(stubs_[i].r, stubs_[i].phi, parameters_.sp, parameters_.ip);
			z_resid = residual(stubs_[i].r, stubs_[i].z, parameters_.sz, parameters_.iz);

			residuals_[i] = abs_t(phi_resid + z_resid);
		}
	}
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::killResidual() {

	dtf_t largest = 0;
	uint4_t idx = 0;

	for(int i = 0; i < STUBS; i++) {
		if(residuals_[i] > largest) {
			largest = residuals_[i];
			idx = i;
		}
	}

	nStubs_ -= 1;
    population_[stubs_[idx].layerId] -= 1;

    stubs_[idx].r = 0;
    stubs_[idx].phi = 0;
    stubs_[idx].z = 0;
    stubs_[idx].layerId = 0;
    stubs_[idx].valid = 0;

}

template<int STUBS, int LAYERS, int LIMIT>
uint1_t LRHLS_v6<STUBS, LAYERS, LIMIT>::exit_t() {
	for(int i = 0; i < LAYERS; i++) {

		if(population_[i] > 1) {
			return 0;
		}
	}
	return 1;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
