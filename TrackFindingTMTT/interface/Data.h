#ifndef __DATA_H__
#define __DATA_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/SLV.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace TMTT {

    class Settings;
    class Module;
    class Stub;
    class TP;
    class Track;

    typedef unordered_map<uint32_t, Module *> Modules;
    typedef unordered_map<uint32_t, int> CablingMap;

    typedef vector<TP *> TPs;
    typedef vector<TPs> TPss;

    typedef vector<Stub *> Stubs;
    typedef vector<Stubs> Stubss;
    typedef vector<Stubss> Stubsss;

    typedef vector<Track *> Tracks;
    typedef vector<Tracks> Trackss;
    typedef vector<Trackss> Tracksss;

    typedef vector<string> Strings;
    typedef vector<Strings> Stringss;

    typedef pair<Track *, Stub *> TS;
    typedef vector<TS> TSs;
    typedef vector<TSs> TSss;
    static const TS nullTS(nullptr, nullptr);

    typedef vector<int> Ints;
    typedef vector<Ints> Intss;

    typedef vector<double> Doubles;

    typedef unordered_map<int, int> Pos;
    typedef pair<int, int> Range;

    typedef vector<SLV> SLVs;

/**
 * Class to store and provide Data (Stubs, Tracks, ...)
 */
class Data {

public:

    Data(const Settings *settings);

    ~Data();

    /*!
    delete last event data.
    */
    void clear();

    Modules &modules() { return modules_; }
    Stubs &stubs() { return stubs_; }
    TPs &tps() { return tps_; }
    Tracks &tracksHT() { return tracksHT_; }
    Tracks &tracksMHT() { return tracksMHT_; }
    Tracks &tracksLR() { return tracksLR_; }
    Tracks &tracksLRHLS() { return tracksLRHLS_; }
    Tracks &tracksKF() { return tracksKF_; }

public:

    const Settings *settings_;

    Modules modules_;
    TPs tps_;
    Stubs stubs_;
    Tracks tracksHT_;
    Tracks tracksMHT_;
    Tracks tracksLR_;
    Tracks tracksLRHLS_;
    Tracks tracksKF_;

};

}

#endif
