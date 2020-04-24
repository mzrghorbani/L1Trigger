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

template<int I, int LAYERS, int LIMIT>
class LRHLS_v6 {
public:

	LRHLS_v6(TrackHLS<I> *trackIn, TrackHLS<I> *trackOut);
	~LRHLS_v6() {}

	void produce();
	void initFit();
	void calcHelix();
	void calcResidual();
	void killResidual();
	uint1_t exit_t();

public:

	TrackHLS<I> *trackIn_;
	TrackHLS<I> *trackOut_;
	uint3_t population_[LAYERS];
	dtf_t residuals_[I];
	LRTrack parameters_;
	uint3_t foundLayers_;
	LRSums sums_;
};

template<int I, int LAYERS, int LIMIT>
LRHLS_v6<I, LAYERS, LIMIT>::LRHLS_v6(TrackHLS<I> *trackIn, TrackHLS<I> *trackOut) : trackIn_(trackIn), trackOut_(trackOut), foundLayers_(0) {

}


template<int I, int LAYERS, int LIMIT>
void LRHLS_v6<I, LAYERS, LIMIT>::produce() {

	initFit();
	calcHelix();

	for(int i = 0; i < I; i++) {

		if(exit_t())
			break;

		calcResidual();
		killResidual();
	}

	for(int i = 0; i < I; i++) {
		trackOut_->stubs[i] = trackIn_->stubs[i];
	}
}

template<int I, int LAYERS, int LIMIT>
void LRHLS_v6<I, LAYERS, LIMIT>::initFit() {

	uint1_t foundLayers[LAYERS];
#pragma HLS ARRAY_PARTITION variable=foundLayers complete dim=1

	for(int i = 0; i < LAYERS; i++) {
		foundLayers[i] = 0;
		population_[i] = 0;
	}

	for(int i = 0; i < I; i++) {
		population_[trackIn_->stubs[i].layerId] += 1;
		foundLayers[trackIn_->stubs[i].layerId] = 1;
	}

	for(int i = 0; i < LAYERS; i++) {
		foundLayers_ += 1;
	}
}

template<int I, int LAYERS, int LIMIT>
void LRHLS_v6<I, LAYERS, LIMIT>::calcHelix() {

	for(int i = 0; i < I; i++) {
		sums_.rSum = dtf_t(sums_.rSum + (dtf_t(trackIn_->stubs[i].r) >> 3));
		sums_.phiSum = dtf_t(sums_.phiSum + (dtf_t(trackIn_->stubs[i].phi) >> 3));
		sums_.zSum = dtf_t(sums_.zSum + (dtf_t(trackIn_->stubs[i].z) >> 3));
	}

	parameters_.sp = slope(foundLayers_, sums_.rSum, sums_.phiSum);
	parameters_.ip = intercept(foundLayers_, sums_.rSum, sums_.phiSum, parameters_.sp);

	parameters_.sz = slope(foundLayers_, sums_.rSum, sums_.zSum);
	parameters_.iz = intercept(foundLayers_, sums_.rSum, sums_.zSum, parameters_.sz);
}

template<int I, int LAYERS, int LIMIT>
void LRHLS_v6<I, LAYERS, LIMIT>::calcResidual() {

	dtf_t phi_resid = 0;
	dtf_t z_resid = 0;

	for(int i = 0; i < I; i++) {

		phi_resid = residual(trackIn_->stubs[i].r, trackIn_->stubs[i].phi, parameters_.sp, parameters_.ip);
		z_resid = residual(trackIn_->stubs[i].r, trackIn_->stubs[i].z, parameters_.sz, parameters_.iz);

		residuals_[i] = abs_t(phi_resid + z_resid);
	}
}

template<int I, int LAYERS, int LIMIT>
void LRHLS_v6<I, LAYERS, LIMIT>::killResidual() {

	dtf_t largest = residuals_[0];
	uint4_t idx = 0;

	for(int i = 0; i < I; i++) {
		if(residuals_[i] > largest) {
			largest = residuals_[i];
			idx = i;
		}
	}

    population_[trackIn_->stubs[idx].layerId] -= 1;

    trackIn_->stubs[idx].r = 0;
    trackIn_->stubs[idx].phi = 0;
    trackIn_->stubs[idx].z = 0;
    trackIn_->stubs[idx].layerId = 0;
    trackIn_->stubs[idx].valid = 0;

}

template<int I, int LAYERS, int LIMIT>
uint1_t LRHLS_v6<I, LAYERS, LIMIT>::exit_t() {
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
