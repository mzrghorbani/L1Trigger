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

void LRHLS_top(const data_t stubIn[STUBS], data_t stubOut[STUBS]) {

	LRHLS_v10<STUBS, LAYERS, LIMIT> lrhlsV10(stubIn, stubOut);

	lrhlsV10.produce();

}

#ifdef CMSSW_GIT_HASH
}

}
#endif
