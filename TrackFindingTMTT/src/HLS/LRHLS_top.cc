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

void LRHLS_top(StubHLS* stubIn, StubHLS* stubOut) {
#pragma HLS DATA_PACK variable=stubOut
#pragma HLS DATA_PACK variable=stubIn
//#pragma HLS PIPELINE II=1

	LRHLS_v5<WIN_LEN, LAYERS, LIMIT> lrhlsV5;

	lrhlsV5.produce(stubIn, stubOut);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
