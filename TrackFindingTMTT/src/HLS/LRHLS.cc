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

            ap_uint<STUBS*45> dataIn = 0;
            ap_uint<STUBS*45> dataOut = 0;

            i = 0;
            for (auto stubMHT : trackMHT->stubs()) {

                dataIn.range(12+i,0+i) = ap_fixed<32,13>(stubMHT->r()) << 5;
                dataIn.range(26+i,13+i) = ap_fixed<32,14>(stubMHT->phi()) << 9;
                dataIn.range(40+i,27+i) = ap_fixed<32,14>(stubMHT->z()) << 4;
                dataIn.range(43+i,41+i) = ap_uint<3>(stubMHT->layerId());
                dataIn.range(44+i,44+i) = ap_uint<1>(stubMHT->valid());
                i+=45;
            }

            TMTT::HLS::LRHLS_top(dataIn, dataOut);
            
            auto *trackLR = new Track();

            k = 0;
            for (i = 0; i < STUBS*45; i=i+45) {
                if(ap_uint<1>(dataOut.range(44+i,44+i)) == 1) {

                    k+=1;
                    Stub *stubLR = new Stub();

                    stubLR->r_ = ap_fixed<32,13>(dataOut.range(12+i,0+i)) >> 5; 
                    stubLR->phi_ = ap_fixed<32,14>(dataOut.range(26+i,13+i)) >> 9;
                    stubLR->z_ = ap_fixed<32,14>(dataOut.range(40+i,27+i)) >> 4;
                    stubLR->layerId_ = ap_uint<3>(dataOut.range(43+i,41+i));
                    stubLR->valid_ = ap_uint<1>(dataOut.range(44+i,44+i));

                    trackLR->stubs_.push_back(stubLR);
                }
            }
            if(k > 1) {
                trackLR->valid_ = true;
                trackLR->settings_ = trackMHT->settings_;
                trackLR->tps_ = trackMHT->tps();
                trackLR->region_ = trackMHT->region();
                trackLR->parent_ = trackMHT->parent();
                trackLR->streamId_ = trackMHT->streamId();
                trackLR->streamPos_ = trackMHT->streamPos();
                trackLR->cellId_ = trackMHT->cellId();
                trackLR->secEta_ = trackMHT->secEta();
                trackLR->secPhi_ = trackMHT->secPhi();
                trackLR->qOverPt_ = trackMHT->qOverPt();
                trackLR->phi_ = trackMHT->phi();
                trackLR->cot_ = trackMHT->cot();
                trackLR->z_ = trackMHT->z();
                trackLR->chi2_ = trackMHT->chi2();
                trackLR->binPhi_ = trackMHT->binPhi();
                trackLR->binPt_ = trackMHT->binPt();

                data_->tracksLRHLS_.push_back(trackLR);
            }
        }

    }

}
