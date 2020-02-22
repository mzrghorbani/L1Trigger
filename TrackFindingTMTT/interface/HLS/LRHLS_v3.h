/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V3_H__
#define __LRHLS_V3_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/DataHLS.h"
#else
#include "LRHLS_types.h"
#include "DataHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class LRHLS_v3 {

public:


    LRHLS_v3(DataHLS *dataHLS);

    ~LRHLS_v3(){}

    void produce();
    void initFit();
    bool checkValidity();
    void create();
    void calcHelix();
    void calcResid();
    bool killLargestResid();
    void findLargestResid();


public:

    DataHLS *data_;
    LRStub stubs_[10];
    LRTrack LRParameter_;
    stubData layerPos_[7];
    residData residuals_[10];
    residData largestResid_;
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
