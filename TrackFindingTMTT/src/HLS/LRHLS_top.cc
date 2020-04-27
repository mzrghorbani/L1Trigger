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

void LRHLS_top(ap_uint<STUBS*45> &dataIn, ap_uint<STUBS*45> &dataOut) {
#pragma HLS PIPELINE II=450

	TrackHLS<STUBS> trackIn;
	TrackHLS<STUBS> trackOut;

	int j = 0;
	for(int i = 0; i < STUBS*45; i=i+45) {

		trackIn.stubs[j].r = dataIn.range(0+i, 12+i);
		trackIn.stubs[j].phi = dataIn.range(13+i, 26+i);
		trackIn.stubs[j].z = dataIn.range(27+i, 40+i);
		trackIn.stubs[j].layerId = dataIn.range(41+i, 43+i);
		trackIn.stubs[j].valid = dataIn.range(44+i, 44+i);
		j++;
	}

	LRHLS_v6<STUBS, LAYERS, LIMIT> lrhlsV6(trackIn, trackOut);
//#pragma HLS ARRAY_PARTITION variable=lrhlsV6.residuals_ complete dim=1
//#pragma HLS ARRAY_PARTITION variable=lrhlsV6.population_ complete dim=1
//#pragma HLS ARRAY_PARTITION variable=lrhlsV6.stubs_ complete dim=1

	lrhlsV6.produce();

	j = 0;
	for(int i = 0; i < STUBS*45; i=i+45) {

		dataOut.range(0+i, 12+i) = trackOut.stubs[j].r;
		dataOut.range(13+i, 26+i) = trackOut.stubs[j].phi;
		dataOut.range(27+i, 40+i) = trackOut.stubs[j].z;
		dataOut.range(41+i, 43+i) = trackOut.stubs[j].layerId;
		dataOut.range(44+i, 44+i) = trackOut.stubs[j].valid;
		j++;
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
