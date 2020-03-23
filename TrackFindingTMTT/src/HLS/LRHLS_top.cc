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

void LRHLS_top(T& indata, T& outdata) {

	LRHLS_v5<T, WIN_LEN, LAYERS, LIMIT> lrhlsV5;

	lrhlsV5.produce(indata, outdata);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
