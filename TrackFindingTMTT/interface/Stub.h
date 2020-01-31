#ifndef __STUB_H__
#define __STUB_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/Module.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <cmath>

using namespace std;

namespace TMTT {

class Settings;
class TP;
class Track;

class Stub {

public:

    Stub(const Settings *settings, Module *module, const TTStubRef &ttStubRef);
    ~Stub() {}

    void assoc(const TPs &assocTPs) { assocTPs_ = assocTPs; }
    const Module *module() const { return module_; }
    TTStubRef ttRef() const { return ttStubRef_; }
    double col() const { return col_; }
    double row() const { return row_; }
    double bend() const { return bend_; }
    double r() const { return r_; }
    double phi() const { return phi_; }
    double z() const { return z_; }
    bool valid() const { return valid_; }

    Range rangePhi() const { return rangePhi_; }
    Range rangeEta() const { return rangeEta_; }
    Range rangePt() const { return rangePt_; }

    bool passDTC() const { return passDTC_; }
    bool passFE() const { return passFE_; }

    TPs assocTPs() const { return assocTPs_; }
    int dtcId() const { return module_->dtcId(); }
    bool barrel() const { return module_->barrel(); }
    bool psModule() const { return module_->psModule(); }
    int layerId() const { return module_->layerId(); }
    int uniqueLayerId() const { return module_->uniqueLayerId(); }

    //int  uniqueLayerId() const { return module_->layerId(); }
    int moduleType() const { return module_->modType(); }
    bool match(TP *tp) const { return find(assocTPs_.begin(), assocTPs_.end(), tp) != assocTPs_.end(); }

    void dtcStreamPos(const int &id, const int &pos) {
        dtcStreamIds_.push_back(id);
        dtcStreamPos_[id] = pos;
    }

    void gpStreamPos(const int &id, const int &pos) {
        gpStreamIds_.push_back(id);
        gpStreamPos_[id] = pos;
    }

    void htCellPos(const int &id, const int &pos) {
        htCellIds_.push_back(id);
        htCellPos_[id] = pos;
    }

    Ints dtcStreamIds() const { return dtcStreamIds_; }
    Ints gpStreamIds() const { return gpStreamIds_; }
    Ints htCellIds() const { return htCellIds_; }

    int dtcStreamPos(const int &id) const { return dtcStreamPos_.at(id); }
    int gpStreamPos(const int &id) const { return gpStreamPos_.at(id); }
    int htCellPos(const int &id) const { return htCellPos_.at(id); }

public:

    double digi(const double &val, const double &base) const {
        return settings_->enableDigitize() ? (floor(val / base) + .5) * base : val;
    }

    const Settings *settings_;
    Module *module_;
    const TTStubRef ttStubRef_;

    double col_;
    double row_;
    double bend_;
    double r_;
    double phi_;
    double z_;
    bool valid_;

    Range rangePt_;
    Range rangePhi_;
    Range rangeEta_;

    bool passDTC_;
    bool passFE_;

    TPs assocTPs_;

    Ints dtcStreamIds_;
    Ints gpStreamIds_;
    Ints htCellIds_;

    Pos dtcStreamPos_;
    Pos gpStreamPos_;
    Pos htCellPos_;

};

}
#endif
