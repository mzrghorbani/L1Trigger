#ifndef __DEMO_H__
#define __DEMO_H__

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <sstream>
#include <string>
#include <cmath>

using namespace std;

namespace TMTT {

class Settings;
class Stub;
class Track;

/**
 * Class to play data through modelsim and to compare with sw
 */
class Demo {

public:

  Demo( const Settings* settings, Data* data );

  ~Demo(){};

  void analyze();

private:

  void write( Stringss& swStrings, Stringss& fsSrings );

  void read( Stringss& strings ) const;

  void compare( const Stringss& swStrings, const Stringss& fwStrings );

  string read( const string& token, const string& type ) const;

  void predict( Stringss& swStrings, Stringss& fwStrings, const string& type ) const;

  string readGP ( SLV& stub ) const;
  string readHT ( SLV& stub ) const;
  string readMHT( SLV& stub ) const;

  string readGP ( const int& ptMax, const int& ptMin, const int& layer, const int& z, const int& phi, const int& r )                     const;
  string readHT ( const int& layer, const int& secEta, const int& binPhi, const int& z, const int& phi, const int& r, const int& mSel )  const;
  string readMHT( const int& phi, const int& r, const int& secEta, const int& z, const int& layer, const int& binPt, const int& binPhi ) const;

  void predictDTC( Stringss& swStrings, Stringss& fwStrings ) const;
  void predictGP ( Stringss& swStrings, Stringss& fwStrings ) const;
  void predictHT ( Stringss& swStrings, Stringss& fwStrings ) const;
  void predictMHT( Stringss& swStrings, Stringss& fwStrings ) const;

  int    digii( const double& val, const double& base ) const { return floor( val / base ); }
  double digif( const double& val, const double& base ) const { return ( floor( val / base ) + .5 ) * base; }

  const Settings* settings_;
  Data*           data_;
  const string    stageIn_;
  const string    stageOut_;
  const int       numRegions_;
  const int       numDTCsPerTFP_;
  const int       numSectorsPhi_;
  const int       numSectorsEta_;
  const int       numSectors_;
  const int       widthBend_;
  const int       widthR_;
  const int       widthPhi_;
  const int       widthZ_;
  const int       widthEta_;
  const int       widthCBin_;
  const int       widthMBin_;
  const int       widthLayer_;
  const int       widthChiPhi_;
  const int       widthChiZ_;
  const int       widthFrame_;
  const int       widthBinPt_;
  const int       widthBinPhi_;
  const double    baseBend_;
  const double    baseM_;
  const double    baseC_;
  const double    baseR_;
  const double    basePhi_;
  const double    baseZ_;
  const double    baseRegion_;
  const double    baseSector_;
  const double    baseCotTheta_;
  const double    chosenRofPhi_;
  const double    chosenRofZ_;
  const int       htNumBinsPt_;
  const int       htNumBinsPhi_;
  const int       mhtNumBinsPt_;
  const int       mhtNumBinsPhi_;
  const int       numCells_;
  const int       numLayers_;
  const Doubles   sectorsCot_;
  const int       numFramesInfra_;
  const int       dtcLenString_;
  const int       gpLenString_;
  const int       htLenString_;
  const int       mhtLenString_;
  const int       numLinksDTC_;
  const int       numLinksGP_;
  const int       numLinksHT_;
  const int       numLinksMHT_;
  const string    dirModelSim_;
  const bool      enableDemo_;

  int    numPayloadFrames_;
  Ints   inputLinks_;
  int    numInputLinks_;
  int    numOutputLinks_;
  string modelsimDir_;
  int    capOffset_;
  double runTime_;
  int    lenOutputString_;
  int    lenInputString_;
  int    widthStub_;

  stringstream cmd_;
  stringstream debugStream_;

};

}

#endif