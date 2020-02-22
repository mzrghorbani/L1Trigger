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

      Track *trackLR = new Track();
      DataHLS *dataHLS = new DataHLS();
      int k = 0;

      for (auto stubMHT : trackMHT->stubs()) {

        StubHLS *stubIn = new StubHLS();
        Stub *stubOut = new Stub();

        stubIn->r_ = stubMHT->r();
        stubIn->phi_ = stubMHT->phi();
        stubIn->z_ = stubMHT->z();
        stubIn->layerId_ = stubMHT->layerId();
        stubIn->valid_ = stubMHT->valid();

        dataHLS->stubsMHT_[k] = *stubIn;
        k++;

        if(k == trackMHT->size())
          TMTT::HLS::LRHLS_top(dataHLS);
        else
          continue;

        int i;
        for(i = 0; i < 12; i++) {
          if(dataHLS->stubsLR_[i].valid()) {

            stubOut->r_ = dataHLS->stubsLR_[i].r();
            stubOut->phi_ = dataHLS->stubsLR_[i].phi();
            stubOut->z_ = dataHLS->stubsLR_[i].z();
            stubOut->layerId_ = dataHLS->stubsLR_[i].layerId();
            stubOut->valid_ = dataHLS->stubsLR_[i].valid();

            trackLR->stubs_.push_back(stubOut);
          }
        }
      }

      data_->tracksLRHLS_.push_back(trackLR);
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