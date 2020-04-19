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

void LRHLS_top(const int13_t* r, const int14_t* phi, const int14_t* z, const uint3_t* layerId, uint1_t* valid) {
//#pragma HLS PIPELINE II=10

	LRHLS_v5<WIN_LEN, LAYERS, LIMIT> lrhlsV5;

	lrhlsV5.produce(r, phi, z, layerId, valid);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
