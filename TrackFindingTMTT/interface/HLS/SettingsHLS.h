/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __SETTINGSHLS_H__
#define __SETTINGSHLS_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#else
#include "LRHLS_types.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class SettingsHLS {

public:

    SettingsHLS();

    ~SettingsHLS(){}

public:

    dtf_t lrResidPhi() const;
    dtf_t lrResidZ2S() const;
    dtf_t lrResidZPS() const;
    uint3_t lrMinLayers() const;
    uint3_t lrMinLayersPS() const;
    uint3_t lrNumIterations() const;
    uint3_t trackerNumLayers() const;

public:

    dtf_t lrResidPhi_;
    dtf_t lrResidZ2S_;
    dtf_t lrResidZPS_;
    uint3_t lrMinLayers_;
    uint3_t lrMinLayersPS_;
    uint3_t lrNumIterations_;
    uint3_t trackerNumLayers_;
};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
