/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_TOP_H__
#define __LRHLS_TOP_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v5.h"
#else
#include "LRHLS_types.h"
#include "LRHLS_v5.h"
#endif

#define WIN_LEN 10
#define LAYERS 7
#define LIMIT 4

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

void LRHLS_top(StubHLS* stubIn, StubHLS* stubOut);

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
