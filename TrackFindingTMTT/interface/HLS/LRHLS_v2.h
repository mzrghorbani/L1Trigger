/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V2_H__
#define __LRHLS_V2_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "TrackHLS.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class LRHLS_v2 {

public:


    LRHLS_v2(TrackHLS *trackIn, TrackHLS *trackOut);

    ~LRHLS_v2(){}

    void produce();
    void initFit();
    bool checkValidity();
    void calcHelix();
    void calcResidual();
    void findLeastResid();
    void keepLeastResid();
    void returnTrack();
    void createTrack();

public:

    TrackHLS *trackIn_;
    TrackHLS *trackOut_;
    LRTrack LRParameter_;
    LRStub stubs_[12];
    stubData layerPos_[7];
    residData residuals_[12];
    residData layerMinResid_[7];
    uint3_t layerPopulation_[7];
    uint3_t nLayers_;
    uint4_t nStubs_;
    bool valid_;


};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif