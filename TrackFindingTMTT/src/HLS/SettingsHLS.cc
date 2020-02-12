/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/SettingsHLS.h"
#else
#include "SettingsHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

SettingsHLS::SettingsHLS() :
		lrResidPhi_(0), lrResidZ2S_(0), lrResidZPS_(0), lrMinLayers_(0), lrMinLayersPS_(0), lrNumIterations_(0), trackerNumLayers_(0) {}

dtf_t SettingsHLS::lrResidPhi() const {
    return lrResidPhi_;
}

dtf_t SettingsHLS::lrResidZ2S() const {
    return lrResidZ2S_;
}

dtf_t SettingsHLS::lrResidZPS() const {
    return lrResidZPS_;
}

uint3_t SettingsHLS::lrMinLayers() const {
    return lrMinLayers_;
}

uint3_t SettingsHLS::lrMinLayersPS() const {
    return lrMinLayersPS_;
}

uint4_t SettingsHLS::lrNumIterations() const {
    return lrNumIterations_;
}

uint3_t SettingsHLS::trackerNumLayers() const {
    return trackerNumLayers_;
}


#ifdef CMSSW_GIT_HASH
}

}
#endif
