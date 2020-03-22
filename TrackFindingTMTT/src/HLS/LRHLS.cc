/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"
#else
#include "LRHLS.h"
#endif

namespace TMTT {

LRHLS::LRHLS(Settings *settings, Data *data) : settings_(settings), data_(data) {

}

void LRHLS::produce() {

    Tracks *temp;

    for (auto trackMHT : data_->tracksMHT()) {

        auto *trackLR = new Track();

        for (auto stubMHT : trackMHT->stubs()) {

            TMTT::HLS::StubHLS stubIn = TMTT::HLS::StubHLS();
            TMTT::HLS::StubHLS stubOut = TMTT::HLS::StubHLS();

            stubIn.r_ = stubMHT->r();
            stubIn.phi_ = stubMHT->phi();
            stubIn.z_ = stubMHT->z();
            stubIn.layerId_ = stubMHT->layerId();
            stubIn.valid_ = stubMHT->valid();

            TMTT::HLS::LRHLS_top(stubIn, stubOut);

            if(stubOut.valid()) {
                Stub *stubLR = new Stub();
                stubLR->r_ = stubOut.r();
                stubLR->phi_ = stubOut.phi();
                stubLR->z_ = stubOut.z();
                stubLR->layerId_ = stubOut.layerId();
                stubLR->valid_ = stubOut.valid();

                trackLR->stubs_.push_back(stubLR);
            }
        }
        if(trackLR->size() > 5)
            data_->tracksLRHLS().push_back(trackLR);
    }
}

}