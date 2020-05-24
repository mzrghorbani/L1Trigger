/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_TOP_H__
#define __LRHLS_TOP_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v7.h"
#else
#include "LRHLS_types.h"
#include "LRHLS_v7.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

data_t LRHLS_top(data_t data);

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
