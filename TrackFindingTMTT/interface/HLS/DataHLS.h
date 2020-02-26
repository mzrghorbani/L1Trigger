/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __DATAHLS_H__
#define __DATAHLS_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/TrackHLS.h"
#else
#include "TrackHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class DataHLS {

public:

    DataHLS();

    ~DataHLS(){}

    const array_t<TrackHLS> &tracksMHTHLS() const;

    const array_t<TrackHLS> &tracksLRHLS() const;

public:

    array_t<TrackHLS> tracksMHTHLS_;
    array_t<TrackHLS> tracksLRHLS_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
