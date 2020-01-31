#include "L1Trigger/TrackFindingTMTT/interface/TP.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/SLV.h"


using namespace std;

namespace TMTT {

TP::TP( const Settings* settings, const TrackingParticlePtr& tpPtr, const bool& tight, vector< const reco::GenJet* > jets ) :
  settings_ ( settings              ),
  edmPtr_   ( tpPtr                 ),
  eta_      ( tpPtr->eta()          ),
  phi_      ( tpPtr->phi()          ),
  pt_       ( tpPtr->pt()           ),
  qOverPt_  ( tpPtr->charge() / pt_ )
{

  const double cot = sinh( eta_ );
  const TrackingParticle::Point& v = tpPtr->vertex();
  const double d0 =                 v.x() * sin( phi_ ) - v.y() * cos( phi_ )  ;
  const double z0 = v.z() - cot * ( v.x() * cos( phi_ ) + v.y() * sin( phi_ ) );
  useForEff_ = tight && ( fabs( d0 ) <= settings->tpMaxD0() ) && ( fabs( z0 ) <= settings->trackerBeamWindowZ() );

  const double maxDR  = 0.4;
  double minDR        = 999.;
  tpInJet_           = false;
  for ( const auto& jet : jets ) {
    double deltaR = reco::deltaR( eta_, phi_, jet->eta(), jet->phi() );
    if ( deltaR < minDR ) {
      minDR         = deltaR;
      tpInJet_      = deltaR < maxDR;
      nearestJetPt_ = jet->pt();
    }
  }
  useForAlgEff_            = false;
  reconstructableAfterDTC_ = false;
  reconstructableAfterGP_  = false;

}


void TP::assocComplete() {

  if ( !useForEff_ )
    return;

  useForAlgEff_ = reconstructable( assocStubs_ );

}

void TP::routingComplete() {

  if ( !useForAlgEff_ )
    return;

  const int&  numRegions        = settings_->dtcNumRegions();
  const int&  numSectors        = settings_->numSectors();
  const bool& enableTruncation  = settings_->enableTruncation();
  const int&  numFramesAglgoOut = settings_->numFramesAlgoOut();
  const int&  numFramesAglgoIn  = settings_->numFramesAlgoIn();

  Stubss dtcStubs( numRegions );
  Stubss gpStubs ( numRegions * numSectors );
  for ( Stub* stub : assocStubs_ ) {
    for ( const int& id : stub->dtcStreamIds() )
      if ( !enableTruncation || stub->dtcStreamPos( id ) < numFramesAglgoOut )
        dtcStubs[ id / settings_->dtcNumPerTFP() ].push_back( stub );
    for ( const int& id : stub->gpStreamIds() )
      if ( !enableTruncation || stub->gpStreamPos( id ) < numFramesAglgoIn )
        gpStubs[ id ].push_back( stub );
  }
  reconstructableAfterDTC_ = reconstructable( dtcStubs );
  reconstructableAfterGP_  = reconstructable( gpStubs  );

}

int TP::numMatchedLayers( const Stubs& stubs ) const {

  const int& numLayers = settings_->trackerMaxLayerId() + 1;

  SLV hitPattern( 0, numLayers );
  for ( const Stub* stub : stubs )
    hitPattern.set( stub->uniqueLayerId() );

  return hitPattern.count();

}

bool TP::reconstructable( const Stubs& stubs ) {

  const int& minLayersMatch = settings_->tpMinLayersMatch();

  return numMatchedLayers( stubs ) >= minLayersMatch;

}

bool TP::reconstructable( const Stubss& stubss ) {

  return !all_of( stubss.begin(), stubss.end(), [this]( const Stubs& stubs ){ return !reconstructable( stubs ); } );

}

}
