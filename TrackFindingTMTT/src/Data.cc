#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/Data.h"
#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/TP.h"
#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/Module.h"
#include "/home/mghorbani/workspace/CMSSW_10_4_0/src/L1Trigger/TrackFindingTMTT/interface/Track.h"


namespace TMTT {

Data::Data(const Settings *settings) : settings_(settings) {

    modules_.reserve(13296);
    stubs_.reserve(20000);
    tps_.reserve(2500);
    tracksHT_.reserve(500);
    tracksMHT_.reserve(400);
    tracksLR_.reserve(300);
    tracksLRHLS_.reserve(300);
    tracksKF_.reserve(300);

}

Data::~Data() {

    for (auto &module : modules_)
        delete module.second;
    modules_.clear();
    clear();

}

void Data::clear() {

    for (auto &stub : stubs_)
        delete stub;
    stubs_.clear();
    for (auto &tp : tps_)
        delete tp;
    tps_.clear();
    for (auto &track : tracksHT_)
        delete track;
    tracksHT_.clear();
    for (auto &track : tracksMHT_)
        delete track;
    tracksMHT_.clear();
    for (auto &track : tracksLR_)
        delete track;
    tracksLR_.clear();
    for (auto &track : tracksLRHLS_)
        delete track;
    tracksLRHLS_.clear();
    for (auto &track : tracksKF_)
        delete track;
    tracksKF_.clear();

}

}