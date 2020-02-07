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

    data_->tracksLRHLS().clear();

    for (auto trackMHT : data_->tracksMHT()) {

        TrackHLS trackMHTHLS;
        trackMHTHLS.size_ = 0;
        TrackHLS trackLRHLS;
        trackLRHLS.size_ = 0;

        trackMHTHLS.qOverPt_ = trackMHT->qOverPt();
        trackMHTHLS.phi_ = trackMHT->phi();
        trackMHTHLS.cot_ = trackMHT->cot();
        trackMHTHLS.z_ = trackMHT->z();
        trackMHTHLS.size_ = trackMHT->size();
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

        TMTT::HLS::LRHLS_top(&trackMHTHLS, &trackLRHLS);

        std::cout << "size :    " << trackLRHLS.size() << std::endl;

        if (trackLRHLS.size() > 6) {

            Track *trackLR = new Track();
            trackLR->qOverPt_ = trackLRHLS.qOverPt();
            trackLR->phi_ = trackLRHLS.phi();
            trackLR->cot_ = trackLRHLS.cot();
            trackLR->z_ = trackLRHLS.z();
            trackLR->valid_ = trackLRHLS.valid();
            data_->tracksLRHLS().push_back(trackLR);

            for (j = 0; j < trackLRHLS.size(); j++) {
                if (trackLRHLS.stubs_[j].valid()) {

                    Stub *stubLR = new Stub();
                    stubLR->r_ = trackLRHLS.stubs_[j].r();
                    stubLR->phi_ = trackLRHLS.stubs_[j].phi();
                    stubLR->z_ = trackLRHLS.stubs_[j].z();
                    stubLR->valid_ = trackLRHLS.stubs_[j].valid();
                    data_->tracksLRHLS_[i]->stubs_.push_back(stubLR);
                }
            }
            i++;
        }
    }

   // for (auto track : data_->tracksLRHLS()) {
   //     std::cout << "size :    " << track->size() << std::endl;
   //     std::cout << "qOverPt : " << track->qOverPt() << std::endl;
   //     std::cout << "phi :     " << track->phi() << std::endl;
   //     std::cout << "cot :     " << track->cot() << std::endl;
   //     std::cout << "z :       " << track->z() << std::endl;
   //     std::cout << "valid :   " << track->valid() << std::endl;
   //     std::cout << std::endl;

   //     for (auto stub : track->stubs()) {
   //         std::cout << "r :       " << stub->r_ << std::endl;
   //         std::cout << "phi :     " << stub->phi_ << std::endl;
   //         std::cout << "z :       " << stub->z_ << std::endl;
   //         std::cout << "valid :   " << stub->valid_ << std::endl;
   //         std::cout << std::endl;
   //     }
   //     std::cout << std::endl;
   // }

}