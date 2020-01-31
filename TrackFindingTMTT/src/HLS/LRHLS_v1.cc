/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v1.h"
#else
#include "LRHLS_v1.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

LRHLS_v1::LRHLS_v1(TrackHLS *trackIn, TrackHLS *trackOut) :
		trackIn_(trackIn), trackOut_(trackOut), nLayers_(0), nStubs_(0), valid_(false) {

	int i;

	for(i = 0; i < 7; i++)
		layerPopulation_[i] = 0;

	for(i = 0; i < 12; i++) {
		stubs_[i].rHLS_ = trackIn_->stubsHLS()[i].rHLS();
		stubs_[i].phiHLS_ = trackIn_->stubsHLS()[i].phiHLS();
		stubs_[i].zHLS_ = trackIn_->stubsHLS()[i].zHLS();
		stubs_[i].layerIdHLS_ = trackIn_->stubsHLS()[i].layerIdHLS();
		stubs_[i].psModuleHLS_ = trackIn_->stubsHLS()[i].psModuleHLS();
		stubs_[i].barrelHLS_ = trackIn_->stubsHLS()[i].barrelHLS();
		stubs_[i].validHLS_ = trackIn_->stubsHLS()[i].validHLS();
	}
}


void LRHLS_v1::produce() {

    int i, j;

    initFit();

    valid_ = checkValidity();

    if (not valid_) {
        returnTrack();
        return;
    }

    for(i = 0; i < 1; i++) {

        calcHelix();
        calcResidual();
        findLargestResid();
        killLargestResid();

        if(nStubs_ < 5)
        	break;

    }
	createTrack();
}

void LRHLS_v1::initFit() {

	int i;

	int nLayers[7];

	for(i = 0; i < 7; i++)
        nLayers[i] = 0;

	for(i = 0; i < 12; i++) {

		StubHLS stub = stubs_[i];

		if(stub.validHLS()) {

			switch(stub.layerIdHLS()) {
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
		nLayers_ = nLayers_ + nLayers[i];
}

bool LRHLS_v1::checkValidity() {

    bool valid = true;

    if(nLayers_ < 3)
        valid = false;

    if(nStubs_ < 3)
        valid = false;

	return valid;
}

void LRHLS_v1::calcHelix() {

	int i;

	sumData phiSums;

	sumData zSums;

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

	for(i = 0; i < 12; i++) {

        StubHLS stub = stubs_[i];

		if(stub.validHLS()) {

			stubData pos(stub.rHLS(), stub.phiHLS(), stub.rHLS(), stub.zHLS());

			switch (stub.layerIdHLS()) {
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

	for (i = 0; i < 7; i++) {

		if (layerPopulation_[i] > 0) {

			phiSums += make_pair_t(layerPos_[i].RPhi, layerPos_[i].Phi);
			zSums += make_pair_t(layerPos_[i].RZ, layerPos_[i].Z);
		}
	}

	const pair_t<dtf_t, dtf_t> &phiParameter = phiSums.calcLinearParameter();
	const pair_t<dtf_t, dtf_t> &zParameter = zSums.calcLinearParameter();

	LRParameter_ = LRParameter(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);

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

	for(i = 0; i < 12; i++) {
	    residuals_[i].phi = 0;
        residuals_[i].z = 0;
        residuals_[i].stubId = 0;
        residuals_[i].layerId = 0;
        residuals_[i].valid = false;
	}

	for(i = 0; i < 12 ; i++) {

        StubHLS stub = stubs_[i];

		if(stub.validHLS()) {

            residData residual;

            for (j = 0; j < 7; j++) {

                if (stub.layerIdHLS() == j) {

                    residual.phi = abs_t(stub.phiHLS() - phiResid[j]);
                    residual.z = abs_t(stub.zHLS() - zResid[j]);
                    residual.stubId = i;
                    residual.layerId = j;
                    residual.valid = true;
                    residuals_[i] = residual;
                }
            }
        }
	}

//	for(i = 0; i < 7; i++) {
//		for(j = 0; j < 12; j++) {
//			if(residuals_[j].layerId == i) {
//				std::cout << residuals_[j].valid << " , ";
//			}
//		}
//		std::cout << std::endl;
//	}

}

void LRHLS_v1::findLargestResid() {

    int i;

    largestResid_.phi = 0;
    largestResid_.z = 0;
    largestResid_.stubId = 0;
    largestResid_.layerId = 0;
    largestResid_.valid = false;

    for(i = 0; i < 12; i++) {

        if(residuals_[i].valid) {

            if (residuals_[i].combined() > largestResid_.combined()) {
                largestResid_ = residuals_[i];
            }
        }
    }
}

void LRHLS_v1::killLargestResid() {

//    for(i = 0; i < 7; i++) {
//        if(layerPopulation_[i] >= 1) {
//            if (largestResid_.layerId == i) {
//                stubs_[largestResid_.stubId].valid = false;
//                layerPopulation_[largestResid_.layerId]--;
//                nStubs_--;
//
//            } else {
//                continue;
//            }
//        }
//    }

    stubs_[largestResid_.stubId].validHLS_ = false;
    layerPopulation_[largestResid_.layerId]--;
    nStubs_--;
}

void LRHLS_v1::returnTrack() {

	int i;

	trackOut_->qOverPtHLS_ = trackIn_->qOverPtHLS_;
	trackOut_->phiHLS_ = trackIn_->phiHLS_;
	trackOut_->cotHLS_ = trackIn_->cotHLS_;
	trackOut_->zHLS_ = trackIn_->zHLS_;
	trackOut_->validHLS_ = true;

	for(i = 0; i < 12; i++) {
		trackOut_->stubsHLS_[i].rHLS_ = trackIn_->stubsHLS_[i].rHLS();
		trackOut_->stubsHLS_[i].phiHLS_ = trackIn_->stubsHLS_[i].phiHLS();
		trackOut_->stubsHLS_[i].zHLS_ = trackIn_->stubsHLS_[i].zHLS();
		trackOut_->stubsHLS_[i].layerIdHLS_ = trackIn_->stubsHLS_[i].layerIdHLS();
		trackOut_->stubsHLS_[i].psModuleHLS_ = trackIn_->stubsHLS_[i].psModuleHLS();
		trackOut_->stubsHLS_[i].barrelHLS_ = trackIn_->stubsHLS_[i].barrelHLS();
		trackOut_->stubsHLS_[i].validHLS_ = trackIn_->stubsHLS_[i].validHLS();
	}
}

void LRHLS_v1::createTrack() {

	int i;

	trackOut_->qOverPtHLS_ = LRParameter_.qOverPt;
	trackOut_->phiHLS_ = LRParameter_.phiT;
	trackOut_->cotHLS_ = LRParameter_.cotT;
	trackOut_->zHLS_ = LRParameter_.zT;
	trackOut_->validHLS_ = true;

	for(i = 0; i < 12; i++) {
        trackOut_->stubsHLS_[i].rHLS_ = stubs_[i].rHLS();
        trackOut_->stubsHLS_[i].phiHLS_ = stubs_[i].phiHLS();
        trackOut_->stubsHLS_[i].zHLS_ = stubs_[i].zHLS();
        trackOut_->stubsHLS_[i].layerIdHLS_ = stubs_[i].layerIdHLS();
        trackOut_->stubsHLS_[i].psModuleHLS_ = stubs_[i].psModuleHLS();
        trackOut_->stubsHLS_[i].barrelHLS_ = stubs_[i].barrelHLS();
        trackOut_->stubsHLS_[i].validHLS_ = stubs_[i].validHLS();
    }
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

