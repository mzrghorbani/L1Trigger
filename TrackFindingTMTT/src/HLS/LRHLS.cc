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

        int i, j, k;

        for (auto trackMHT : data_->tracksMHT()) {

            auto *trackLR = new Track();

            k = 0;
            for (auto stubMHT : trackMHT->stubs()) {

                auto *stubLR = new Stub();

                TMTT::HLS::StubHLS stubIn;
                TMTT::HLS::StubHLS stubOut;

                stubIn.r = ap_int<13+6>(TMTT::HLS::dtf_t(stubMHT->r()) << 6);
                stubIn.phi = ap_int<14+6>(TMTT::HLS::dtf_t(stubMHT->phi()) << 6);
                stubIn.z = ap_int<14+6>(TMTT::HLS::dtf_t(stubMHT->z()) << 6);
                stubIn.layerId = TMTT::HLS::uint3_t(stubMHT->layerId());
                stubIn.valid = TMTT::HLS::uint1_t(stubMHT->valid());
                stubOut = TMTT::HLS::LRHLS_top(stubIn);

                stubLR->r_ = double(ap_int<13+6>(stubOut.r >> 6));
                stubLR->phi_ = double(ap_int<13+6>(stubOut.phi >> 6));
                stubLR->z_ = double(ap_int<13+6>(stubOut.z >> 6));
                stubLR->layerId_ = int(stubOut.layerId);
                stubLR->valid_ = bool(stubOut.valid);

                if(stubLR->valid_) {
                    k++;
                    trackLR->stubs_.push_back(stubLR);
                }
            }

            if(k > 1)
                data_->tracksLRHLS_.push_back(trackLR);
            
        }
    }

}
