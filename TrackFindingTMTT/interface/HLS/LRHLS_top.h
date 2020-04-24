/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_TOP_H__
#define __LRHLS_TOP_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v6.h"
#else
#include "LRHLS_types.h"
#include "LRHLS_v6.h"
#endif

#define STUBS 10
#define LAYERS 7
#define LIMIT 4

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

void LRHLS_top(TrackHLS<STUBS> *trackIn, TrackHLS<STUBS> *trackOut);

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
