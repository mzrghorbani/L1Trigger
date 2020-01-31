#include "L1Trigger/TrackFindingTMTT/interface/DR.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"

#include <algorithm>

namespace TMTT {

DR::DR( const Settings* settings, Data* data ) : settings_( settings ), data_( data ), strict_( settings->drStrict() ) {}


void DR::produce() {

  mhtIds_.clear();
  if ( !strict_ ) {
    for ( const Track* track : data_->tracksMHT() )
      mhtIds_.push_back( track->cellId() );
  }
  mhtIds_.shrink_to_fit();

//  clean( data_->tracksLR() );

  clean( data_->tracksKF() );

}


void DR::clean( Tracks& tracks ) const {

  for ( Track* track : tracks ) {
    const int& mhtId = track->parent()->cellId();
    const int& lrId  = track->cellId();
    if ( ( lrId != mhtId ) && ( !strict_ || find( mhtIds_.begin(), mhtIds_.end(), lrId ) != mhtIds_.end() ) )
      track->kill();
  }

}


}
