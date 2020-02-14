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

    const StubHLS *stubs() const;
    dtf_t qOverPt() const;
    dtf_t phi() const;
    dtf_t cot() const;
    dtf_t z() const;
    uint1_t valid() const;
    uint4_t secPhi() const;
    uint4_t secEta() const;

public:

StubHLS stubs_[12];
dtf_t qOverPt_;
dtf_t phi_;
dtf_t cot_;
dtf_t z_;
uint4_t secPhi_;
uint4_t secEta_;
uint1_t valid_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif