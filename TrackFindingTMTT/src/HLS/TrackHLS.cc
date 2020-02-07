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

TrackHLS::TrackHLS() : size_(0), qOverPt_(0), phi_(0), cot_(0), z_(0), valid_(false) {}

//const array_s<Stub> &Track::stubs() const {
//    return stubs_;
//}

uint4_t TrackHLS::size() const {
    return size_;
}

const StubHLS *TrackHLS::stubs() const {
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

bool TrackHLS::valid() const {
    return valid_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
