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
#pragma HLS PIPELINE II=1

	static LRHLS_v7<STUBS, LAYERS, LIMIT> lrhlsV7;
#pragma HLS ARRAY_PARTITION variable=lrhlsV7.residuals_ complete dim=1
#pragma HLS ARRAY_PARTITION variable=lrhlsV7.population_ complete dim=1
#pragma HLS ARRAY_PARTITION variable=lrhlsV7.stubs_ complete dim=1

	return lrhlsV7(data);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
