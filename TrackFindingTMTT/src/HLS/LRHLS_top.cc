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

void LRHLS_top(StubHLS &dataIn, StubHLS &dataOut) {
#pragma HLS PIPELINE II=1

	static StubHLS stubs[STUBS];

	static uint4_t nStubs = 0;

	for(int i=STUBS-1; i>0; i--) {
		stubs[i] = stubs[i-1];
	}
	stubs[0] = dataIn;

	StubHLS stubsIn[STUBS];

	StubHLS stubsOut[STUBS];

	if(dataIn.valid) {
		nStubs++;

		if(nStubs == STUBS) {
			for(int i=STUBS-1; i>=0; i--) {
				stubsIn[i] = stubs[i];
			}

			LRHLS_v7<STUBS, LAYERS, LIMIT> lrhlsV7(stubsIn, stubsOut);
			lrhlsV7.produce();

			for(int i=STUBS-1; i>=0; i--) {
				stubs[i] = stubsOut[i];
			}
			nStubs = 0;
		}
	}

	dataOut = stubs[STUBS-1];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
