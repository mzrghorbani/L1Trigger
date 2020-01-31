/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif


StubHLS::StubHLS() : rHLS_(0), phiHLS_(0), zHLS_(0), layerIdHLS_(0), psModuleHLS_(false), barrelHLS_(false), validHLS_(false) {

}

int13_t StubHLS::rHLS() const {
    return rHLS_;
}

int14_t StubHLS::phiHLS() const {
    return phiHLS_;
}

int14_t StubHLS::zHLS() const {
    return zHLS_;
}

uint3_t StubHLS::layerIdHLS() const {
    return layerIdHLS_;
}

bool StubHLS::psModuleHLS() const {
    return psModuleHLS_;
}

bool StubHLS::barrelHLS() const {
    return barrelHLS_;
}

bool StubHLS::validHLS() const {
    return validHLS_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif