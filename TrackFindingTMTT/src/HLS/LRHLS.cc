/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"

using namespace TMTT;
using namespace HLS;

LRHLS::LRHLS(const Settings *settings, Data *data) : settings_(settings), data_(data) {

}

void LRHLS::produce() {

    auto *dataHLS = new DataHLS();
    auto *settingsHLS = new SettingsHLS();

    int i = 0;
    int j = 0;

    settingsHLS->lrResidPhi_ = settings_->lrResidPhi();
    settingsHLS->lrResidZ2S_ = settings_->lrResidZ2S();
    settingsHLS->lrResidZPS_ = settings_->lrResidZPS();
    settingsHLS->lrMinLayers_ = settings_->lrMinLayers();
    settingsHLS->lrMinLayersPS_ = settings_->lrMinLayersPS();
    settingsHLS->lrNumIterations_ = settings_->lrNumIterations();
    settingsHLS->trackerNumLayers_ = settings_->trackerNumLayers();

    for (auto trackMHT : data_->tracksMHT()) {
        TrackHLS trackMHTHLS;
        trackMHTHLS.qOverPt_ = trackMHT->qOverPt();
        trackMHTHLS.phi_ = trackMHT->phi();
        trackMHTHLS.cot_ = trackMHT->cot();
        trackMHTHLS.z_ = trackMHT->z();
        trackMHTHLS.secPhi_ = trackMHT->secPhi();
        trackMHTHLS.secEta_ = trackMHT->secEta();
        trackMHTHLS.valid_ = trackMHT->valid();
        dataHLS->tracksMHTHLS_.push_back(trackMHTHLS);

        for (auto stubMHT : trackMHT->stubs()) {
            StubHLS stubMHTHLS;
            stubMHTHLS.r_ = stubMHT->r();
            stubMHTHLS.phi_ = stubMHT->phi();
            stubMHTHLS.z_ = stubMHT->z();
            stubMHTHLS.layerId_ = stubMHT->layerId();
            stubMHTHLS.psModule_ = stubMHT->psModule();
            stubMHTHLS.barrel_ = stubMHT->barrel();
            stubMHTHLS.valid_ = stubMHT->valid();
            dataHLS->tracksMHTHLS_[i].stubs_.push_back(stubMHTHLS);
        }
        i++;
    }

    TMTT::HLS::LRHLS_top(settingsHLS, dataHLS);

    i = 0;
    for(const auto& trackLRHLS : dataHLS->tracksLRHLS()) {
        auto *trackLR = new Track();
        trackLR->qOverPt_ = trackLRHLS.qOverPt();
        trackLR->phi_ = trackLRHLS.phi();
        trackLR->cot_ = trackLRHLS.cot();
        trackLR->z_ = trackLRHLS.z();
        trackLR->secPhi_ = trackLRHLS.secPhi();
        trackLR->secEta_ = trackLRHLS.secEta();
        trackLR->valid_ = trackLRHLS.valid();
        data_->tracksLRHLS_.push_back(trackLR);

        for(const auto& stubLRHLS : trackLRHLS.stubs()) {
            Stub *stubLR = new Stub();
            stubLR->r_ = stubLRHLS.r();
            stubLR->phi_ = stubLRHLS.phi();
            stubLR->z_ = stubLRHLS.z();
            stubLR->valid_ = stubLRHLS.valid();
            data_->tracksLRHLS_[i]->stubs_.push_back(stubLR);
        }
        i++;
    }
    
}
