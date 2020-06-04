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

void LRHLS_top(data_t dataIn[STUBS], data_t dataOut[STUBS]) {

	LRHLS_v9<STUBS, LAYERS, LIMIT> lrhlsV9(dataIn, dataOut);

	lrhlsV9.produce();

}

#ifdef CMSSW_GIT_HASH
}

}
#endif
