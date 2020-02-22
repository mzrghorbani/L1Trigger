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

const StubHLS *DataHLS::stubsMHT() const {
    return stubsMHT_;
}

const StubHLS *DataHLS::stubsLR() const {
    return stubsLR_;
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

