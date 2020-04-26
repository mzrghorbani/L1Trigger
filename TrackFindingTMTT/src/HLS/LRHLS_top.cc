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

void LRHLS_top(TrackHLS<STUBS> &trackIn, TrackHLS<STUBS> &trackOut) {

	LRHLS_v6<STUBS, LAYERS, LIMIT> lrhlsV6(trackIn, trackOut);
#pragma HLS ARRAY_PARTITION variable=lrhlsV6.stubs_ complete dim=1
#pragma HLS ARRAY_PARTITION variable=lrhlsV6.population_ complete dim=1

	lrhlsV6.produce();
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
