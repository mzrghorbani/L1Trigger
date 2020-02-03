/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"

using namespace TMTT;
using namespace HLS;

LRHLS::LRHLS(Settings *settings, Data *data) : settings_(settings), data_(data) {

}

void LRHLS::produce() {

    auto *dataHLS = new DataHLS();

    int i = 0;
    int j = 0;

    for (auto trackMHT : data_->tracksMHT()) {

        TrackHLS trackMHTHLS;
        trackMHTHLS.qOverPtHLS_ = trackMHT->qOverPt();
        trackMHTHLS.phiHLS_ = trackMHT->phi();
        trackMHTHLS.cotHLS_ = trackMHT->cot();
        trackMHTHLS.zHLS_ = trackMHT->z();
        trackMHTHLS.validHLS_ = trackMHT->valid();
        dataHLS->tracksMHTHLS_.push_back(trackMHTHLS);

        for (auto stubMHT : trackMHT->stubs()) {

            StubHLS stubMHTHLS;
            stubMHTHLS.rHLS_ = stubMHT->r();
            stubMHTHLS.phiHLS_ = stubMHT->phi();
            stubMHTHLS.zHLS_ = stubMHT->z();
            stubMHTHLS.layerIdHLS_ = stubMHT->layerId();
            stubMHTHLS.psModuleHLS_ = stubMHT->psModule();
            stubMHTHLS.barrelHLS_ = stubMHT->barrel();
            stubMHTHLS.validHLS_ = stubMHT->valid();
            dataHLS->tracksMHTHLS_[i].stubsHLS_.push_back(stubMHTHLS);
        }
        i++;
    }

    TMTT::HLS::LRHLS_top(dataHLS);

    for(auto trackMHT : data_->tracksMHT()) {
        data_->tracksLRHLS_.push_back(trackMHT);
    }


    i = 0;
    for(auto trackLR : data_->tracksLRHLS()) {
        trackLR->valid_ = dataHLS->tracksLRHLS()[i].validHLS();
        j = 0;

        for(auto stubLR : trackLR->stubs()) {
            stubLR->valid_ = dataHLS->tracksLRHLS()[i].stubsHLS()[j].validHLS();
            j++;
        }
        i++;
    }

}
