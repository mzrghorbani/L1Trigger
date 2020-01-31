/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V0_H__
#define __LRHLS_V0_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/DataHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "DataHLS.h"
#include "TrackHLS.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif



class LRHLS_v0 {

public:


    LRHLS_v0(DataHLS *dataHLS);

    ~LRHLS_v0(){}

    void produce();
    void initFit();
    void create();
    bool checkValidity();
    void calcHelix();
    void calcResidual();
    bool killLargestResidual();
    void findLargestResidual();

private:

    DataHLS *dataHLS_;
    TrackHLS trackIn_;
    TrackHLS trackOut_;
//    array_s<LRStub> stubs_;
    array_s<residData> residuals_;
    LRTrack HTParameter_;
    LRTrack LRParameter_;
    uint4_t layerPopulation_[7]{};
    stubData layerPos_[7];
    residData largestResid_;
    uint4_t nLayers_;
    uint4_t nLayersPS_;
    uint4_t nStubs_;
    uint4_t nIterations_;
    uint4_t maxIterations_;
    bool valid_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
