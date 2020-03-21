/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V3_H__
#define __LRHLS_V3_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class LRHLS_v3 {

public:

    LRHLS_v3();

    void produce(StubHLS &stubIn, StubHLS &stubOut);

    ~LRHLS_v3(){}

public:

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
