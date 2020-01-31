#include "L1Trigger/TrackFindingTMTT/interface/KF.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

KF::KF( const Settings* settings, Data* data ) : settings_( settings ), data_( data ) {}


void KF::produce() {

  // take these tracks
  const Tracks& tracksMHT = data_->tracksMHT();

  // fit them
  
  // store fitted tracks here
  Tracks& tracksKF = data_->tracksKF();

}

}