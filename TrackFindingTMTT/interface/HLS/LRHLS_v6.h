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
};

template<int STUBS, int LAYERS, int LIMIT>
LRHLS_v6<STUBS, LAYERS, LIMIT>::LRHLS_v6(TrackHLS<STUBS> &trackIn, TrackHLS<STUBS> &trackOut) : trackIn_(&trackIn), trackOut_(&trackOut), foundLayers_(0), nStubs_(STUBS) {

}


template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::produce() {

	for(int i = 0; i < STUBS; i++) {
		stubs_[i] = trackIn_->stubs[i];
	}

	initFit();
	calcHelix();
	calcResidual();

	for(int i = 0; i < STUBS; i++) {

		if(exit_t())
			break;

		killResidual();
	}

	for(int i = 0; i < STUBS; i++) {
		trackOut_->stubs[i] = stubs_[i];
	}

}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::initFit() {

	uint1_t foundLayers[LAYERS];

	for(int i = 0; i < LAYERS; i++) {
		population_[i] = 0;
		foundLayers[i] = 0;
	}

	for(int i = 0; i < STUBS; i++) {
		if(stubs_[i].valid) {
			population_[stubs_[i].layerId] += 1;
			foundLayers[stubs_[i].layerId] = 1;
		}
		residuals_[i] = 0;
	}

	for(int i = 0; i < LAYERS; i++) {
		foundLayers_ += foundLayers[i];
	}
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::calcHelix() {

	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;

	for(int i = 0; i < STUBS; i++) {
		rSum = dtf_t(rSum + (dtf_t(stubs_[i].r) >> foundLayers_));
		phiSum = dtf_t(phiSum + (dtf_t(stubs_[i].phi) >> foundLayers_));
		zSum = dtf_t(zSum + (dtf_t(stubs_[i].z) >> foundLayers_));
	}

	parameters_.sp = slope(foundLayers_, rSum, phiSum);
	parameters_.ip = intercept(foundLayers_, rSum, phiSum, parameters_.sp);

	parameters_.sz = slope(foundLayers_, rSum, zSum);
	parameters_.iz = intercept(foundLayers_, rSum, zSum, parameters_.sz);
}

template<int STUBS, int LAYERS, int LIMIT>
void LRHLS_v6<STUBS, LAYERS, LIMIT>::calcResidual() {

	for(int i = 0; i < STUBS; i++) {
		if(stubs_[i].valid) {
			
			dtf_t phi_resid = residual(stubs_[i].r, stubs_[i].phi, parameters_.sp, parameters_.ip);
			dtf_t z_resid = residual(stubs_[i].r, stubs_[i].z, parameters_.sz, parameters_.iz);

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
	residuals_[idx] = 0;

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
