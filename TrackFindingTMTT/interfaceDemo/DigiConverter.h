#ifndef __DEMONSTRATOR_DATAFORMATS_DIGICONVERTER_HPP__
#define __DEMONSTRATOR_DATAFORMATS_DIGICONVERTER_HPP__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <cmath>

using namespace std;

namespace demo {

class DigiConverter {

public:

  DigiConverter( const TMTT::Settings* settings, TMTT::Data* data );

  ~DigiConverter() {};

  void put( edm::Event& iEvent ) const;

private:

  double digi( const double& val, const double& base ) const { return ( floor( val / base ) + .5 ) * base; }

  void putDTC( edm::Event& iEvent ) const;
  void putGP ( edm::Event& iEvent ) const;
  void putHT ( edm::Event& iEvent ) const;
  void putMHT( edm::Event& iEvent ) const;
  void putKF4( edm::Event& iEvent ) const;

  const TMTT::Settings* settings_;
        TMTT::Data*     data_;
  const int             numRegions_;
  const int             numSectorsEta_;
  const double          baseBend_;
  const double          baseM_;
  const double          baseC_;
  const double          baseR_;
  const double          basePhi_;
  const double          baseZ_;
  const double          baseRegion_;
  const double          baseSector_;
  const double          baseCotTheta_;
  const double          chosenRofPhi_;
  const double          chosenRofZ_;
  const int             numBinsPt_;
  const int             numBinsPhi_;
  const TMTT::Doubles   sectorsCot_;
  const int             numLinksDTC_;
  const int             numLinksGP_;
  const int             numLinksHT_;
  const int             numLinksMHT_;

};

}

#endif
