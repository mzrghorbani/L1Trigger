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

LRHLS_v0::LRHLS_v0(DataHLS *dataHLS) : dataHLS_(dataHLS) {

    int i = 0;

    for(i = 0; i < 7; i++) {
        layerPopulation_[i] = 0;
    }

    nStubs_ = 0;
    nLayers_ = 0;
    nLayersPS_ = 0;
    nIterations_ = 0;
    maxIterations_ = 4;
    valid_ = false;

}


void LRHLS_v0::produce() {

    const array_t<TrackHLS> &tracks = dataHLS_->tracksMHTHLS();

    for (const auto& track : tracks ) {
        trackIn_ = track;
        initFit();
        valid_ = checkValidity();
        if (not valid_) {
            dataHLS_->tracksLRHLS_.push_back(trackIn_);
            continue;
        }

        while(true) {
            nIterations_++;
            calcHelix();
            calcResidual();
            bool nothingToKill = killLargestResidual();
            if (nothingToKill)
                break;
            if(nIterations_ == maxIterations_)
                break;
        }
        create();
    }
}

void LRHLS_v0::initFit() {

    uint4_t i;

    uint3_t layers[7];
    uint3_t layersPS[7];

    for (i = 0; i < 7; ++i) {
        layers[i] = 0;
        layersPS[i] = 0;
        layerPopulation_[i] = 0;
    }

    for (const auto& stub : trackIn_.stubsHLS()) {

        if (stub.validHLS()) {
            switch (stub.layerIdHLS()) {
                case 1:
                    layerPopulation_[1]++;
                    layers[1]++;
                    if (stub.psModuleHLS())
                        layersPS[1]++;
                    break;
                case 2:
                    layerPopulation_[2]++;
                    layers[2]++;
                    if (stub.psModuleHLS())
                        layersPS[2]++;
                    break;
                case 3:
                    layerPopulation_[3]++;
                    layers[3]++;
                    if (stub.psModuleHLS())
                        layersPS[3]++;
                    break;
                case 4:
                    layerPopulation_[4]++;
                    layers[4]++;
                    if (stub.psModuleHLS())
                        layersPS[4]++;
                    break;
                case 5:
                    layerPopulation_[5]++;
                    layers[5]++;
                    if (stub.psModuleHLS())
                        layersPS[5]++;
                    break;
                case 6:
                    layerPopulation_[6]++;
                    layers[6]++;
                    if (stub.psModuleHLS())
                        layersPS[6]++;
                    break;
                default:
                    layerPopulation_[0]++;
                    layers[0]++;
                    if (stub.psModuleHLS())
                        layersPS[0]++;
            }
        }
    }

    for (i = 0; i < 7; ++i) {
        if (layers[i])
            nLayers_ += 1;
        if (layersPS[i])
            nLayersPS_ += 1;
    }
}

bool LRHLS_v0::checkValidity() {

    bool valid = true;

    if (nLayers_ < 4)
        valid = false;
    if (nLayersPS_ < 2)
        valid = false;

    return valid;
}

void LRHLS_v0::calcHelix() {

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

    bool ps[7];

    for (i = 0; i < 7; i++) {
        ps[i] = false;
    }

    for (const auto& stub : trackIn_.stubsHLS()) {

        if (stub.validHLS()) {
            stubData pos(stub.rHLS(), stub.phiHLS(), stub.rHLS(), stub.zHLS());

            switch (stub.layerIdHLS()) {
                case 1:
                    if (stub.psModuleHLS()) {
                        ps[1] = true;
                        layerMinPos[1] <= pos;
                        layerMaxPos[1] >= pos;
                    } else {
                        layerMinPos[1].RPhi = min_t(layerMinPos[1].RPhi, pos.RPhi);
                        layerMinPos[1].RPhi = min_t(layerMinPos[1].Phi, pos.Phi);
                        layerMaxPos[1].RPhi = max_t(layerMaxPos[1].RPhi, pos.RPhi);
                        layerMaxPos[1].RPhi = max_t(layerMaxPos[1].Phi, pos.Phi);
                    }
                    layerPos_[1] = layerMinPos[1] + layerMaxPos[1];
                    layerPos_[1] /= 2;
                    break;

                case 2:
                    if (stub.psModuleHLS()) {
                        ps[2] = true;
                        layerMinPos[2] <= pos;
                        layerMaxPos[2] >= pos;
                    } else {
                        layerMinPos[2].RPhi = min_t(layerMinPos[2].RPhi, pos.RPhi);
                        layerMinPos[2].RPhi = min_t(layerMinPos[2].Phi, pos.Phi);
                        layerMaxPos[2].RPhi = max_t(layerMaxPos[2].RPhi, pos.RPhi);
                        layerMaxPos[2].RPhi = max_t(layerMaxPos[2].Phi, pos.Phi);
                    }
                    layerPos_[2] = layerMinPos[2] + layerMaxPos[2];
                    layerPos_[2] /= 2;
                    break;

                case 3:
                    if (stub.psModuleHLS()) {
                        ps[3] = true;
                        layerMinPos[3] <= pos;
                        layerMaxPos[3] >= pos;
                    } else {
                        layerMinPos[3].RPhi = min_t(layerMinPos[3].RPhi, pos.RPhi);
                        layerMinPos[3].RPhi = min_t(layerMinPos[3].Phi, pos.Phi);
                        layerMaxPos[3].RPhi = max_t(layerMaxPos[3].RPhi, pos.RPhi);
                        layerMaxPos[3].RPhi = max_t(layerMaxPos[3].Phi, pos.Phi);
                    }
                    layerPos_[3] = layerMinPos[3] + layerMaxPos[3];
                    layerPos_[3] /= 2;
                    break;

                case 4:
                    if (stub.psModuleHLS()) {
                        ps[4] = true;
                        layerMinPos[4] <= pos;
                        layerMaxPos[4] >= pos;
                    } else {
                        layerMinPos[4].RPhi = min_t(layerMinPos[4].RPhi, pos.RPhi);
                        layerMinPos[4].RPhi = min_t(layerMinPos[4].Phi, pos.Phi);
                        layerMaxPos[4].RPhi = max_t(layerMaxPos[4].RPhi, pos.RPhi);
                        layerMaxPos[4].RPhi = max_t(layerMaxPos[4].Phi, pos.Phi);
                    }
                    layerPos_[4] = layerMinPos[4] + layerMaxPos[4];
                    layerPos_[4] /= 2;
                    break;

                case 5:
                    if (stub.psModuleHLS()) {
                        ps[5] = true;
                        layerMinPos[5] <= pos;
                        layerMaxPos[5] >= pos;
                    } else {
                        layerMinPos[5].RPhi = min_t(layerMinPos[5].RPhi, pos.RPhi);
                        layerMinPos[5].RPhi = min_t(layerMinPos[5].Phi, pos.Phi);
                        layerMaxPos[5].RPhi = max_t(layerMaxPos[5].RPhi, pos.RPhi);
                        layerMaxPos[5].RPhi = max_t(layerMaxPos[5].Phi, pos.Phi);
                    }
                    layerPos_[5] = layerMinPos[5] + layerMaxPos[5];
                    layerPos_[5] /= 2;
                    break;

                case 6:
                    if (stub.psModuleHLS()) {
                        ps[6] = true;
                        layerMinPos[6] <= pos;
                        layerMaxPos[6] >= pos;
                    } else {
                        layerMinPos[6].RPhi = min_t(layerMinPos[6].RPhi, pos.RPhi);
                        layerMinPos[6].RPhi = min_t(layerMinPos[6].Phi, pos.Phi);
                        layerMaxPos[6].RPhi = max_t(layerMaxPos[6].RPhi, pos.RPhi);
                        layerMaxPos[6].RPhi = max_t(layerMaxPos[6].Phi, pos.Phi);
                    }
                    layerPos_[6] = layerMinPos[6] + layerMaxPos[6];
                    layerPos_[6] /= 2;
                    break;

                default:
                    if (stub.psModuleHLS()) {
                        ps[0] = true;
                        layerMinPos[0] <= pos;
                        layerMaxPos[0] >= pos;
                    } else {
                        layerMinPos[0].RPhi = min_t(layerMinPos[0].RPhi, pos.RPhi);
                        layerMinPos[0].RPhi = min_t(layerMinPos[0].Phi, pos.Phi);
                        layerMaxPos[0].RPhi = max_t(layerMaxPos[0].RPhi, pos.RPhi);
                        layerMaxPos[0].RPhi = max_t(layerMaxPos[0].Phi, pos.Phi);
                    }
                    layerPos_[0] = layerMinPos[0] + layerMaxPos[0];
                    layerPos_[0] /= 2;
                    break;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        if (layerPopulation_[i] > 0) {
            phiSums += make_pair_t(layerPos_[i].RPhi, layerPos_[i].Phi);
            if (ps[i] == 1)
                zSums += make_pair_t(layerPos_[i].RZ, layerPos_[i].Z);
        }
    }

    const pair_t<dtf_t, dtf_t> &phiParameter = phiSums.calcLinearParameter();
    const pair_t<dtf_t, dtf_t> &zParameter = zSums.calcLinearParameter();

    LRParameter_ = LRTrack(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);
}

void LRHLS_v0::calcResidual() {

    int i = 0;
    int j = 0;

    dtf_t phiResid[7];
    dtf_t zResid[7];

    for (i = 0; i < 7; ++i) {
        phiResid[i] = 0;
        zResid[i] = 0;
    }

    for (i = 0; i < 7; i++) {
        if (layerPopulation_[i] > 0) {
            phiResid[i] = dtf_t(LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * layerPos_[i].Phi));
            zResid[i] = dtf_t(LRParameter_.zT + dtf_t(LRParameter_.cotTheta * layerPos_[i].Z));
        }
    }

    residuals_.clear();
    residData residual;

    for (i = 0; i < trackIn_.stubsHLS().size(); i++) {
        if (trackIn_.stubsHLS()[i].validHLS()) {

            for (j = 0; j < 7; j++) {
                if (trackIn_.stubsHLS()[i].layerIdHLS() == j) {

                    residual.phi = abs_t(trackIn_.stubsHLS()[i].phiHLS() - phiResid[j]);
                    residual.z = abs_t(trackIn_.stubsHLS()[i].zHLS() - zResid[j]);
                    residual.layerId = j;
                    residual.stubId = i;
                    if (trackIn_.stubsHLS()[i].psModuleHLS())
                        residual.ps = true;
                    residual.valid = trackIn_.stubsHLS()[i].validHLS();
                    residuals_.push_back(residual);
                    break;
                }
            }
        }
    }
}

bool LRHLS_v0::killLargestResidual() {

    findLargestResidual();

    if(nStubs_ <= 3) {
        return true;
    }

    trackIn_.stubsHLS_[largestResid_.stubId].validHLS_ = false;
    layerPopulation_[largestResid_.layerId]--;
    nStubs_--;

    return false;
}

void LRHLS_v0::findLargestResidual() {

    int i = 0;

    largestResid_.phi = 0;
    largestResid_.z = 0;
    largestResid_.layerId = 0;
    largestResid_.stubId = 0;
    largestResid_.valid = false;
    largestResid_.ps = false;

    for(i = 0; i < 7; i++) {
        if(layerPopulation_[i] == 0)
            continue;
        uint1_t single = layerPopulation_[i] == 1;
        uint1_t notPurged = nStubs_ != nLayers_;
        uint1_t layerCritical = nLayers_ == 4;
        uint1_t psCritical = nLayers_ == 2;
        uint1_t critical = nStubs_ == 4;
        if(single && notPurged && layerCritical)
            continue;
        for(auto residual : residuals_) {
            if (residual.ps && psCritical && !critical) {
                if ((nStubs_ == 2) || single) {
                    continue;
                }
            }
            if(residual.combined() > largestResid_.combined()) {
                largestResid_ = residual;
            }
        }
    }
}

void LRHLS_v0::create() {

    trackOut_ = trackIn_;

    trackOut_.qOverPtHLS_ = LRParameter_.qOverPt;
    trackOut_.phiHLS_ = LRParameter_.phiT;
    trackOut_.cotHLS_ = LRParameter_.cotTheta;
    trackOut_.zHLS_ = LRParameter_.zT;

    int i = 0;

    for (const auto& stub : trackOut_.stubsHLS()) {
        trackOut_.stubsHLS_[i].rHLS_ = stub.rHLS();
        trackOut_.stubsHLS_[i].phiHLS_ = stub.phiHLS();
        trackOut_.stubsHLS_[i].zHLS_ = stub.zHLS();
        trackOut_.stubsHLS_[i].validHLS_ = stub.validHLS();
        trackOut_.stubsHLS_[i].layerIdHLS_ = stub.layerIdHLS();
        trackOut_.stubsHLS_[i].psModuleHLS_ = stub.psModuleHLS();
        trackOut_.stubsHLS_[i].barrelHLS_ = stub.barrelHLS();
        i++;
    }

    dataHLS_->tracksLRHLS_.push_back(trackOut_);
}


#ifdef CMSSW_GIT_HASH
}

}
#endif

