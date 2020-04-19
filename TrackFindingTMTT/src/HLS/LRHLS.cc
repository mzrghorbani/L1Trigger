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

        TMTT::HLS::int13_t r[WIN_LEN];
        TMTT::HLS::int14_t phi[WIN_LEN];
        TMTT:HLS::int14_t z[WIN_LEN];
        TMTT::HLS::uint3_t layerId[WIN_LEN];
        TMTT::HLS::uint1_t valid[WIN_LEN];

        for (auto trackMHT : data_->tracksMHT()) {

            auto *trackLR = new Track();

            TMTT::HLS::StubHLS stubIn[WIN_LEN];
            TMTT::HLS::StubHLS stubOut[WIN_LEN];

            for(i = 0; i < WIN_LEN; i++) {
                r[i] = 0;
                phi[i] = 0;
                z[i] = 0;
                layerId[i] = 0;
                valid[i] = 0;
	    }

            i = 0;
            for (auto stubMHT : trackMHT->stubs()) {

                r[i] = stubMHT->r();
                phi[i] = stubMHT->phi();
                z[i] = stubMHT->z();
                layerId[i] = stubMHT->layerId();
                valid[i] = stubMHT->valid();
                i++;
            }

            TMTT::HLS::LRHLS_top(r, phi, z, layerId, valid);

            k = 0;
            for (j = 0; j < WIN_LEN; j++) {
                if (valid[j] == 1) {
                    Stub *stubLR = new Stub();
                    stubLR->r_ = r[k];
                    stubLR->phi_ = phi[k];
                    stubLR->z_ = z[k];
                    stubLR->layerId_ = layerId[k];
                    stubLR->valid_ = true;
		    k++;

                    trackLR->stubs_.push_back(stubLR);
                }
            }

            data_->tracksLRHLS_.push_back(trackLR);
        }
    }
}
