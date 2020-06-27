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

	LRHLS_v8(hls::stream<word_t> &stubIn, hls::stream<word_t> &stubOut);
	~LRHLS_v8() {}

	void produce();
	void initFit();
	void calcSums();
	void calcHelix();
	void calcResidual();
	void findLargestResidual();
	void killLargestResidual();
	uint1_t exit_t();

public:

	hls::stream<word_t> *stubIn_;
	hls::stream<word_t> *stubOut_;

	data_t stub_[nSTUBS];
	uint3_t population_[nLAYERS];
	uint1_t foundLayers_[nLAYERS];
	dtf_t residuals_[nSTUBS];
	LRParams parameters_;
	LRSums sums_;
	uint3_t nLayers_;
	uint4_t idx_;
};

template<int nSTUBS, int nLAYERS, int nLIMIT>
LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::LRHLS_v8(hls::stream<word_t> &stubIn, hls::stream<word_t> &stubOut)
	: stubIn_(&stubIn), stubOut_(&stubOut), nLayers_(0), idx_(0)
	  {



}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::produce() {

	int i = 0, j;

LOOP_0:
	while(!stubIn_->empty()) {
//#pragma HLS LOOP_TRIPCOUNT

		word_t tmp = stubIn_->read();
		word_t temp;

		stub_[i].r = tmp.range(12, 0);
		stub_[i].phi = tmp.range(26, 13);
		stub_[i].z = tmp.range(40, 27);
		stub_[i].layerId = tmp.range(43, 41);
		stub_[i].valid = tmp.range(44, 44);
		stub_[i].padding = tmp.range(63, 45);

		initFit();
		calcSums();
		calcHelix();
		calcResidual();

		if(i == (nSTUBS-1)) {

			for(j=0; j<nSTUBS; j++) {

				if(exit_t())
					break;

				findLargestResidual();
				killLargestResidual();

				temp.range(12, 0) = stub_[j].r;
				temp.range(26, 13) = stub_[j].phi;
				temp.range(40, 27) = stub_[j].z;
				temp.range(43, 41) = stub_[j].layerId;
				temp.range(44, 44) = stub_[j].valid;
				temp.range(63, 45) = stub_[j].padding;

				stubOut_->write(temp);
			}
		}
		i++;
	}

//	int i, j;
//	for(i=0; i<nSTUBS; i++) {
//		stubOut_[i] = stubIn_[i];
//	}

//	for(i=0; i<nSTUBS; i++) {
//		stub_[i] = stubIn_[i];
//	}
//
////	initFit();
//	for(i=0; i<nLAYERS; i++) {
//		population_[i] = 0;
//		foundLayers_[i] = 0;
//	}
//
//
//	for(i=0; i<nSTUBS; i++) {
//		switch(stub_[i].layerId) {
//		case 1:
//			population_[1]++;
//			foundLayers_[1] = 1;
//			break;
//		case 2:
//			population_[2]++;
//			foundLayers_[2] = 1;
//			break;
//		case 3:
//			population_[3]++;
//			foundLayers_[3] = 1;
//			break;
//		case 4:
//			population_[4]++;
//			foundLayers_[4] = 1;
//			break;
//		case 5:
//			population_[5]++;
//			foundLayers_[5] = 1;
//			break;
//		case 6:
//			population_[6]++;
//			foundLayers_[6] = 1;
//			break;
//		default:
//			population_[0]++;
//			foundLayers_[0] = 1;
//			break;
//		}
//	}
//
//	for(i=0; i<nLAYERS; i++) {
//		nLayers_ += foundLayers_[i];
//	}
//
//
//
////	calcSums();
//	for(i=0; i<nSTUBS; i++) {
//		sums_.r = dtf_t(sums_.r + (dtf_t(stub_[i].r) >> FB));
//		sums_.phi = dtf_t(sums_.phi + (dtf_t(stub_[i].phi) >> FB));
//		sums_.z = dtf_t(sums_.z + (dtf_t(stub_[i].z) >> FB));
//	}
//
//
//
//
////	calcHelix();
//	parameters_.sp = slope(nLayers_, sums_.r, sums_.phi);
//	parameters_.ip = intercept(nLayers_, sums_.r, sums_.phi, parameters_.sp);
//
//	parameters_.sz = slope(nLayers_, sums_.r, sums_.z);
//	parameters_.iz = intercept(nLayers_, sums_.r, sums_.z, parameters_.sz);
//
//
//
////	calcResidual();
//	for(i=0; i<nSTUBS; i++) {
//		dtf_t phi_resid = residual(stub_[i].r, stub_[i].phi, parameters_.sp, parameters_.ip);
//		dtf_t z_resid = residual(stub_[i].r, stub_[i].z, parameters_.sz, parameters_.iz);
//
//		residuals_[i] = abs_t(phi_resid + z_resid);
//	}
//
////	residuals_[0] = 36.8281;
////	residuals_[1] = 49.3281;
////	residuals_[2] = 17.5469;
////	residuals_[3] = 56.2969;
////	residuals_[4] = 43.0781;
////	residuals_[5] = 4.42188;
////	residuals_[6] = 50.6719;
////	residuals_[7] = 80.3594;
//
//
//	for(j=0; j<nSTUBS; j++) {
////	while(true) {
////#pragma HLS LOOP_TRIPCOUNT
//
//		bool single = true;
//
////		if(exit_t())
////			break;
//		for(i=0; i<nLAYERS; i++) {
//			if(population_[i] > 1) {
//				single = false;
//			}
//		}
//
//		if(single == true)
//			break;
//
//
//
////		findLargestResidual();
//		dtf_t largest = 0;
//
//
//		for(i=0; i<nSTUBS; i++) {
//			if(residuals_[i] > largest) {
//				largest = residuals_[i];
//				idx_ = i;
//			}
//		}
//
//
//
////		killLargestResidual();
//		population_[stub_[idx_].layerId] -= 1;
//
//		stub_[idx_].r = 0;
//		stub_[idx_].phi = 0;
//		stub_[idx_].z = 0;
//		stub_[idx_].layerId = 0;
//		stub_[idx_].valid = 0;
//
//		residuals_[idx_] = 0;
//
//	}
//
//	for(i=0; i<nSTUBS; i++) {
//		stubOut_[i] = stub_[i];
//	}

}


template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::initFit() {

	int i;

	for(i=0; i<nLAYERS; i++) {
		population_[i] = 0;
		foundLayers_[i] = 0;
	}


	for(i=0; i<nSTUBS; i++) {
		switch(stub_[i].layerId) {
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

	for(i=0; i<nLAYERS; i++) {
		nLayers_ += foundLayers_[i];
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::calcSums() {

	int i;

	for(i=0; i<nSTUBS; i++) {
		sums_.r = dtf_t(sums_.r + (dtf_t(stub_[i].r) >> FB));
		sums_.phi = dtf_t(sums_.phi + (dtf_t(stub_[i].phi) >> FB));
		sums_.z = dtf_t(sums_.z + (dtf_t(stub_[i].z) >> FB));
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::calcHelix() {

	parameters_.sp = slope(nLayers_, sums_.r, sums_.phi);
	parameters_.ip = intercept(nLayers_, sums_.r, sums_.phi, parameters_.sp);

	parameters_.sz = slope(nLayers_, sums_.r, sums_.z);
	parameters_.iz = intercept(nLayers_, sums_.r, sums_.z, parameters_.sz);

}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::calcResidual() {

	int i;

	for(i=0; i<nSTUBS; i++) {
		dtf_t phi_resid = residual(stub_[i].r, stub_[i].phi, parameters_.sp, parameters_.ip);
		dtf_t z_resid = residual(stub_[i].r, stub_[i].z, parameters_.sz, parameters_.iz);

		residuals_[i] = abs_t(phi_resid + z_resid);
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::findLargestResidual() {

	int i;
	dtf_t largest = 0;

	for(i=0; i<nSTUBS; i++) {
		if(residuals_[i] > largest) {
			largest = residuals_[i];
			idx_ = i;
		}
	}
}

template<int nSTUBS, int nLAYERS, int nLIMIT>
void LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::killLargestResidual() {

	population_[stub_[idx_].layerId] -= 1;

	stub_[idx_].r = 0;
	stub_[idx_].phi = 0;
	stub_[idx_].z = 0;
	stub_[idx_].layerId = 0;
	stub_[idx_].valid = 0;

	residuals_[idx_] = 0;

}

template<int nSTUBS, int nLAYERS, int nLIMIT>
uint1_t LRHLS_v8<nSTUBS, nLAYERS, nLIMIT>::exit_t() {

	int i;

	for(i=0; i<nLAYERS; i++) {
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
