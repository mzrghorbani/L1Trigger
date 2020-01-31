#include "L1Trigger/TrackFindingTMTT/interface/DTC.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"

#include <algorithm>

namespace TMTT {

DTC::DTC( const Settings* settings, Data* data ) :
  settings_           ( settings                        ),
  data_               ( data                            ),
  numRegions_         ( settings->dtcNumRegions()       ),
  numSectorsPhi_      ( settings->gpNumSectorsPhi()     ),
  numDTCsPerRegion_   ( settings->dtcNumPerRegion()     ),
  numDTCsPerTFP_      ( settings->dtcNumPerTFP()        ),
  fracEta_            ( 2                               ),
  numDTCsPerEtaRegion_( numDTCsPerRegion_ / fracEta_    ),
  fracPhi_            ( 2                               ),
  num10GPSPerRegion_  ( settings->dtcNum10GPS()         ),
  orderStubsByBend_   ( settings->dtcOrderStubsByBend() ) {}


void DTC::produce() const {

  const Stubs& stubs = data_->stubs();

  // stub collection which represents a processing nonant
  Stubss tfpStubs( numRegions_ );
  for ( Stubs& stream : tfpStubs )
    stream.reserve( 2048 );

  // organise stubs into processing nonants
  for ( Stub* stub : stubs )
    if ( stub->passDTC() ) {
      const Range rangePhi    = stub->rangePhi(); // range of phi sectors this stub is in
      const Range rangeRegion = make_pair( rangePhi.first / numSectorsPhi_, rangePhi.second / numSectorsPhi_ ); // range of processing nonants this stub is in
      tfpStubs[ rangeRegion.first ].push_back( stub );
      if ( rangeRegion.first != rangeRegion.second ) // a stub cannot be in more than two processing nonants
        tfpStubs[ rangeRegion.second ].push_back( stub );
    }

  // assign stubs to DTCs
  for ( int region = 0; region < numRegions_; region++ ) {

    Stubs& tfp = tfpStubs[ region ];
    if ( orderStubsByBend_ )
      sort( tfp.begin(), tfp.end(), []( Stub* lhs, Stub* rhs ){ return fabs( lhs->bend() ) < fabs( rhs->bend() ); } );

    // gap-less round robin scheme
    int dtc( 0 );
    int pos( 0 );
    for ( Stub* stub : tfp ) {
      const int link = region * numDTCsPerTFP_ + dtc;
      stub->dtcStreamPos( link, pos );
      if ( ++dtc == numDTCsPerTFP_ ) {
        dtc = 0;
        pos++;
      }
    }

  }

}

}