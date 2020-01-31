/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __TRACKHLS_H__
#define __TRACKHLS_H__


#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

class TrackHLS {

public:

    TrackHLS();

    ~TrackHLS() {}

    const array_s<StubHLS> &stubsHLS() const;
//    const StubHLS *stubsHLS() const;
    dtf_t qOverPtHLS() const;
    dtf_t phiHLS() const;
    dtf_t cotHLS() const;
    dtf_t zHLS() const;
    uint1_t validHLS() const;

public:

array_s<StubHLS> stubsHLS_;
//StubHLS stubsHLS_[12];
dtf_t qOverPtHLS_;
dtf_t phiHLS_;
dtf_t cotHLS_;
dtf_t zHLS_;
uint1_t validHLS_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
