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
	void killLargestResidual();
	uint1_t exit_t();

public:

	data_t stubs_[nSTUBS];
	uint3_t population_[nLAYERS];
	dtf_t residuals_[nSTUBS];
	uint1_t foundLayers_[nLAYERS];
	LRTrack parameters_;
	uint3_t nLayers_;
	uint4_t nStubs_;
	uint4_t idx_;
};

template<int nSTUBS, int nLAYERS, int nLIMIT>
data_t LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::operator()(data_t data) {

    for (int i=nSTUBS-1; i>0; i--) {
    	stubs_[i] = stubs_[i-1];
	}
    stubs_[0] = data;

	if(data.valid == 1) {

		initFit();

		nStubs_ += 1;

		if(nStubs_ == nSTUBS) {


			for(int i=nLAYERS-1; i>=0; i--) {
				nLayers_ += foundLayers_[i];
			}


			calcHelix();
			calcResidual();

			for(int k=0; k<nSTUBS; k++) {

				if(exit_t())
					break;

				findLargestResidual();
				killLargestResidual();
			}

			nLayers_ = 0;
			nStubs_ = 0;

		}
	}

	return stubs_[nSTUBS-1];
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::initFit() {

	switch(stubs_[0].layerId) {
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

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::calcHelix() {

	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;

	for(int i=nSTUBS-1; i>=0; i--) {
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
void LRHLS_v7<nSTUBS, nLAYERS, nLIMIT>::killLargestResidual() {

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
