/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v3.h"
#else
#include "LRHLS_v3.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

LRHLS_v3::LRHLS_v3(DataHLS *dataHLS) : data_(dataHLS), nLayers_(0), nStubs_(0), valid_(false) {

	int i, j;

	for(i = 0; i < 10; i++) {
		stubs_[i].r = data_->stubsMHT_[i].r();
		stubs_[i].phi = data_->stubsMHT_[i].phi();
		stubs_[i].z = data_->stubsMHT_[i].z();
		stubs_[i].layerId = data_->stubsMHT_[i].layerId();
		stubs_[i].valid = data_->stubsMHT_[i].valid();
	}

	for(i = 0; i < 7; ++i)
		layerPopulation_[i] = 0;

	nLayers_ = 0;
	nStubs_ = 0;
	valid_ = false;

}

void LRHLS_v3::produce() {

	int i;
	int nIterations = 0;
	int maxIterations = 10;

	initFit();
	valid_ = checkValidity();

	if(not valid_) {
		create();
		return;
	}

	for(i = 0; i < maxIterations; i++) {
		nIterations++;
		calcHelix();
		calcResid();
		bool nothingToKill = killLargestResid();
		if(nothingToKill)
			break;
//		if (nIterations == maxIterations)
//			break;
	}

	create();
}

void LRHLS_v3::initFit() {

	int i;

	uint1_t nLayers[7];

	for(i = 0; i < 7; i++)
		nLayers[i] = 0;

	for(i = 0; i < 10; ++i) {
		LRStub stub = stubs_[i];

		if(stub.valid){
			switch(stub.layerId) {
				case 1:
				layerPopulation_[1]++;
				nLayers[1] = 1;
				nStubs_++;
				break;

				case 2:
				layerPopulation_[2]++;
				nLayers[2] = 1;
				nStubs_++;
				break;

				case 3:
				layerPopulation_[3]++;
				nLayers[3] = 1;
				nStubs_++;
				break;

				case 4:
				layerPopulation_[4]++;
				nLayers[4] = 1;
				nStubs_++;
				break;

				case 5:
				layerPopulation_[5]++;
				nLayers[5] = 1;
				nStubs_++;
				break;

				case 6:
				layerPopulation_[6]++;
				nLayers[6] = 1;
				nStubs_++;
				break;

				default:
				layerPopulation_[0]++;
				nLayers[0] = 1;
				nStubs_++;
				break;
			}
		}
	}

	for(i = 0; i < 7; i++) {
		nLayers_ += nLayers[i];
	}
}

bool LRHLS_v3::checkValidity() {

	int i;

	for(i = 0; i < 7; ++i)
		if(layerPopulation_[i] > 1)
			return true;

	return false;
}


void LRHLS_v3::create() {

	int i;

	for(i = 0; i < 10; i++) {
		data_->stubsLR_[i].r_ = stubs_[i].r;
		data_->stubsLR_[i].phi_ = stubs_[i].phi;
		data_->stubsLR_[i].z_ = stubs_[i].z;
		data_->stubsLR_[i].layerId_ = stubs_[i].layerId;
		data_->stubsLR_[i].valid_ = stubs_[i].valid;
	}
}

void LRHLS_v3::calcHelix() {

	int i, j;

	stubData layerMinPos[7];

	for (i = 0; i < 7; i++) {
		layerMinPos[i].RPhi = 4095;
		layerMinPos[i].Phi = 4095;
		layerMinPos[i].RZ = 4095;
		layerMinPos[i].Z = 4095;
	}

	stubData layerMaxPos[7];

	for (i = 0; i < 7; i++) {
		layerMaxPos[i].RPhi = -4096;
		layerMaxPos[i].Phi = -4096;
		layerMaxPos[i].RZ = -4096;
		layerMaxPos[i].Z = -4096;
	}

	for (i = 0; i < 10; i++) {
		LRStub stub = stubs_[i];

		if (stub.valid) {
			stubData pos(stub.r, stub.phi, stub.r, stub.z);

			switch (stub.layerId) {
				case 1:
					layerMinPos[1] <= pos;
					layerMaxPos[1] >= pos;
					layerPos_[1] = layerMinPos[1] + layerMaxPos[1];
					layerPos_[1] /= 2;
					break;

				case 2:
					layerMinPos[2] <= pos;
					layerMaxPos[2] >= pos;
					layerPos_[2] = layerMinPos[2] + layerMaxPos[2];
					layerPos_[2] /= 2;
					break;

				case 3:
					layerMinPos[3] <= pos;
					layerMaxPos[3] >= pos;
					layerPos_[3] = layerMinPos[3] + layerMaxPos[3];
					layerPos_[3] /= 2;
					break;

				case 4:
					layerMinPos[4] <= pos;
					layerMaxPos[4] >= pos;
					layerPos_[4] = layerMinPos[4] + layerMaxPos[4];
					layerPos_[4] /= 2;
					break;

				case 5:
					layerMinPos[5] <= pos;
					layerMaxPos[5] >= pos;
					layerPos_[5] = layerMinPos[5] + layerMaxPos[5];
					layerPos_[5] /= 2;
					break;

				case 6:
					layerMinPos[6] <= pos;
					layerMaxPos[6] >= pos;
					layerPos_[6] = layerMinPos[6] + layerMaxPos[6];
					layerPos_[6] /= 2;
					break;

				default:
					layerMinPos[0] <= pos;
					layerMaxPos[0] >= pos;
					layerPos_[0] = layerMinPos[0] + layerMaxPos[0];
					layerPos_[0] /= 2;
					break;

			}
		}
	}

	sumData phiSums;

	for(i = 0; i < 7; i++) {
		phiSums += make_pair_t(layerPos_[i].RPhi, layerPos_[i].Phi);
	}

	sumData zSums;

	for(i = 0; i < 7; i++) {
		zSums += make_pair_t(layerPos_[i].RZ, layerPos_[i].Z);
	}

	const pair_t<dtf_t,dtf_t>& phiParameter = phiSums.calcLinearParameter();
	const pair_t<dtf_t,dtf_t>& zParameter = zSums.calcLinearParameter();

	LRParameter_.qOverPt = phiParameter.first;
	LRParameter_.phiT = phiParameter.second;
	LRParameter_.cotTheta = zParameter.first;
	LRParameter_.zT = zParameter.second;

}

void LRHLS_v3::calcResid() {

	int i, j;

	dtf_t phiResid[7];
	dtf_t zResid[7];

	for (i = 0; i < 7; i++) {
		if (layerPopulation_[i] > 0) {
			phiResid[i] = dtf_t(LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * layerPos_[i].RPhi));
			zResid[i] = dtf_t(LRParameter_.zT + dtf_t(LRParameter_.cotTheta * layerPos_[i].RZ));

		} else {
			phiResid[i] = 0;
			zResid[i] = 0;
		}
	}

	for (i = 0; i < 10; i++) {
		residuals_[i].phi = 0;
		residuals_[i].z = 0;
		residuals_[i].stubId = 0;
		residuals_[i].layerId = 0;
		residuals_[i].valid = false;
	}

	for (i = 0; i < 10; i++) {
		LRStub stub = stubs_[i];

		if (stub.valid) {
			residData residual;

			for (j = 0; j < 7; j++) {
				if (stub.layerId == j) {

					residual.phi = abs_t(stub.phi - phiResid[j]);
					residual.z = abs_t(stub.z - zResid[j]);
					residual.stubId = i;
					residual.layerId = j;
					residual.valid = true;
					residuals_[i] = residual;
				}
			}
		}
	}
}


bool LRHLS_v3::killLargestResid() {

	int i, j;

	findLargestResid();

	bool stubCritical = false;
	bool layerCritical = false;
	bool single = true;

	if(nStubs_ <= 4)
		stubCritical = true;

	if(nLayers_ == 2)
		layerCritical = true;

	for(i = 0; i < 7; i++)
		if(layerPopulation_[i] > 1)
			single = false;

	if(stubCritical && layerCritical && single)
		return true;

	stubs_[largestResid_.stubId].valid = false;
	layerPopulation_[largestResid_.layerId]--;

	if(layerPopulation_[largestResid_.layerId] == 0)
		nLayers_--;

	nStubs_--;

	return false;
}

void LRHLS_v3::findLargestResid() {

	int i, j;

	largestResid_.phi = -1.;
	largestResid_.z = -1.;
	largestResid_.stubId = 0;
	largestResid_.layerId = 0;
	largestResid_.valid = false;

	for (i = 0; i < 10; i++) {
		residData residual = residuals_[i];

		if (residual.valid) {
			if (residual.combined() > largestResid_.combined()) {
				largestResid_ = residual;
			}
		}
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

