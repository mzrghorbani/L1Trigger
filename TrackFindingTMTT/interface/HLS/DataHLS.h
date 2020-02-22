/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __DATAHLS_H__
#define __DATAHLS_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class DataHLS {

public:

    DataHLS();

    ~DataHLS(){}

    const StubHLS *stubsMHT() const;
    const StubHLS *stubsLR() const;

public:

    StubHLS stubsMHT_[10];
    StubHLS stubsLR_[10];

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
