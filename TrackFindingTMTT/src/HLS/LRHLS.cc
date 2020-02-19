/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"

using namespace TMTT;
using namespace HLS;

LRHLS::LRHLS(Settings *settings, Data *data) : settings_(settings), data_(data) {

}

void LRHLS::produce() {

    data_->tracksLRHLS().clear();

    for (auto trackMHT : data_->tracksMHT()) {

        for (auto stubMHT : trackMHT->stubs()) {

          StubHLS *stubIn = new StubHLS();
          StubHLS *stubOut = new StubHLS();
          Stub *stub = new Stub();
          TrackLR *track = new Track();

          stubIn->r_ = stubMHT->r();
          stubIn->phi_ = stubMHT->phi();
          stubIn->z_ = stubMHT->z();
          stubIn->layerId_ = stubMHT->layerId();
          stubIn->valid_ = stubMHT->valid();

          TMTT::HLS::LRHLS_top(stubIn, stubOut);

          stub.r_ = stubOut->r();
          stub.r_ = stubOut->phi();
          stub.r_ = stubOut->z();
          stub.layerId_ = stubOut->layerId();
          stub.valid_ = stubOut->valid();

          trackLR->stubs_.push_back(stub);
      }
      data_->trackLRHLS_.push_back(trackLR);
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