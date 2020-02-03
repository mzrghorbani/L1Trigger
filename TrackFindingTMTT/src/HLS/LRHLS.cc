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

        trackMHTHLS.qOverPt_ = trackMHT->qOverPt();
        trackMHTHLS.phi_ = trackMHT->phi();
        trackMHTHLS.cot_ = trackMHT->cot();
        trackMHTHLS.z_ = trackMHT->z();
        trackMHTHLS.valid_ = trackMHT->valid();

        j = 0;
        for (auto stubMHT : trackMHT->stubs()) {

            trackMHTHLS.stubs_[j].r_ = stubMHT->r();
            trackMHTHLS.stubs_[j].phi_ = stubMHT->phi();
            trackMHTHLS.stubs_[j].z_ = stubMHT->z();
            trackMHTHLS.stubs_[j].layerId_ = stubMHT->layerId();
            trackMHTHLS.stubs_[j].valid_ = stubMHT->valid();
            j++;
        }

        std::cout << "track " << i;
        TMTT::HLS::LRHLS_top(&trackMHTHLS, &trackLRHLS);

        data_->tracksLRHLS().push_back(trackMHT);

        data_->tracksLRHLS_[i]->qOverPt_ = trackLRHLS.qOverPt();
        data_->tracksLRHLS_[i]->phi_ = trackLRHLS.phi();
        data_->tracksLRHLS_[i]->cot_ = trackLRHLS.cot();
        data_->tracksLRHLS_[i]->z_ = trackLRHLS.z();
        data_->tracksLRHLS_[i]->valid_ = trackLRHLS.valid();

        j = 0;
        for(auto stubLRHLS : data_->tracksLRHLS()[i]->stubs()) {

            if(trackLRHLS.stubs_[j].valid()) {

                stubLRHLS->r_ = trackLRHLS.stubs_[j].r();
                stubLRHLS->phi_ = trackLRHLS.stubs_[j].phi();
                stubLRHLS->z_ = trackLRHLS.stubs_[j].z();
                stubLRHLS->module_->layerId_ = trackLRHLS.stubs_[j].layerId();
                stubLRHLS->valid_ = trackLRHLS.stubs_[j].valid();
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

}