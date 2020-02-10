/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_TOP_H__
#define __LRHLS_TOP_H__


#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/DataHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/SettingsHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v0.h"
#else
#include "LRHLS_types.h"
#include "DataHLS.h"
#include "SettingsHLS.h"
#include "TrackHLS.h"
#include "StubHLS.h"
#include "LRHLS_v0.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

void LRHLS_top(const SettingsHLS *settingsHLS, DataHLS *dataHLS);

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
