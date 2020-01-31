#include "L1Trigger/TrackFindingTMTT/interface/HT.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"

#include <algorithm>

namespace TMTT {

HT::HT( const Settings* settings, Data* data ) :
  settings_        ( settings                     ),
  data_            ( data                         ),
  numRegions_      ( settings->dtcNumRegions()    ),
  numSectorsPhi_   ( settings->gpNumSectorsPhi()  ),
  numSectorsEta_   ( settings->gpNumSectorsEta()  ),
  numSectors_      ( settings->numSectors()       ),
  numBinsPt_       ( settings->htNumBinsPt()      ),
  numBinsPhi_      ( settings->htNumBinsPhi()     ),
  numCells_        ( settings->htNumCells()       ),
  chosenRofPhi_    ( settings->chosenRofPhi()     ),
  baseM_           ( settings->baseM()            ),
  baseC_           ( settings->baseC()            ),
  baseSector_      ( settings->baseSector()       ),
  baseRegion_      ( settings->baseRegion()       ),
  numLayers_       ( settings->trackerNumLayers() ),
  minLayers_       ( settings->htMinLayers()      ),
  enableTruncation_( settings->enableTruncation() ),
  maxStubsInCell_  ( settings->numFramesAlgoIn()  ),
  maxStubsInStream_( settings->numFramesAlgoOut() ),
  enableDigitize_  ( settings->enableDigitize()   ),
  baseR_           ( settings->baseR()            ),
  basePhi_         ( settings->basePhi()          ),
  baseCFull_       ( settings->baseCFull()        ),
  sizeBuffer_      ( settings->htSizeBuffer()     )
{

  phiBins_ = Intss( numRegions_ * numSectors_ * numBinsPt_ );

}


void HT::produce() {

  for ( Ints& phiBins : phiBins_ )
    phiBins.clear();

  readIn();

  readOut();

}


void HT::readOut() const {

  const Stubs& stubs = data_->stubs();

  // organise stubs in candidates
  unordered_map< int, Stubs > htCells;
  SLVs hitsPhis( numRegions_ * numSectors_ * numBinsPt_, SLV( 0, numBinsPhi_ ) );
  for ( int ptBin = 0; ptBin < numRegions_ * numSectors_ * numBinsPt_; ptBin++ ) {
    SLV& hitsPhi = hitsPhis[ ptBin ];
    for( const int& phi : phiBins_[ ptBin ] ) {
      hitsPhi.set( phi );
      htCells[ ptBin * numBinsPhi_ + phi ].reserve( 32 );
    }
  }

  for ( Stub* stub : stubs )
    for ( const int& id : stub->htCellIds() ) {
      SLV& hitsPhi = hitsPhis[ id / numBinsPhi_ ];
      if ( hitsPhi[ id % numBinsPhi_ ] )
        htCells[ id ].push_back( stub );
    }

  Tracks& tracks = data_->tracksHT();

  // organise candidates in streams, transverse readout scheme
  for ( int streamId = 0; streamId < numRegions_ * numBinsPt_;  streamId++ ) {

    int streamTrackPos( 0 );
    int streamStubPos( 0 );
    const int region = streamId / numBinsPt_;
    const int link = streamId % numBinsPt_;
    for ( int mSel = 1; mSel > - 1; mSel-- ) { // two q/pt values per stream
      const int pt = link % ( numBinsPt_ / 2 ) + mSel * numBinsPt_ / 2;
      for ( int eta = numSectorsEta_ - 1; eta > -1; eta-- ) { // all eta sectors per stream
        const int sector = ( link / ( numBinsPt_ / 2 ) ) * numSectorsEta_ + eta;
        const int pid    = numBinsPt_ * ( numSectors_ * region + sector ) + pt;
        for ( const int& phiBin : phiBins_[ pid ] ) { // loop over found tracks in the orders as they have been found (first come first served)

          const int id = numBinsPhi_ * pid + phiBin;
          Stubs& htCell = htCells[ id ];
          // restore stub ordering
          sort( htCell.begin(), htCell.end(), [id]( Stub* lhs, Stub* rhs ) { return lhs->htCellPos( id ) < rhs->htCellPos( id ); } );
          if ( enableTruncation_ )
            htCell.erase( remove_if( htCell.begin(), htCell.end(), [id,this]( Stub* stub ){ return stub->htCellPos( id ) >= maxStubsInCell_; } ), htCell.end() );
          // reverse stub ordering
          reverse( htCell.begin(), htCell.end() );
          if ( enableTruncation_ && streamStubPos + (int)htCell.size() > maxStubsInStream_ )
            htCell.resize( maxStubsInStream_ - streamStubPos );
          streamStubPos += htCell.size();
          htCell.shrink_to_fit();
          // create track
          if ( htCell.size() > 0 )
            tracks.push_back( new Track( settings_, id, htCell, streamTrackPos++ ) );

        }
      }
    }

  }
  
}


void HT::readIn() {

  const Stubs& stubs = data_->stubs();

  // organise stubs in sectors
  Stubss stubss( numRegions_ * numSectors_ );
  for ( Stubs& stream : stubss )
    stream.reserve( 256 );

  for ( Stub* stub: stubs )
    for( const int& id : stub->gpStreamIds() )
      stubss[ id ].push_back( stub );

  for ( int id = 0; id < numRegions_ * numSectors_; id++ ) {

    Stubs& stream = stubss[ id ];
    // restore stub ordering
    sort( stream.begin(), stream.end(), [id]( Stub* lhs, Stub* rhs ) { return lhs->gpStreamPos( id ) < rhs->gpStreamPos( id ); } );
    // restore gaps
    for ( int n = 0; n < (int)stream.size(); n++ ) {
      const int& pos = stream[ n ]->gpStreamPos( id );
      stream.insert( stream.begin() + n, pos - n, nullptr );
      n = pos;
    }

    const int region = id / numSectors_;
    const int sector = id % numSectors_;
    const int secPhi = sector / numSectorsEta_;
    const int secEta = sector % numSectorsEta_;
    Stubss ptBins( numBinsPt_ );
    for ( Stubs& stream : ptBins )
      stream.reserve( 32 );

    // organise sector stubs in q/pt bins
    for( Stub* stub : stream ) {
      Range rangePt = make_pair( 0, numBinsPt_ - 1 );
      if ( stub != nullptr )
        rangePt = stub->rangePt();
      for ( int pt = 0; pt < numBinsPt_; pt++ ) {
        Stub* s = nullptr;
        if ( pt >= rangePt.first && pt <= rangePt.second )
          s = stub;
        ptBins[ pt ].push_back( s );
      }
    }

    // perform pattern recognition per pt bin
    for ( int pt = 0; pt < numBinsPt_; pt++ ) {
      const int pid = numBinsPt_ * ( numSectors_ * region + sector ) + pt;
      // hitPattern for all possible track phis
      SLVs hitPattern( numBinsPhi_, SLV( 0, numLayers_ ) );
      // found tracks, helper to recognise point in time when a track is found first time
      SLV trackPattern( 0, numBinsPhi_ );
      const int mBin = pt - numBinsPt_ / 2;
      // fifo, used to store stubs which belongs to a second possible track
      Frames stack;
      Stubs& binStubs = ptBins[ pt ];
      int pos( 0 );
      // helper, fifo can read a word first one clock cycle after it is written
      bool tooNew( true );
      // process all stubs, one stub per clock tick
      while ( !binStubs.empty() || !stack.empty() ) { // one clock tick
        Stub* s( nullptr );
        int phiBin( -1 );

        if ( !binStubs.empty() ) {
          Stub* stub = binStubs.front(); 
          binStubs.erase( binStubs.begin() );
          if ( stub != nullptr ) {
            // calculate track phi aka c
            const double r      = digi( stub->r() - chosenRofPhi_, baseR_ );
            const double phi    = digi( digi( reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ), basePhi_ ) - ( secPhi - .5 ) * baseSector_, basePhi_ );
            const double c      = digi( phi + ( mBin + 0.5 ) * baseM_ * r, baseCFull_ );
            const int    cBin   = floor( c / baseC_ );
            // calculate stub residual
            const double chi    = c - ( cBin + .5 ) * baseC_;
                         phiBin = cBin + numBinsPhi_ / 2;
            // identify if stub belong to two candidates
            int bin( -1 );
            const bool comp = fabs( r * baseM_ ) + 2. * fabs( chi ) >= baseC_;
            if ( comp ) { // stub belongs to two candidates
              if ( chi >= 0. ) bin = phiBin + 1; else bin = phiBin - 1;
              if ( bin > -1 && bin < numBinsPhi_ ) // second (minor) candidate has pt > threshold (3 GeV)
                // store stub in fifo
                if ( !enableTruncation_ || (int)stack.size() < sizeBuffer_ ) {
                  tooNew = true;
                  stack.push_back( make_pair( bin, stub ) );
                }
            }
            if ( phiBin > -1 && phiBin < numBinsPhi_ ) // major candidate has pt > threshold (3 GeV)
              s = stub;
          }
        }

        // look in fifo in case of no new input stub
        if ( s == nullptr && !stack.empty() && ( (int)stack.size() > 1 || !tooNew ) ) {
          const Frame& f = stack.front();
          phiBin = f.first;
          s      = f.second;
          stack.erase( stack.begin() );
        }
        tooNew = false;

        // assign stub to track candidate and perform pattern recognition
        if ( s != nullptr ) {
          const int id = pid * numBinsPhi_ + phiBin;
          s->htCellPos( id, pos );
          SLV& hits = hitPattern[ phiBin ];
          if ( !enableTruncation_ || pos < maxStubsInCell_ )
            hits.set(  s->layerId() );
          const bool foundTrk = hits.count() >= minLayers_;
          if ( foundTrk && !trackPattern[ phiBin ] ) { // first time track found
            trackPattern.set( phiBin );
            phiBins_[ pid ].push_back( phiBin ); // store found tracks, ordered in time
          }
        }
        pos++;
      }
    }
  }

}

}