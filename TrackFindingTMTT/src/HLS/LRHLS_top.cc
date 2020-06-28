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

void LRHLS_top(hls::stream<word_t> &dataIn, hls::stream<word_t> &dataOut) {

	int i = 0;
	int j = 0;

	word_t tmp = 0;

	data_t stubsIn[STUBS];
	data_t stubsOut[STUBS];

	LRHLS_top_label0: while(!dataIn.empty()) {

		tmp = dataIn.read();

		stubsIn[i].r = tmp.range(12, 0);
		stubsIn[i].phi = tmp.range(26, 13);
		stubsIn[i].z = tmp.range(40, 27);
		stubsIn[i].layerId = tmp.range(43, 41);
		stubsIn[i].valid = tmp.range(44, 44);
		stubsIn[i].padding = tmp.range(63, 45);

		if(i == 7) {

			LRHLS_v8<STUBS, LAYERS, LIMIT> lrhlsV8(stubsIn, stubsOut);
			lrhlsV8.produce();

			LRHLS_top_label1: for(j=0; j<=i; j++) {

				tmp.range(12, 0) = stubsOut[j].r;
				tmp.range(26, 13) = stubsOut[j].phi;
				tmp.range(40, 27) = stubsOut[j].z;
				tmp.range(43, 41) = stubsOut[j].layerId;
				tmp.range(44, 44) = stubsOut[j].valid;
				tmp.range(63, 45) = stubsOut[j].padding;

				dataOut.write(tmp);
			}
			i = 0;
		} else {
			++i;
		}
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
