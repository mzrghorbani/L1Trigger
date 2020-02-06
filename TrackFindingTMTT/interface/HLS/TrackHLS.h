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

//    const array_s<Stub> &stubs() const;
    const StubHLS *stubs() const;
    uint4_t size() const;
    dtf_t qOverPt() const;
    dtf_t phi() const;
    dtf_t cot() const;
    dtf_t z() const;
    bool valid() const;

public:

//array_s<Stub> stubs_;
StubHLS stubs_[12];
uint4_t size_;
dtf_t qOverPt_;
dtf_t phi_;
dtf_t cot_;
dtf_t z_;
bool valid_;

};

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
