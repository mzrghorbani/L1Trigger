/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V0_H__
#define __LRHLS_V0_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/DataHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/SettingsHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "DataHLS.h"
#include "SettingsHLS.h"
#include "TrackHLS.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif



class LRHLS_v0 {

public:


    LRHLS_v0(const SettingsHLS *settingsHLS, DataHLS *dataHLS);

    ~LRHLS_v0(){}

    void produce();
    void initFit();
    void create();
    void calcHelix();
    void calcResidual();
    bool killLargestResidual();
    void findLargestResidual();
    bool checkValidity(StubHLS stubs[12]) const;
    uint4_t countStubs(StubHLS stubs[12], const bool& onlyPS = false) const;
    uint3_t countLayers(StubHLS stubs[12], const bool& onlySeed = false) const;


public:

    const SettingsHLS *settingsHLS_;
    DataHLS *dataHLS_;
    const uint3_t numLayers_;
    const uint3_t minLayers_;
    const uint3_t minLayersPS_;
    const dtf_t residPhi_;
    const dtf_t residZ2S_;
    const dtf_t residZPS_;
    const uint4_t maxIteartions_;

    TrackHLS track_;
    uint4_t secPhi_;
    uint4_t secEta_;
    LRTrack HTParameter_;
    LRTrack LRParameter_;
    StubHLS stubs_[12];
    StubHLS stubMap_[7][4];
    residData residuals_[7][4];
    uint3_t layerPopulation_[7];
    residData largestResid_;
    uint4_t nIterations_;
    uint4_t nStubs_;
    bool valid_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
