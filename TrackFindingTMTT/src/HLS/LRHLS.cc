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

            TMTT::HLS::StubHLS dataIn[STUBS];
            TMTT::HLS::StubHLS dataOut[STUBS];

            for(i = 0; i < STUBS; i++) {
                dataIn[i].r = 0;
                dataIn[i].phi = 0;
                dataIn[i].z = 0;
                dataIn[i].layerId = 0;
                dataIn[i].valid = 0;
                dataOut[i].r = 0;
                dataOut[i].phi = 0;
                dataOut[i].z = 0;
                dataOut[i].layerId = 0;
                dataOut[i].valid = 0;
            }

            auto *trackLR = new Track();

            i = 0;
            for (auto stubMHT : trackMHT->stubs()) {

                if(stubMHT->valid()) {

                    dataIn[i].r = ap_int<13+6>(TMTT::HLS::dtf_t(stubMHT->r()) << 6);
                    dataIn[i].phi = ap_int<14+6>(TMTT::HLS::dtf_t(stubMHT->phi()) << 6);
                    dataIn[i].z = ap_int<14+6>(TMTT::HLS::dtf_t(stubMHT->z()) << 6);
                    dataIn[i].layerId = TMTT::HLS::uint3_t(stubMHT->layerId());
                    dataIn[i].valid = TMTT::HLS::uint1_t(stubMHT->valid());
                    i++;
                }
            }


            TMTT::HLS::LRHLS_top(dataIn, dataOut);

            k = 0;
            for (j = 0; j < STUBS; j++) {

                if(dataOut[j].valid == 1) {

                    k++;
                    Stub *stubLR = new Stub();

                    stubLR->r_ = double(ap_int<13+6>(dataOut[j].r >> 6));
                    stubLR->phi_ = double(ap_int<13+6>(dataOut[j].phi >> 6));
                    stubLR->z_ = double(ap_int<13+6>(dataOut[j].z >> 6));
                    stubLR->layerId_ = int(dataOut[j].layerId);
                    stubLR->valid_ = bool(dataOut[j].valid);

                    trackLR->stubs_.push_back(stubLR);
                }
            }
            
            if(k >= 2)
                data_->tracksLRHLS_.push_back(trackLR);
        }
    }
}
