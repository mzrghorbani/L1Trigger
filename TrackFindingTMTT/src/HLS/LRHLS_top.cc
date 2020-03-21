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

void LRHLS_top(StubHLS indata[WIN_LEN], StubHLS outdata[WIN_LEN]) {
#pragma HLS INTERFACE ap_fifo port=outdata
#pragma HLS INTERFACE ap_fifo port=indata

	LRHLS_v4<StubHLS,WIN_LEN,LAYERS,LIMIT> lrhlsV4;

	lrhlsV4.produce(indata, outdata);
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
