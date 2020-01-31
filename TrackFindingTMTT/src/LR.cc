#include "L1Trigger/TrackFindingTMTT/interface/LR.h"


using namespace std;

namespace TMTT {

LR::LR( const Settings* settings, Data* data ) :
  settings_     ( settings                     ),
  data_         ( data                         ),
  numLayers_    ( settings->trackerNumLayers() ),
  minLayers_    ( settings->lrMinLayers()      ),
  minLayersPS_  ( settings->lrMinLayersPS()    ),
  residPhi_     ( settings->lrResidPhi()       ),
  residZ2S_     ( settings->lrResidZ2S()       ),
  residZPS_     ( settings->lrResidZPS()       ),
  maxIteartions_( settings->lrNumIterations()  ) {}


void LR::produce() {

  const Tracks& tracks = data_->tracksMHT();

  for ( Track* track : tracks ) {
    track_ = track;
    initFit();
    if ( not valid_ ) {
      create();
      continue;
    }
    while ( true ) {
      nIterations_++;
      calcHelix();
      calcResidual();
      bool nothingToKill = killLargestResidual();
      if ( nothingToKill )
        break;
      if ( nIterations_ == maxIteartions_ )
        break;
    }
    create();
  }

}


void LR::create() {

  Tracks& tracks = data_->tracksLR();

  if ( valid_ ) {
    LRParameter_ += HTParameter_;
    tracks.push_back( new Track( track_, stubs_, LRParameter_.qOverPt, LRParameter_.phiT, LRParameter_.cotTheta, LRParameter_.zT ) );
  }

}

void LR::initFit() {
 
  nIterations_ = 0;
  secPhi_ = track_->secPhi();
  secEta_ = track_->secEta();
  HTParameter_ = LRTrack( track_->qOverPt(), track_->phi(), track_->cot(), track_->z() );
  HTParameter_.convert( settings_ );
  LRParameter_ = HTParameter_;
  stubs_ = track_->stubs();
  valid_ = checkValidity( stubs_ );
  if ( not valid_ )
    return;
  stubMap_.clear();
  layerPopulation_.clear();
  for ( auto const &stub : stubs_ ) {
    stubMap_[ stub->layerId() ].push_back( stub );
    layerPopulation_[ stub->layerId() ]++;
  }
  residuals_.clear();
  largestResid_ = Residual( -1. );
 
}


bool LR::checkValidity( const Stubs& stubs ) const {
 
  bool valid = true;
  if ( countLayers( stubs ) < minLayers_ )
    valid = false;
  if ( countLayers( stubs, true ) < minLayersPS_ )
    valid = false;
  return valid;
 
}


int LR::countLayers( const Stubs& stubs, const bool& onlySeed ) const {
 
  SLV foundLayers( 0, numLayers_ );
  for ( auto const &stub : stubs )
    if ( !onlySeed || stub->psModule() )
        foundLayers.set( stub->layerId() );
  return foundLayers.count();
 
}


void LR::calcHelix( ) {
 
  Sum phiSums, zSums;
  for ( auto const& layer : stubMap_ ) if ( layer.second.size() > 0 ) {
    LRStub layerMinPos( numeric_limits< double >::infinity(), numeric_limits< double >::infinity(), numeric_limits< double >::infinity(), numeric_limits< double >::infinity() );
    LRStub layerMaxPos(-numeric_limits< double >::infinity(),-numeric_limits< double >::infinity(),-numeric_limits< double >::infinity(),-numeric_limits< double >::infinity() );
    bool ps( false );
    for ( auto const& stub : layer.second ) {
      LRStub pos( stub, HTParameter_, settings_ );
      if ( stub->psModule() ) {
        ps = true;
        layerMinPos <= pos;
        layerMaxPos >= pos;
      }
      else {
        layerMinPos.RPhi = min( layerMinPos.RPhi, pos.RPhi );
        layerMinPos.Phi = min( layerMinPos.Phi, pos.Phi );
        layerMaxPos.RPhi = max( layerMaxPos.RPhi, pos.RPhi );
        layerMaxPos.Phi = max( layerMaxPos.Phi, pos.Phi );
      }
    }
    LRStub layerPos = layerMinPos + layerMaxPos;
    layerPos /= 2.;
    phiSums += make_pair( layerPos.RPhi, layerPos.Phi );
    if ( ps )
      zSums += make_pair( layerPos.RZ, layerPos.Z );
  }
  const pair< float, float >& phiParameter = phiSums.calcLinearParameter();
  const pair< float, float >& zParameter = zSums.calcLinearParameter();
  LRParameter_ = LRTrack( phiParameter.first, phiParameter.second, zParameter.first, zParameter.second );
 
}


void LR::calcResidual() {
 
  residuals_.clear();
  for ( auto const &layer : stubMap_ ) if ( layer.second.size() > 0 ) {
    unsigned int stubId = 0;
    for ( auto const &stub : layer.second ) {
      LRStub pos( stub, HTParameter_, settings_ );
      float zResid = pos.Z - ( LRParameter_.zT + LRParameter_.cotTheta * pos.RZ );
      float phiResid = reco::deltaPhi( pos.Phi - ( LRParameter_.phiT + LRParameter_.qOverPt * pos.RPhi ) , 0. );
      Residual resid( fabs( phiResid ), fabs( zResid ), layer.first, stubId, stub->psModule() );
      stubId++;
      resid.phi /= residPhi_;
      if ( !stub->barrel() )
        resid.z /= fabs( HTParameter_.cotTheta );
      if ( stub->psModule() )
        resid.z /= residZPS_;
      else
        resid.z /= residZ2S_;
      residuals_[ layer.first ].push_back( resid );
    }
  }
 
}


bool LR::killLargestResidual() {
 
  findLargestResidual();
  valid_ = largestResid_.combined() < 2.;
  if ( countStubs( stubs_ ) == 4 )
    return true;
  const int& layerID = largestResid_.layerId ;
  Stubs& stubs = stubMap_[ layerID ];
  const Stub* stub = stubs[ largestResid_.stubId ];
  stubs.erase( remove( stubs.begin(), stubs.end(), stub ), stubs.end() );
  stubs_.erase( remove( stubs_.begin(), stubs_.end(), stub ), stubs_.end() );
  layerPopulation_[ layerID ]--;
  return false;
 
}


void LR::findLargestResidual() {
 
  largestResid_ = Residual( -1. );
  for ( auto const &layer : residuals_ ) {
    bool single = layer.second.size() == 1;
    bool notPurged = countStubs( stubs_ ) != countLayers( stubs_ );
    bool layerCritical = countLayers( stubs_ ) == minLayers_;
    bool psCritical = countLayers( stubs_, true ) == minLayersPS_;
    if ( single && notPurged && layerCritical )
      continue;
    for ( auto const &resid : layer.second ) {
      if( resid.ps && psCritical && countStubs( stubs_ ) > 4 )
        if ( layerPopulation_[ stubMap_[ layer.first ][ resid.stubId ]->layerId() ] == 1 )
          continue;
      if ( resid.combined() > largestResid_.combined() )
        largestResid_ = resid;
    }
  }
 
}


int LR::countStubs( const Stubs& stubs, const bool& onlyPS ) const {
 
  int nStubs( 0 );
  if ( onlyPS ) {
    for ( const Stub* stub : stubs )
      if ( stub->psModule() )
        nStubs++;
  } else
    nStubs = stubs_.size();
  return nStubs;
 
}

}