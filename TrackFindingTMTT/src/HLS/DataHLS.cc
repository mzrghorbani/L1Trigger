/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/DataHLS.h"
#else
#include "DataHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

DataHLS::DataHLS() {

}

const array_t<TrackHLS> &DataHLS::tracksMHTHLS() const {
    return tracksMHTHLS_;
}

const array_t<TrackHLS> &DataHLS::tracksLRHLS() const {
    return tracksLRHLS_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif


