/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v0.h"
#else
#include "LRHLS_v0.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

LRHLS_v0::LRHLS_v0(const SettingsHLS *settingsHLS, DataHLS *dataHLS) :
    settingsHLS_(settingsHLS),
    dataHLS_(dataHLS),
    numLayers_(settingsHLS->trackerNumLayers()),
    minLayers_(settingsHLS->lrMinLayers()),
    minLayersPS_(settingsHLS->lrMinLayersPS()),
    residPhi_(settingsHLS->lrResidPhi()),
    residZ2S_(settingsHLS->lrResidZ2S()),
    residZPS_(settingsHLS->lrResidZPS()),
    maxIteartions_(settingsHLS->lrNumIterations()),
	secPhi_(0), secEta_(0), nIterations_(0), nStubs_(0), valid_(false) {
}

void LRHLS_v0::produce() {

	int k = 0;
	uint4_t j;

    for (const auto& track : dataHLS_->tracksMHTHLS()) {
        track_ = track;
        initFit();
        if (not valid_) {
            create();
            continue;
        }
        while(true) {
            nIterations_++;
            calcHelix();
            calcResidual();
            bool nothingToKill = killLargestResidual();
            if (nothingToKill)
                break;
            if (nIterations_ == maxIteartions_)
                break;
        }
        create();

        if(valid_)
            dataHLS_->tracksLRHLS_.push_back(track_);
    }
}

void LRHLS_v0::initFit() {

    uint4_t i, j;

    nIterations_ = 0;
    nStubs_ = 0;
    secPhi_ = track_.secPhi();
    secEta_ = track_.secEta();
    HTParameter_ = LRTrack(track_.qOverPt(), track_.phi(), track_.cot(), track_.z());
    LRParameter_ = HTParameter_;
    stubs_.clear();
    for (const auto& stub : track_.stubs())
        stubs_.push_back(stub);

    valid_ = checkValidity(stubs_);
    if (not valid_)
        return;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 4; j++) {
            stubMap_[i][j].r_ = 0;
            stubMap_[i][j].phi_ = 0;
            stubMap_[i][j].z_ = 0;
            stubMap_[i][j].layerId_ = 0;
            stubMap_[i][j].psModule_ = false;
            stubMap_[i][j].barrel_ = false;
            stubMap_[i][j].valid_ = false;
        }
    }

    for (i = 0; i < 7; i++)
        layerPopulation_[i] = 0;

    uint3_t j0 = 0;
    uint3_t j1 = 0;
    uint3_t j2 = 0;
    uint3_t j3 = 0;
    uint3_t j4 = 0;
    uint3_t j5 = 0;
    uint3_t j6 = 0;

    for (const auto& stub : stubs_) {
        if (stub.valid()) {
            switch (stub.layerId()) {
            case 1:
                stubMap_[1][j1] = stub;
                layerPopulation_[1]++;
                nStubs_++;
                j1++;
                break;
            case 2:
                stubMap_[2][j2] = stub;
                layerPopulation_[2]++;
                nStubs_++;
                j2++;
                break;
            case 3:
                stubMap_[3][j3] = stub;
                layerPopulation_[3]++;
                nStubs_++;
                j3++;
                break;
            case 4:
                stubMap_[4][j4] = stub;
                layerPopulation_[4]++;
                nStubs_++;
                j4++;
                break;
            case 5:
                stubMap_[5][j5] = stub;
                layerPopulation_[5]++;
                nStubs_++;
                j5++;
                break;
            case 6:
                stubMap_[6][j6] = stub;
                layerPopulation_[6]++;
                nStubs_++;
                j6++;
                break;
            default:
                stubMap_[0][j0] = stub;
                layerPopulation_[0]++;
                nStubs_++;
                j0++;
                break;
            }
        }
    }
    largestResid_ = residData(-1. , -1., 0, 0, false, false);
}

bool LRHLS_v0::checkValidity(const array_s<StubHLS>& stubs) const {

    bool valid = true;

    if (countLayers(stubs) < minLayers_)
        valid = false;

    if (countLayers(stubs, true) < minLayersPS_)
        valid = false;

    return valid;
}

uint3_t LRHLS_v0::countLayers(const array_s<StubHLS>& stubs, const bool &onlySeed) const {

    uint4_t i;
    uint4_t nLayers = 0;
    uint1_t foundLayers[7];

    for (i = 0; i < 7; i++)
    	foundLayers[i] = 0;

    for (const auto& stub : stubs) {
        if (stub.valid()) {
            if (!onlySeed || stub.psModule()) {
                foundLayers[stub.layerId()] = 1;
            }
        }
    }

    for (i = 0; i < 7; i++)
    	nLayers += foundLayers[i];

    return nLayers;
}

void LRHLS_v0::calcHelix() {

    uint4_t i, j;

    sumData phiSums, zSums;

    for (i = 0; i < 7; i++) {
        stubData layerMinPos(4096, 4096, 4096, 4096);
        stubData layerMaxPos(-4096, -4096, -4096, -4096);
        bool ps( false );
        for (j = 0; j < 4; j++) {
        	StubHLS stub = stubMap_[i][j];
        	if (stub.valid()) {
				stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());
				if (stub.psModule()) {
					ps = true;
					layerMinPos <= pos;
					layerMaxPos >= pos;
				} else {
					layerMinPos.RPhi = min_t(layerMinPos.RPhi, pos.RPhi);
					layerMinPos.Phi = min_t(layerMinPos.Phi, pos.Phi);
					layerMaxPos.RPhi = max_t(layerMaxPos.RPhi, pos.RPhi);
					layerMaxPos.Phi = max_t(layerMaxPos.Phi, pos.Phi);
				}
				stubData layerPos = layerMinPos + layerMaxPos;
				layerPos /= 2.;
				phiSums += make_pair_t(layerPos.RPhi, layerPos.Phi);
				if ( ps )
					zSums += make_pair_t(layerPos.RZ, layerPos.Z);
        	}
        }
    }
    const pair_t<dtf_t,dtf_t>& phiParameter = phiSums.calcLinearParameter();
    const pair_t<dtf_t,dtf_t>& zParameter = zSums.calcLinearParameter();
    LRParameter_ = LRTrack(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);
}

void LRHLS_v0::calcResidual() {

    uint4_t i, j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 4; j++) {
            residuals_[i][j].phi = 0;
            residuals_[i][j].z = 0;
            residuals_[i][j].stubId = 0;
            residuals_[i][j].layerId = 0;
            residuals_[i][j].ps = false;
            residuals_[i][j].valid = false;
        }
    }

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 4; j++) {
        	StubHLS stub = stubMap_[i][j];
        	if (stub.valid()) {
				stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());
				dtf_t zResid = pos.Z - (LRParameter_.zT + LRParameter_.cotTheta * pos.RZ);
				dtf_t phiResid = pos.Phi - (LRParameter_.phiT + LRParameter_.qOverPt * pos.RPhi);
				residData resid(abs_t(phiResid), abs_t(zResid), i, j, stub.psModule(), true);
				resid.phi /= residPhi_;
				if (!stub.barrel())
					resid.z /= abs_t(HTParameter_.cotTheta);
				if (stub.psModule())
					resid.z /= residZPS_;
				else
					resid.z /= residZ2S_;
				residuals_[i][j] = resid;
			}
        }
    }
}

bool LRHLS_v0::killLargestResidual() {

	uint4_t i, j;
	uint4_t k = 0;

    findLargestResidual();
    valid_ = largestResid_.combined() < 2.;
    if (countStubs(stubs_) == 4) {
        return true;
    }
    const uint3_t & layerID = largestResid_.layerId;
    const uint4_t & stubID = largestResid_.stubId;
    stubMap_[layerID][stubID].valid_ = false;

    stubs_.clear();

    for (i = 0; i < 7; i++)
    	for (j = 0; j < 4; j++)
    		if (stubMap_[i][j].valid_)
    			stubs_.push_back(stubMap_[i][j]);

    layerPopulation_[layerID]--;
    nStubs_--;

    return false;
}

void LRHLS_v0::findLargestResidual() {

    uint4_t i, j;

    largestResid_.phi = -1.;
    largestResid_.z = -1.;
    largestResid_.stubId = 0;
    largestResid_.layerId = 0;
    largestResid_.ps = false;
    largestResid_.valid = false;

    for (i = 0; i < 7; i++) {
    	if (layerPopulation_[i] == 0)
    		continue;
		bool single = layerPopulation_[i] == 1;
		bool notPurged = countStubs(stubs_, false) != countLayers(stubs_, false);
		bool layerCritical = countLayers(stubs_, false) == minLayers_;
		bool psCritical = countLayers(stubs_, true) == minLayersPS_;
		if (single && notPurged && layerCritical)
			continue;
		for (j = 0; j < 4; j++) {
			residData residual = residuals_[i][j];
			if (residual.valid) {
				if (residual.ps && psCritical && countStubs(stubs_) > 4) {
					if (layerPopulation_[i] == 1) {
						continue;
					}
				}
				if (residual.combined() > largestResid_.combined()) {
					largestResid_ = residual;
				}
			}
		}
    }
}

uint4_t LRHLS_v0::countStubs(const array_s<StubHLS>& stubs, const bool &onlyPS) const {

	uint4_t i;
    uint4_t nStubs(0);

    if (onlyPS) {
        for (const auto& stub : stubs) {
            if (stub.valid()) {
                if (stub.psModule()) {
                    nStubs++;
                }
            }
        }
    } else {
        nStubs = nStubs_;
    }
    return nStubs;
}

void LRHLS_v0::create() {

	uint4_t i;
	int k = 0;

    if (valid_) {
        LRParameter_ += HTParameter_;
        TrackHLS track;
        track.qOverPt_ = LRParameter_.qOverPt;
        track.phi_ = LRParameter_.phiT;
        track.cot_ = LRParameter_.cotTheta;
        track.z_ = LRParameter_.zT;
        track.secPhi_ = secPhi_;
        track.secEta_ = secEta_;
        track.valid_ = true;
        for (const auto& stub : stubs_)
        	if(stub.valid())
        		track.stubs_.push_back(stub);

        track_ = track;
    }
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

