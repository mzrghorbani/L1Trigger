/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __STUBHLS_H__
#define __STUBHLS_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#else
#include "LRHLS_types.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class StubHLS {

public:

    StubHLS();

    ~StubHLS(){}

    int13_t r() const;
    int14_t phi() const;
    int14_t z() const;
    uint3_t layerId() const;
    uint1_t psModule() const;
    uint1_t barrel() const;
    uint1_t valid() const;

public:

    int13_t r_;
    int14_t phi_;
    int14_t z_;
    uint3_t layerId_;
    uint1_t psModule_;
    uint1_t barrel_;
    uint1_t valid_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
