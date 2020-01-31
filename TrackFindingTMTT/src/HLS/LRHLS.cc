/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"

using namespace TMTT;
using namespace HLS;

LRHLS::LRHLS(Settings *settings, Data *data) : settings_(settings), data_(data) {

}

void LRHLS::produce() {

    int i = 0;
    int j = 0;

    for (auto trackMHT : data_->tracksMHT()) {

        TrackHLS trackMHTHLS;
        TrackHLS trackLRHLS;

        trackMHTHLS.qOverPtHLS_ = trackMHT->qOverPt();
        trackMHTHLS.phiHLS_ = trackMHT->phi();
        trackMHTHLS.cotHLS_ = trackMHT->cot();
        trackMHTHLS.zHLS_ = trackMHT->z();
        trackMHTHLS.validHLS_ = trackMHT->valid();

        j = 0;
        for (auto stubMHT : trackMHT->stubs()) {

            trackMHTHLS.stubsHLS_[j].rHLS_ = stubMHT->r();
            trackMHTHLS.stubsHLS_[j].phiHLS_ = stubMHT->phi();
            trackMHTHLS.stubsHLS_[j].zHLS_ = stubMHT->z();
            trackMHTHLS.stubsHLS_[j].layerIdHLS_ = stubMHT->layerId();
            trackMHTHLS.stubsHLS_[j].psModuleHLS_ = stubMHT->psModule();
            trackMHTHLS.stubsHLS_[j].barrelHLS_ = stubMHT->barrel();
            trackMHTHLS.stubsHLS_[j].validHLS_ = stubMHT->valid();
            j++;
        }

        std::cout << "track " << i << std::endl;
        TMTT::HLS::LRHLS_top(&trackMHTHLS, &trackLRHLS);

        data_->tracksLRHLS().push_back(trackMHT);

        data_->tracksLRHLS_[i]->qOverPt_ = trackLRHLS.qOverPtHLS();
        data_->tracksLRHLS_[i]->phi_ = trackLRHLS.phiHLS();
        data_->tracksLRHLS_[i]->cot_ = trackLRHLS.cotHLS();
        data_->tracksLRHLS_[i]->z_ = trackLRHLS.zHLS();
        data_->tracksLRHLS_[i]->valid_ = trackLRHLS.validHLS();

        j = 0;
        for(auto stubLRHLS : data_->tracksLRHLS()[i]->stubs()) {

            if(trackLRHLS.stubsHLS_[j].validHLS()) {

                stubLRHLS->r_ = trackLRHLS.stubsHLS_[j].rHLS();
                stubLRHLS->phi_ = trackLRHLS.stubsHLS_[j].phiHLS();
                stubLRHLS->z_ = trackLRHLS.stubsHLS_[j].zHLS();
                stubLRHLS->module_->layerId_ = trackLRHLS.stubsHLS_[j].layerIdHLS();
                stubLRHLS->module_->psModule_ = trackLRHLS.stubsHLS_[j].psModuleHLS();
                stubLRHLS->module_->barrel_ = trackLRHLS.stubsHLS_[j].barrelHLS();
                stubLRHLS->valid_ = trackLRHLS.stubsHLS_[j].validHLS();
                j++;
            } else {
                stubLRHLS->r_ = 0;
                stubLRHLS->phi_ = 0;
                stubLRHLS->z_ = 0;
                stubLRHLS->module_->layerId_ = 0;
                stubLRHLS->module_->psModule_ = false;
                stubLRHLS->module_->barrel_ = false;
                stubLRHLS->valid_ = false;
            }
        }
        i++;
    }

    for(auto track : data_->tracksLRHLS()) {

        cout << "qOverPt : " << track->qOverPt() << endl;
        cout << "phi : " << track->phi() << endl;
        cout << "cot : " << track->cot() << endl;
        cout << "z : " << track->z() << endl;

        for(auto stub : track->stubs()) {
            if(stub->valid()) {
                cout << "stub layer id : " << stub->layerId() << endl;
            }

        }
    }

}
