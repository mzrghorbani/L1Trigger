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

data_t LRHLS_top(data_t data) {

	static LRHLS_v7<STUBS, LAYERS, LIMIT> lrhlsV7;

	return lrhlsV7(data);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
