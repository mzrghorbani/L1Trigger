/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_top.h"
#else
#include "LRHLS_top.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

void LRHLS_top(TrackHLS *trackIn, TrackHLS *trackOut) {

    LRHLS_v1 lrhlsV1(trackIn, trackOut);

    lrhlsV1.produce();

}

#ifdef CMSSW_GIT_HASH
}

}
#endif
