/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_H__
#define __LRHLS_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_top.h"
#else
#include "Settings.h"
#include "Data.h"
#include "Track.h"
#include "Stub.h"
#include "LRHLS_top.h"
#endif

namespace TMTT {

class LRHLS {

public:

    LRHLS(Settings *settings, Data *data);

    ~LRHLS() {}

    void produce();

private:

    Settings *settings_;
    Data *data_;

};

}

#endif
