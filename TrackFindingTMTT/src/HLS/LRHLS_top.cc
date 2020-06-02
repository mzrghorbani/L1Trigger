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

	data_t stubsIn[STUBS];
	data_t stubsOut[STUBS];

	for(int i=0; i<STUBS; i++) {
		stubsIn[i] = dataIn[i];
	}


	// for (int j = 0; j < STUBS; j++) {
	// 	std::cout << stubsIn[j].layerId << ", ";
	// }
	// std::cout << std::endl;

	LRHLS_v8<STUBS, LAYERS, LIMIT> lrhlsV8(stubsIn, stubsOut);

	lrhlsV8.produce();

	for(int i=0; i<STUBS; i++) {
		dataOut[i] = stubsOut[i];
	}

}

#ifdef CMSSW_GIT_HASH
}

}
#endif
