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

void LRHLS_top(int13_t r[10], int14_t phi[10], int14_t z[10], uint3_t layerId[10], bool valid[10]) {
//#pragma HLS PIPELINE II=10

	LRHLS_v5<T, WIN_LEN, LAYERS, LIMIT> lrhlsV5;
#pragma HLS ARRAY_PARTITION variable=lrhlsV5.population_ complete dim=1

	lrhlsV5.produce(r, phi, z, layerId, valid);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
