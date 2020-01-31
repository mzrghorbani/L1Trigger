#ifndef __HT_H__
#define __HT_H__

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <vector>
#include <cmath>


using namespace std;

namespace TMTT {

class Settings;
class Stub;

/**
 * Class to assign Stubs to r-phi track candidates
 */
class HT {

public:

  HT( const Settings* settings, Data* data );

  ~HT() {}

  void produce();

private:

  /*!
   stub to track candidate assignment
  */
  void readIn ();
  /*!
   stub to HT stream assignment
  */
  void readOut() const;

  /*!
  truncate double precision to f/w equivalent
  @param[out] val variable
  @param[in] base f/w precision
  */
  double digi( const double& val, const double& base ) const { return enableDigitize_ ? ( floor( val / base ) + .5 ) * base : val; }

  typedef pair< int, Stub* > Frame;
  typedef vector< Frame >    Frames;

  const Settings* settings_;
        Data*     data_;
  const int       numRegions_;
  const int       numSectorsPhi_;
  const int       numSectorsEta_;
  const int       numSectors_;
  const int       numBinsPt_;
  const int       numBinsPhi_;
  const int       numCells_;
  const double    chosenRofPhi_;
  const double    baseM_;
  const double    baseC_;
  const double    baseSector_;
  const double    baseRegion_;
  const int       numLayers_;
  const int       minLayers_;
  const bool      enableTruncation_;
  const int       maxStubsInCell_;
  const int       maxStubsInStream_;
  const bool      enableDigitize_;
  const double    baseR_;
  const double    basePhi_;
  const double    baseCFull_;
  const int       sizeBuffer_;

  Intss phiBins_;

};

}

#endif