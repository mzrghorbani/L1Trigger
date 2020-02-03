/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface//StubHLS.h"
#else
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace  {
#endif


StubHLS::StubHLS() : r_(0), phi_(0), z_(0), layerId_(0), valid_(false) {

}

int13_t StubHLS::r() const {
    return r_;
}

int14_t StubHLS::phi() const {
    return phi_;
}

int14_t StubHLS::z() const {
    return z_;
}

uint3_t StubHLS::layerId() const {
    return layerId_;
}

bool StubHLS::valid() const {
    return valid_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
