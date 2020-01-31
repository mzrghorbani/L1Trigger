/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#else
#include "TrackHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

TrackHLS::TrackHLS() : qOverPtHLS_(0), phiHLS_(0), cotHLS_(0), zHLS_(0), validHLS_(false) {}

//const array_s<StubHLS> &TrackHLS::stubsHLS() const {
//    return stubsHLS_;
//}

const StubHLS *TrackHLS::stubsHLS() const {
    return stubsHLS_;
}

dtf_t TrackHLS::qOverPtHLS() const {
    return qOverPtHLS_;
}

dtf_t TrackHLS::phiHLS() const {
    return phiHLS_;
}

dtf_t TrackHLS::cotHLS() const {
    return cotHLS_;
}

dtf_t TrackHLS::zHLS() const {
    return zHLS_;
}

bool TrackHLS::validHLS() const {
    return validHLS_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

