/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface//LRHLS_v1.h"
#else
#include "LRHLS_v1.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace  {
#endif

LRHLS_v1::LRHLS_v1(TrackHLS *trackIn, TrackHLS *trackOut) :
		trackIn_(trackIn), trackOut_(trackOut), nLayers_(0), nStubs_(0), valid_(false) {

	int i;

	for (i = 0; i < 7; i++)
		layerPopulation_[i] = 0;

	for (i = 0; i < 12; i++) {
		stubs_[i].r = trackIn_->stubs_[i].r_;
		stubs_[i].phi = trackIn_->stubs_[i].phi_;
		stubs_[i].z = trackIn_->stubs_[i].z_;
		stubs_[i].layerId = trackIn_->stubs_[i].layerId_;
		stubs_[i].valid = trackIn_->stubs_[i].valid_;
	}
}


void LRHLS_v1::produce() {

    int i;

    initFit();

    valid_ = checkValidity();

    if (not valid_) {
        returnTrack();
        return;
    }

    int maxIterations = 12;

    for (i = 0; i < maxIterations; i++) {

		calcHelix();
		calcResidual();
		findLargestResid();
		killLargestResid();

		if((nLayers_ < 3) && (nStubs_ < 3))
			break;
    }
	createTrack();
}

void LRHLS_v1::initFit() {

	int i;

	int nLayers[7];

	for(i = 0; i < 7; i++)
		nLayers[i] = 0;

	for (i = 0; i < 12; i++) {

		LRStub stub = stubs_[i];

		if (stub.valid) {

			switch (stub.layerId) {
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

	for(i = 0; i < 7; i++)
		nLayers_ += nLayers[i];
}

bool LRHLS_v1::checkValidity() {

	int i;

    for (i = 0; i < 7; i++)
    	if (layerPopulation_[i] > 1)
    		return true;

	return false;
}

void LRHLS_v1::calcHelix() {

	int i;

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

	for (i = 0; i < 12; i++) {

        LRStub stub = stubs_[i];

		if (stub.valid) {

			stubData pos;
			pos.RPhi = stub.r;
			pos.Phi = stub.phi;
			pos.RZ = stub.r;
			pos.Z = stub.z;

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

	sumData phiSum[7];

	for (i = 0; i < 7; i++) {
		if (layerPopulation_[i] > 0) {
			phiSum[i] += make_pair_t(layerPos_[i].RPhi, layerPos_[i].Phi);
		}
	}

	sumData zSum[7];

	for (i = 0; i < 7; i++) {
		if (layerPopulation_[i] > 0) {
			zSum[i] += make_pair_t(layerPos_[i].RZ, layerPos_[i].Z);
		}
	}

	sumData phiSums;

	for(i = 0; i < 7; i++) {
		phiSums += phiSum[i];
	}

	sumData zSums;

	for(i = 0; i < 7; i++) {
		zSums += zSum[i];
	}

	const pair_t<dtf_t, dtf_t> &phiParameter = phiSums.calcLinearParameter();
	const pair_t<dtf_t, dtf_t> &zParameter = zSums.calcLinearParameter();

	LRParameter_.qOverPt = phiParameter.first;
	LRParameter_.phiT = phiParameter.second;
	LRParameter_.cotT = zParameter.first;
	LRParameter_.zT = zParameter.second;

}

void LRHLS_v1::calcResidual() {

	int i, j;

	dtf_t phiResid[7];
	dtf_t zResid[7];

	for (i = 0; i < 7; i++) {
		if (layerPopulation_[i] > 0) {
			phiResid[i] = dtf_t(LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * layerPos_[i].Phi));
			zResid[i] = dtf_t(LRParameter_.zT + dtf_t(LRParameter_.cotT * layerPos_[i].Z));

		} else {
			phiResid[i] = 0;
			zResid[i] = 0;
		}
	}

	for (i = 0; i < 12; i++) {
	    residuals_[i].phi = 0;
        residuals_[i].z = 0;
        residuals_[i].stubId = 0;
        residuals_[i].layerId = 0;
        residuals_[i].valid = false;
	}

	for (i = 0; i < 12 ; i++) {

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

void LRHLS_v1::findLargestResid() {

    int i;

    largestResid_.phi = 0;
    largestResid_.z = 0;
    largestResid_.stubId = 0;
    largestResid_.layerId = 0;
    largestResid_.valid = false;

    for (i = 0; i < 12; i++) {

        if (residuals_[i].valid) {

            if (residuals_[i].combined() > largestResid_.combined()) {
                largestResid_ = residuals_[i];
            }
        }
    }
}

void LRHLS_v1::killLargestResid() {

	stubs_[largestResid_.stubId].valid = false;
    layerPopulation_[largestResid_.layerId]--;

    if(layerPopulation_[largestResid_.layerId] == 0)
    	nLayers_--;

    nStubs_--;
}

void LRHLS_v1::returnTrack() {

	int i;

	trackOut_->qOverPt_ = trackIn_->qOverPt();
	trackOut_->phi_ = trackIn_->phi();
	trackOut_->cot_ = trackIn_->cot();
	trackOut_->z_ = trackIn_->z();
	trackOut_->valid_ = trackIn_->valid();

	for (i = 0; i < 12; i++)
		trackOut_->stubs_[i] = trackIn_->stubs_[i];

#ifndef __SYNTHESIS__
    std::cout << " rejected" << std::endl;
#endif

}

void LRHLS_v1::createTrack() {

	int i;

	trackOut_->qOverPt_ = LRParameter_.qOverPt;
	trackOut_->phi_ = LRParameter_.phiT;
	trackOut_->cot_ = LRParameter_.cotT;
	trackOut_->z_ = LRParameter_.zT;
	trackOut_->valid_ = trackIn_->valid();

	for (i = 0; i < 12; i++) {
        trackOut_->stubs_[i].r_ = stubs_[i].r;
        trackOut_->stubs_[i].phi_ = stubs_[i].phi;
        trackOut_->stubs_[i].z_ = stubs_[i].z;
        trackOut_->stubs_[i].layerId_ = stubs_[i].layerId;
        trackOut_->stubs_[i].valid_ = stubs_[i].valid;
	}

#ifndef __SYNTHESIS__
    std::cout << " accepted" << std::endl;
    std::cout << "qOverPt : " << trackOut_->qOverPt() << std::endl;
    std::cout << "phi : " << trackOut_->phi() << std::endl;
    std::cout << "cot : " << trackOut_->cot() << std::endl;
    std::cout << "z : " << trackOut_->z() << std::endl;

    for(i = 0; i < 12; i++)
        if(trackOut_->stubs_[i].valid())
            std::cout << "layer : " << trackOut_->stubs_[i].layerId() << std::endl;
#endif

}

#ifdef CMSSW_GIT_HASH
}

}
#endif

