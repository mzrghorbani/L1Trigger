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

void LRHLS_top(data_t &dataIn, data_t &dataOut) {

	int i, j;

	if(dataIn.range(TRACKWORD-1,TRACKWORD-1) == 1) {

		TrackHLS<STUBS> trackIn;
		TrackHLS<STUBS> trackOut;

		j = 0;
		for(i = 0; i < (STUBS*STUBWORD); i=i+STUBWORD) {

			trackIn.stubs[j].r = dataIn.range(12+i,0+i);
			trackIn.stubs[j].phi = dataIn.range(26+i,13+i);
			trackIn.stubs[j].z = dataIn.range(40+i,27+i);
			trackIn.stubs[j].layerId = dataIn.range(43+i,41+i);
			trackIn.stubs[j].valid = dataIn.range(44+i,44+i);
			j++;
		}
		trackIn.valid = dataIn.range(TRACKWORD-1,TRACKWORD-1);

		LRHLS_v6<STUBS, LAYERS, LIMIT> lrhlsV6(trackIn, trackOut);

		lrhlsV6.produce();

		j = 0;
		for(i = 0; i < (STUBS*STUBWORD); i=i+STUBWORD) {

			dataOut.range(12+i,0+i) = trackOut.stubs[j].r;
			dataOut.range(26+i,13+i) = trackOut.stubs[j].phi;
			dataOut.range(40+i,27+i) = trackOut.stubs[j].z;
			dataOut.range(43+i,41+i) = trackOut.stubs[j].layerId;
			dataOut.range(44+i,44+i) = trackOut.stubs[j].valid;
			j++;
		}
		dataOut.range(TRACKWORD-1,TRACKWORD-1) = trackOut.valid;
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
