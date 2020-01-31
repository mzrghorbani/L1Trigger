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

    int13_t rHLS() const;
    int14_t phiHLS() const;
    int14_t zHLS() const;
    uint3_t layerIdHLS() const;
    uint1_t psModuleHLS() const;
    uint1_t barrelHLS() const;
    uint1_t validHLS() const;

public:

    int13_t rHLS_;
    int14_t phiHLS_;
    int14_t zHLS_;
    uint3_t layerIdHLS_;
    uint1_t psModuleHLS_;
    uint1_t barrelHLS_;
    uint1_t validHLS_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
