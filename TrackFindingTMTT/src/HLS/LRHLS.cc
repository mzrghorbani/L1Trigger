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

    i = 0;

    for(auto trackLRHLS : dataHLS->tracksLRHLS()) {
        
        if(trackLRHLS.validHLS()) {

            Track *trackLR = new Track();
            trackLR->qOverPt_ = trackLRHLS.qOverPtHLS();
            trackLR->phi_ = trackLRHLS.phiHLS();
            trackLR->cot_ = trackLRHLS.cotHLS();
            trackLR->z_ = trackLRHLS.zHLS();
            trackLR->valid_ = trackLRHLS.validHLS();
            data_->tracksLRHLS_.push_back(trackLR);

            for(auto stubLRHLS : trackLRHLS.stubsHLS()) {

                if(stubLRHLS.validHLS()) {

                    Stub *stubLR = new Stub();
                    stubLR->r_ = stubLRHLS.rHLS();
                    stubLR->phi_ = stubLRHLS.phiHLS();
                    stubLR->z_ = stubLRHLS.zHLS();
                    stubLR->valid_ = stubLRHLS.validHLS();
                    data_->tracksLRHLS_[i]->stubs_.push_back(stubLR);
                }
            }
            i++;
        }
    }

}
