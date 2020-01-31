#ifndef __DTC_H__
#define __DTC_H__

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

class Settings;

/**
 * Class to assign stubs to DTCs
 */
class DTC {

public:

  DTC( const Settings* settings, Data* data );

  ~DTC(){};

  void produce() const;

private:

  bool inside( const int& x, const Range& r ) const { return r.second > r.first ? x >= r.first || x <= r.second : x <= r.second && x >= r.first; }

  const Settings* settings_;
        Data*     data_;
  const int       numRegions_;
  const int       numSectorsPhi_;
  const int       numDTCsPerRegion_;
  const int       numDTCsPerTFP_;
  const int       fracEta_;
  const int       numDTCsPerEtaRegion_;
  const int       fracPhi_;
  const int       num10GPSPerRegion_;
  const bool      orderStubsByBend_;

};

}

#endif