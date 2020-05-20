/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v7_H__
#define __LRHLS_v7_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#else
#include "LRHLS_types.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

template<int nSTUBS, int nLAYERS, int nLIMIT>
class LRHLS_v7 {
public:

	LRHLS_v7(StubHLS stubsIn[nSTUBS], StubHLS stubsOut[nSTUBS]);
	~LRHLS_v7() {}

	void produce();
	void initFit();
	void calcHelix();
	void calcResidual();
	void killResidual();
	uint1_t exit_t();

public:
	StubHLS *stubsIn_;
	StubHLS *stubsOut_;
	StubHLS stubs_[nSTUBS];
	uint3_t population_[nLAYERS];
	dtf_t residuals_[nSTUBS];
	LRTrack parameters_;
	uint3_t foundLayers_;
	uint4_t nStubs_;
};

template<int nSTUBS, int nLAYERS, int nLIMIT>
LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::LRHLS_v7(StubHLS stubsIn[nSTUBS], StubHLS stubsOut[nSTUBS])
	: stubsIn_(stubsIn), stubsOut_(stubsOut), foundLayers_(0), nStubs_(nSTUBS) {

}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::produce() {

	for(int i=nSTUBS-1; i>=0; i--) {
		stubs_[i] = *(stubsIn_+i);
	}

	initFit();
	calcHelix();
	calcResidual();

	for(int k=0; k<nSTUBS; k++) {

		if(exit_t())
			break;

		killResidual();

	}

	for(int i=nSTUBS-1; i>=0; i--) {
		*(stubsOut_+i) = stubs_[i];
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::initFit() {

	for(int i=nLAYERS-1; i>=0; i--) {
		population_[i] = 0;
	}

	for(int i=nSTUBS-1; i>=0; i--) {
		population_[stubs_[i].layerId] += 1;
		residuals_[i] = 0;
	}

	for(int i=nLAYERS-1; i>=0; i--) {
		if(population_[i] > 0)
			foundLayers_ += 1;
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::calcHelix() {

	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;

	for(int i=nSTUBS-1; i>=0; i--) {
		rSum = dtf_t(rSum + (dtf_t(stubs_[i].r) >> foundLayers_));
		phiSum = dtf_t(phiSum + (dtf_t(stubs_[i].phi) >> foundLayers_));
		zSum = dtf_t(zSum + (dtf_t(stubs_[i].z) >> foundLayers_));
	}

	parameters_.sp = slope(foundLayers_, rSum, phiSum);
	parameters_.ip = intercept(foundLayers_, rSum, phiSum, parameters_.sp);

	parameters_.sz = slope(foundLayers_, rSum, zSum);
	parameters_.iz = intercept(foundLayers_, rSum, zSum, parameters_.sz);
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::calcResidual() {

	for(int i=nSTUBS-1; i>=0; i--) {
		dtf_t phi_resid = residual(stubs_[i].r, stubs_[i].phi, parameters_.sp, parameters_.ip);
		dtf_t z_resid = residual(stubs_[i].r, stubs_[i].z, parameters_.sz, parameters_.iz);

		residuals_[i] = abs_t(phi_resid + z_resid);
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::killResidual() {

	dtf_t largest = 0;
	uint4_t idx = 0;

	for(int i=nSTUBS-1; i>=0; i--) {
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

template<int nSTUBS, int nLAYERS, int nLIMIT>
uint1_t LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::exit_t() {

	for(int i=nLAYERS-1; i>=0; i--) {
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
