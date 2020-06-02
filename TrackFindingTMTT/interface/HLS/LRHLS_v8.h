/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v8_H__
#define __LRHLS_v8_H__

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
class LRHLS_v8 {
public:

	LRHLS_v8(data_t stubsIn[nSTUBS], data_t stubsOut[nSTUBS]);
	~LRHLS_v8() {}

	void produce();
	void initFit();
	void calcHelix();
	void calcResidual();
	uint4_t findLargestResidual();
	void killLargestResidual(uint4_t idx);
	uint1_t exit_t();

public:

	data_t *stubsIn_;
	data_t *stubsOut_;

	data_t stubs_[nSTUBS];
	uint3_t population_[nLAYERS];
	uint1_t foundLayers_[nLAYERS];
	dtf_t residuals_[nSTUBS];
	LRTrack parameters_;
	uint3_t nLayers_;
};

template<int nSTUBS, int nLAYERS, int nLIMIT>
LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::LRHLS_v8(data_t stubsIn[nSTUBS], data_t stubsOut[nSTUBS])
	: stubsIn_(stubsIn), stubsOut_(stubsOut), nLayers_(0) {

}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::produce() {

	for(int i=0; i<nSTUBS; i++) {
		stubs_[i] = stubsIn_[i];
	}

	initFit();

	calcHelix();
	calcResidual();

	for(int k=0; k<nSTUBS; k++) {

		if(exit_t())
			break;

		uint4_t idx = findLargestResidual();
		killLargestResidual(idx);

	}

	for(int i=0; i<nSTUBS; i++) {
		stubsOut_[i] = stubs_[i];
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::initFit() {

	for(int i=0; i<nLAYERS; i++) {
		population_[i] = 0;
		foundLayers_[i] = 0;
	}

//	for(int i=0; i<nSTUBS; i++) {
//		population_[stubs_[i].layerId]++;
//		foundLayers_[stubs_[i].layerId] = 1;
//	}


	for(int i=0; i<nSTUBS; i++) {
		switch(stubs_[i].layerId) {
		case 1:
			population_[1]++;
			foundLayers_[1] = 1;
			break;
		case 2:
			population_[2]++;
			foundLayers_[2] = 1;
			break;
		case 3:
			population_[3]++;
			foundLayers_[3] = 1;
			break;
		case 4:
			population_[4]++;
			foundLayers_[4] = 1;
			break;
		case 5:
			population_[5]++;
			foundLayers_[5] = 1;
			break;
		case 6:
			population_[6]++;
			foundLayers_[6] = 1;
			break;
		default:
			population_[0]++;
			foundLayers_[0] = 1;
			break;
		}
	}

	for(int i=0; i<nLAYERS; i++) {
		nLayers_ += foundLayers_[i];
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::calcHelix() {

	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;

	for(int i=0; i<nSTUBS; i++) {
		rSum = dtf_t(rSum + (dtf_t(stubs_[i].r) >> nLayers_));
		phiSum = dtf_t(phiSum + (dtf_t(stubs_[i].phi) >> nLayers_));
		zSum = dtf_t(zSum + (dtf_t(stubs_[i].z) >> nLayers_));
	}

	parameters_.sp = slope(nLayers_, rSum, phiSum);
	parameters_.ip = intercept(nLayers_, rSum, phiSum, parameters_.sp);

	parameters_.sz = slope(nLayers_, rSum, zSum);
	parameters_.iz = intercept(nLayers_, rSum, zSum, parameters_.sz);
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::calcResidual() {

	for(int i=0; i<nSTUBS; i++) {
		dtf_t phi_resid = residual(stubs_[i].r, stubs_[i].phi, parameters_.sp, parameters_.ip);
		dtf_t z_resid = residual(stubs_[i].r, stubs_[i].z, parameters_.sz, parameters_.iz);

		residuals_[i] = abs_t(phi_resid + z_resid);
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
uint4_t LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::findLargestResidual() {

	dtf_t largest = 0;
	uint4_t idx = 0;

	for(int i=0; i<nSTUBS; i++) {
		if(residuals_[i] > largest) {
			largest = residuals_[i];
			idx = i;
		}
	}

	return idx;
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::killLargestResidual(uint4_t idx) {

	population_[stubs_[idx].layerId] -= 1;

	stubs_[idx].r = 0;
	stubs_[idx].phi = 0;
	stubs_[idx].z = 0;
	stubs_[idx].layerId = 0;
	stubs_[idx].valid = 0;

	residuals_[idx] = 0;

}

template<int nSTUBS, int nLAYERS, int nLIMIT>
uint1_t LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::exit_t() {

	for(int i=0; i<nLAYERS; i++) {
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
