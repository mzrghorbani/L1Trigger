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

    for (auto trackMHT : data_->tracksMHT()) {

        auto *trackLR = new Track();

        for (auto stubMHT : trackMHT->stubs()) {

            auto *stubIn = new TMTT::HLS::StubHLS();
            auto *stubOut = new TMTT::HLS::StubHLS();

            stubIn->r_ = stubMHT->r();
            stubIn->phi_ = stubMHT->phi();
            stubIn->z_ = stubMHT->z();
            stubIn->layerId_ = stubMHT->layerId();
            stubIn->valid_ = stubMHT->valid();

            TMTT::HLS::LRHLS_top(stubIn, stubOut);

            Stub *stubLR = new Stub();
            stubLR->r_ = stubOut->r();
            stubLR->phi_ = stubOut->phi();
            stubLR->z_ = stubOut->z();
            stubLR->layerId_ = stubOut->layerId();
            stubLR->valid_ = stubOut->valid();

            trackLR->stubs_.push_back(stubLR);
        }
        data_->tracksLRHLS_.push_back(trackLR);
    }
}

   // for (auto track : data_->tracksLRHLS()) {
   //          std::cout << track->size() << std::endl;
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