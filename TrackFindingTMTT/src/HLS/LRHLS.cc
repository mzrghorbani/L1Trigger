/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"
#else
#include "LRHLS.h"
#endif

namespace TMTT {

    LRHLS::LRHLS(Settings *settings, Data *data) : settings_(settings), data_(data) {}

    void LRHLS::produce() {

        int i = 0;
        int j = 0;
        int k = 0;

        for (auto trackMHT : data_->tracksMHT()) {

            auto *trackLR = new Track();

            TMTT::HLS::StubHLS stubIn[WIN_LEN];
            TMTT::HLS::StubHLS stubOut[WIN_LEN];

            for(i = 0; i < WIN_LEN; i++) {
                stubIn[i].r_ = 0;
                stubIn[i].phi_ = 0;
                stubIn[i].z_ = 0;
                stubIn[i].layerId_ = 0;
                stubIn[i].valid_ = false;
                stubOut[i].r_ = 0;
                stubOut[i].phi_ = 0;
                stubOut[i].z_ = 0;
                stubOut[i].layerId_ = 0;
                stubOut[i].valid_ = false;
            }

            i = 0;
            for (auto stubMHT : trackMHT->stubs()) {

                stubIn[i].r_ = stubMHT->r();
                stubIn[i].phi_ = stubMHT->phi();
                stubIn[i].z_ = stubMHT->z();
                stubIn[i].layerId_ = stubMHT->layerId();
                stubIn[i].valid_ = stubMHT->valid();
                i++;
            }

            TMTT::HLS::LRHLS_top(stubIn, stubOut);

            k = 0;
            for (j = 0; j < WIN_LEN; j++) {
                if (stubOut[j].valid()) {
                    k++;
                    Stub *stubLR = new Stub();
                    stubLR->r_ = stubOut[j].r();
                    stubLR->phi_ = stubOut[j].phi();
                    stubLR->z_ = stubOut[j].z();
                    stubLR->layerId_ = stubOut[j].layerId();
                    stubLR->valid_ = stubOut[j].valid();

                    trackLR->stubs_.push_back(stubLR);
                }
            }

            if(k > 3) data_->tracksLRHLS_.push_back(trackLR);
        }
    }
}
