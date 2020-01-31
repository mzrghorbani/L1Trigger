#ifndef __MHT_H__
#define __MHT_H__

#include <cmath>


using namespace std;

namespace TMTT {

class Settings;
class Data;

/**
 * Class to assign Stubs to finer r-phi track candidates
 */
class MHT {

public:

  MHT( const Settings* settings, Data* data );

  ~MHT() {}

  void produce() const;

private:

  void readIn () const;
  void readOut() const;

  /*!
  truncate double precision to f/w equivalent
  @param[out] val variable
  @param[in] base f/w precision
  */
  double digi( const double& val, const double& base ) const { return ( floor( val / base ) + .5 ) * base; }

  const Settings* settings_;
        Data*     data_;
  const bool      enableTruncation_;
  const bool      enableDigitize_;
  const double    chosenRofPhi_;
  const int       numRegions_;
  const int       numSectorsPhi_;
  const int       numSectorsEta_;
  const int       numSectors_;
  const int       numBinsPhi_;
  const int       numBinsPt_;
  const int       numCellsHT_;
  const double    baseRegion_;
  const double    baseSector_;
  const double    baseR_;
  const double    basePhi_;
  const double    baseM_;
  const double    baseC_;
  const double    baseCFull_;
  const int       mhtNumBinsPt_;
  const int       mhtNumBinsPhi_;
  const int       numCells_;
  const int       numLayers_;
  const int       minLayers_;
  const int       numMaxStubs_;

};

}

#endif