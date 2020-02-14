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

    uint4_t i;

    for (i = 0; i < 7; i++)
        layerPopulation_[i] = 0;

    LRParameter_.qOverPt = trackIn->qOverPt();
    LRParameter_.phiT = trackIn->phi();
    LRParameter_.cotTheta = trackIn->cot();
    LRParameter_.zT = trackIn->z();
    trackOut->secPhi_ = trackIn->secPhi();
    trackOut->secEta_ = trackIn->secEta();
    trackOut->valid_ = trackIn->valid();

    stubs_[0] = trackIn->stubs_[0];
    stubs_[1] = trackIn->stubs_[1];
    stubs_[2] = trackIn->stubs_[2];
    stubs_[3] = trackIn->stubs_[3];
    stubs_[4] = trackIn->stubs_[4];
    stubs_[5] = trackIn->stubs_[5];
    stubs_[6] = trackIn->stubs_[6];
    stubs_[7] = trackIn->stubs_[7];
    stubs_[8] = trackIn->stubs_[8];
    stubs_[9] = trackIn->stubs_[9];
    stubs_[10] = trackIn->stubs_[10];
    stubs_[11] = trackIn->stubs_[11];
}


void LRHLS_v1::produce() {

    uint4_t i, j;

    initFit();

    valid_ = checkValidity();

    if (not valid_) {
        createTrack();
        return;
    }

    uint4_t maxIterations = 10;

    for (i = 0; i < maxIterations; i++) {

        calcHelix();
        calcResidual();
        bool nothingToKill = killLargestResid();
        if (nothingToKill)
            break;
    }
    createTrack();
}

void LRHLS_v1::initFit() {

    uint4_t i;

    uint3_t nLayers[7];

    for (i = 0; i < 7; i++)
        nLayers[i] = 0;

    for (i = 0; i < 12; i++) {
        StubHLS stub = stubs_[i];

        if (stub.valid()) {
            switch (stub.layerId()) {
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

    for (i = 0; i < 7; i++)
        nLayers_ += nLayers[i];
}

bool LRHLS_v1::checkValidity() {

    uint4_t i;

    for (i = 0; i < 7; i++)
        if (layerPopulation_[i] > 1)
            return true;

    return false;
}

void LRHLS_v1::calcHelix() {

    uint4_t i;

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
        StubHLS stub = stubs_[i];

        if (stub.valid()) {
            stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());

            switch (stub.layerId()) {
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
            phiSum[i].n++;
            phiSum[i].xy += dtf_t(layerPos_[i].RPhi * layerPos_[i].Phi);
            phiSum[i].x += layerPos_[i].RPhi;
            phiSum[i].y += layerPos_[i].Phi;
            phiSum[i].xx += dtf_t(layerPos_[i].RPhi * layerPos_[i].RPhi);
        }
    }

    sumData zSum[7];

    for (i = 0; i < 7; i++) {
        if (layerPopulation_[i] > 0) {
            zSum[i].n++;
            zSum[i].xy += dtf_t(layerPos_[i].RZ * layerPos_[i].Z);
            zSum[i].x += layerPos_[i].RZ;
            zSum[i].y += layerPos_[i].Z;
            zSum[i].xx += dtf_t(layerPos_[i].RZ * layerPos_[i].RZ);
        }
    }

    sumData phiSums;

    for (i = 0; i < 7; i++) {
        phiSums.n += phiSum[i].n;
        phiSums.xy += phiSum[i].xy;
        phiSums.x += phiSum[i].x;
        phiSums.y += phiSum[i].y;
        phiSums.xx += phiSum[i].xx;
    }

    sumData zSums;

    for (i = 0; i < 7; i++) {
        zSums.n += zSum[i].n;
        zSums.xy += zSum[i].xy;
        zSums.x += zSum[i].x;
        zSums.y += zSum[i].y;
        zSums.xx += zSum[i].xx;
    }

    dtf_t temp1 = dtf_t(phiSums.n * phiSums.xy - phiSums.x * phiSums.y);
    dtf_t temp2 = dtf_t(phiSums.y * phiSums.xx - phiSums.x * phiSums.xy);
    dtf_t temp3 = dtf_t(phiSums.n * phiSums.xx - phiSums.x * phiSums.x);

    pair_t<dtf_t, dtf_t> phiParameter;
    phiParameter.first = dtf_t(temp1 / temp3);
    phiParameter.second = dtf_t(temp2 / temp3);

    dtf_t temp4 = dtf_t(zSums.n * zSums.xy - zSums.x * zSums.y);
    dtf_t temp5 = dtf_t(zSums.y * zSums.xx - zSums.x * zSums.xy);
    dtf_t temp6 = dtf_t(zSums.n * zSums.xx - zSums.x * zSums.x);

    pair_t<dtf_t, dtf_t> zParameter;
    zParameter.first = dtf_t(temp4 / temp6);
    zParameter.second = dtf_t(temp5 / temp6);

    LRParameter_.qOverPt = phiParameter.first;
    LRParameter_.phiT = phiParameter.second;
    LRParameter_.cotTheta = zParameter.first;
    LRParameter_.zT = zParameter.second;

}

void LRHLS_v1::calcResidual() {

    uint4_t i, j;

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

    for (i = 0; i < 12; i++) {
        residuals_[i].phi = 0;
        residuals_[i].z = 0;
        residuals_[i].stubId = 0;
        residuals_[i].layerId = 0;
        residuals_[i].valid = false;
    }

    for (i = 0; i < 12; i++) {
        StubHLS stub = stubs_[i];

        if (stub.valid()) {
            residData residual;

            for (j = 0; j < 7; j++) {
                if (stub.layerId() == j) {

                    residual.phi = abs_t(stub.phi() - phiResid[j]);
                    residual.z = abs_t(stub.z() - zResid[j]);
                    residual.stubId = i;
                    residual.layerId = j;
                    residual.valid = true;
                    residuals_[i] = residual;
                }
            }
        }
    }

}

bool LRHLS_v1::killLargestResid() {

    uint4_t i, j;
    uint4_t k = 0;

    findLargestResid();

    bool stubCritical = false;
    bool layerCritical = false;
    bool single = true;

    if (nStubs_ <= 4)
        stubCritical = true;

    if(nLayers_ == 2)
        layerCritical = true;

    for(i = 0; i < 7; i++)
        if(layerPopulation_[i] > 1)
            single = false;

    if(stubCritical && layerCritical && single)
        return true;

    stubs_[largestResid_.stubId].valid_ = false;
    layerPopulation_[largestResid_.layerId]--;

    if(layerPopulation_[largestResid_.layerId] == 0)
        nLayers_--;

    nStubs_--;

    return false;
}

void LRHLS_v1::findLargestResid() {

    uint4_t i, j;

    largestResid_.phi = -1.;
    largestResid_.z = -1.;
    largestResid_.stubId = 0;
    largestResid_.layerId = 0;
    largestResid_.valid = false;

    for (i = 0; i < 12; i++) {
        residData residual = residuals_[i];

        if (residual.valid) {
            if (residual.combined() > largestResid_.combined()) {
                largestResid_ = residual;
            }
        }
    }

}

void LRHLS_v1::createTrack() {


    trackOut_->qOverPt_ = LRParameter_.qOverPt;
    trackOut_->phi_ = LRParameter_.phiT;
    trackOut_->cot_ = LRParameter_.cotTheta;
    trackOut_->z_ = LRParameter_.zT;

    trackOut_->stubs_[0] = stubs_[0];
    trackOut_->stubs_[1] = stubs_[1];
    trackOut_->stubs_[2] = stubs_[2];
    trackOut_->stubs_[3] = stubs_[3];
    trackOut_->stubs_[4] = stubs_[4];
    trackOut_->stubs_[5] = stubs_[5];
    trackOut_->stubs_[6] = stubs_[6];
    trackOut_->stubs_[7] = stubs_[7];
    trackOut_->stubs_[8] = stubs_[8];
    trackOut_->stubs_[9] = stubs_[9];
    trackOut_->stubs_[10] = stubs_[10];
    trackOut_->stubs_[11] = stubs_[11];

#ifndef __SYNTHESIS__
    //std::cout << "track accepted" << std::endl;
#endif

}

#ifdef CMSSW_GIT_HASH
}

}
#endif
