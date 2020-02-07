/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v2.h"
#else
#include "LRHLS_v2.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

LRHLS_v2::LRHLS_v2(TrackHLS *trackIn, TrackHLS *trackOut) :
        trackIn_(trackIn), trackOut_(trackOut), nLayers_(0), nStubs_(0), valid_(false) {

    int i;

    for(i = 0; i < 7; i++)
        layerPopulation_[i] = 0;

    for (i = 0; i < 12; i++) {
        stubs_[i].r = trackIn_->stubs_[i].r_;
        stubs_[i].phi = trackIn_->stubs_[i].phi_;
        stubs_[i].z = trackIn_->stubs_[i].z_;
        stubs_[i].layerId = trackIn_->stubs_[i].layerId_;
        stubs_[i].valid = trackIn_->stubs_[i].valid_;
    }


}


void LRHLS_v2::produce() {

    initFit();
    valid_ = checkValidity();

    if(not valid_) {
        returnTrack();
        return;
    }

    calcHelix();
    calcResidual();
    findLeastResid();
    keepLeastResid();
    createTrack();

}

void LRHLS_v2::initFit() {

    int i;

    uint3_t nLayers[7];

    for(i = 0; i < 7; i++)
        nLayers[i] = 0;

    for(i = 0; i < 12; i++) {

        LRStub stub = stubs_[i];

        if(stub.valid) {

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

    for(i = 0; i < 7; i++)
        nLayers_ = nLayers_ + nLayers[i];
}

bool LRHLS_v2::checkValidity() {

    bool valid = true;

    if(nLayers_ < 4)
        valid = false;

    if(nStubs_ < 4)
        valid = false;

    return valid;
}

void LRHLS_v2::calcHelix() {

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

        LRStub stub = stubs_[i];

        if(stub.valid) {

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

    for (i = 0; i < 7; i++) {

        if (layerPopulation_[i] > 0) {

            phiSums += make_pair_t(layerPos_[i].RPhi, layerPos_[i].Phi);
            zSums += make_pair_t(layerPos_[i].RZ, layerPos_[i].Z);
        }
    }

    const pair_t<dtf_t, dtf_t> &phiParameter = phiSums.calcLinearParameter();
    const pair_t<dtf_t, dtf_t> &zParameter = zSums.calcLinearParameter();

    LRParameter_.qOverPt = phiParameter.first;
    LRParameter_.phiT = phiParameter.second;
    LRParameter_.cotT = zParameter.first;
    LRParameter_.zT = zParameter.second;

}

void LRHLS_v2::calcResidual() {

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

    for(i = 0; i < 12 ; i++) {

        LRStub stub = stubs_[i];

        if(stub.valid) {

            residData residual;

            for(j = 0; j < 7; j++) {

                if(stub.layerId == j) {

                    residual.phi = abs_t(stub.phi - phiResid[j]);
                    residual.z = abs_t(stub.z - zResid[j]);
                    residual.valid = stub.valid;
                    residual.keep = false;
                    residual.stubId = i;
                    residual.layerId = j;
                }

            }
            residuals_[i] = residual;
        }
    }

}

void LRHLS_v2::findLeastResid() {

    int i;

    for(i = 0; i < 7; i++) {
        layerMinResid_[i].phi = 4096;
        layerMinResid_[i].z = 4096;
        layerMinResid_[i].layerId = 0;
        layerMinResid_[i].stubId = 0;
        layerMinResid_[i].valid = false;
    }

    for(i = 0; i < 12; i++) {

        if(residuals_[i].valid) {

            switch(residuals_[i].layerId) {
                case 1:
                    if(residuals_[i].combined() < layerMinResid_[1].combined()) {
                        layerMinResid_[1] = residuals_[i];
                    }
                    break;

                case 2:
                    if(residuals_[i].combined() < layerMinResid_[2].combined()) {
                        layerMinResid_[2] = residuals_[i];
                    }
                    break;

                case 3:
                    if(residuals_[i].combined() < layerMinResid_[3].combined()) {
                        layerMinResid_[3] = residuals_[i];
                    }
                    break;

                case 4:
                    if(residuals_[i].combined() < layerMinResid_[4].combined()) {
                        layerMinResid_[4] = residuals_[i];
                    }
                    break;

                case 5:
                    if(residuals_[i].combined() < layerMinResid_[5].combined()) {
                        layerMinResid_[5] = residuals_[i];
                    }
                    break;

                case 6:
                    if(residuals_[i].combined() < layerMinResid_[6].combined()) {
                        layerMinResid_[6] = residuals_[i];
                    }
                    break;

                default:
                    if(residuals_[i].combined() < layerMinResid_[0].combined()) {
                        layerMinResid_[0] = residuals_[i];
                    }
                    break;
            }
        }
    }

}

void LRHLS_v2::keepLeastResid() {

    int i, j;

    for(i = 0; i < 12; i++) {
        for(j = 0; j < 7; j++) {
            if (residuals_[i].stubId == layerMinResid_[j].stubId) {
                residuals_[i].keep = true;
            }
        }
    }
}

void LRHLS_v2::returnTrack() {

    int i;

    trackOut_->size_ = trackIn_->size();
    trackOut_->qOverPt_ = trackIn_->qOverPt_;
    trackOut_->phi_ = trackIn_->phi_;
    trackOut_->cot_ = trackIn_->cot_;
    trackOut_->z_ = trackIn_->z_;
    trackOut_->valid_ = trackIn_->valid_;

    for(i = 0; i < 12; i++) {
        if(trackIn_->stubs_[i].valid()) {

            trackOut_->stubs_[i].r_ = trackIn_->stubs_[i].r();
            trackOut_->stubs_[i].phi_ = trackIn_->stubs_[i].phi();
            trackOut_->stubs_[i].z_ = trackIn_->stubs_[i].z();
            trackOut_->stubs_[i].layerId_ = trackIn_->stubs_[i].layerId();
            trackOut_->stubs_[i].valid_ = trackIn_->stubs_[i].valid();
        }
    }
}

void LRHLS_v2::createTrack() {

    int i;

    trackOut_->size_ = nStubs_;
    trackOut_->qOverPt_ = LRParameter_.qOverPt;
    trackOut_->phi_ = LRParameter_.phiT;
    trackOut_->cot_ = LRParameter_.cotT;
    trackOut_->z_ = LRParameter_.zT;
    trackOut_->valid_ = trackIn_->valid_;

    for(i = 0; i < 12; i++) {

        if(residuals_[i].keep) {

            trackOut_->stubs_[i].r_ = trackIn_->stubs_[i].r();
            trackOut_->stubs_[i].phi_ = trackIn_->stubs_[i].phi();
            trackOut_->stubs_[i].z_ = trackIn_->stubs_[i].z();
            trackOut_->stubs_[i].layerId_ = trackIn_->stubs_[i].layerId();
            trackOut_->stubs_[i].valid_ = trackIn_->stubs_[i].valid();
        }
    }
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

