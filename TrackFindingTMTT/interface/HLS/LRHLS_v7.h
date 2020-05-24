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
private:
public:

	data_t operator()(data_t data);
	void initFit();
	void calcHelix();
	void calcResidual();
	void findLargestResidual();
	void killResidual();
	uint1_t exit_t();

public:

	data_t data_[nSTUBS];
	data_t stubs_[nSTUBS];
	uint3_t population_[nLAYERS];
	dtf_t residuals_[nSTUBS];
	LRTrack parameters_;
	uint3_t foundLayers_;
	uint4_t nStubs_;
	uint4_t idx_;
};

template<int nSTUBS, int nLAYERS, int nLIMIT>
data_t LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::operator()(data_t data) {

    for (int i=nSTUBS-1; i>0; i--) {
    	data_[i] = data_[i-1];
	}
    data_[0] = data;

	if(data.valid == 1) {

		nStubs_ += 1;

		if(nStubs_ == nSTUBS) {

			for (int i=nSTUBS-1; i>=0; i--) {
				stubs_[i] = data_[i];
			}

			initFit();
			calcHelix();
			calcResidual();

			for(int k=0; k<nSTUBS; k++) {

				if(exit_t())
					break;

				findLargestResidual();
				killResidual();
			}

			nStubs_ = 0;

			for (int i=nSTUBS-1; i>=0; i--) {
				data_[i] = stubs_[i];
			}
		}
	}

	return data_[nSTUBS-1];
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::initFit() {

	uint1_t foundLayers[nLAYERS] = {0,0,0,0,0,0,0};
#pragma HLS ARRAY_PARTITION variable=foundLayers complete dim=1

	uint3_t population[nLAYERS] = {0,0,0,0,0,0,0};
#pragma HLS ARRAY_PARTITION variable=population complete dim=1

	for(int i=nSTUBS-1; i>=0; i--) {
		population[stubs_[i].layerId] += 1;
		foundLayers[stubs_[i].layerId] = 1;
	}

	for(int i=nLAYERS-1; i>=0; i--) {
		foundLayers_ += foundLayers[i];
		population_[i] = population[i];
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
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::findLargestResidual() {

	dtf_t largest = 0;

	for(int i=nSTUBS-1; i>=0; i--) {
		if(residuals_[i] > largest) {
			largest = residuals_[i];
			idx_ = i;
		}
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::killResidual() {

	population_[stubs_[idx_].layerId] -= 1;

	stubs_[idx_].r = 0;
	stubs_[idx_].phi = 0;
	stubs_[idx_].z = 0;
	stubs_[idx_].layerId = 0;
	stubs_[idx_].valid = 0;

	residuals_[idx_] = 0;
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
