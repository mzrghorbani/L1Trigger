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

TrackHLS::TrackHLS() : qOverPt_(0), phi_(0), cot_(0), z_(0), valid_(false) {}

const array_s<StubHLS> &TrackHLS::stubs() const {
    return stubs_;
}

dtf_t TrackHLS::qOverPt() const {
    return qOverPt_;
}

dtf_t TrackHLS::phi() const {
    return phi_;
}

dtf_t TrackHLS::cot() const {
    return cot_;
}

dtf_t TrackHLS::z() const {
    return z_;
}

uint1_t TrackHLS::valid() const {
    return valid_;
}

uint4_t TrackHLS::secPhi() const {
    return secPhi_;
}

uint4_t TrackHLS::secEta() const {
    return secEta_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

