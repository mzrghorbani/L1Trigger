#include "L1Trigger/TrackFindingTMTT/interface/MHT.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"


namespace TMTT {

MHT::MHT( const Settings* settings, Data* data ) :
  settings_        ( settings                     ),
  data_            ( data                         ),
  enableTruncation_( settings->enableTruncation() ),
  enableDigitize_  ( settings->enableDigitize()   ),
  chosenRofPhi_    ( settings->chosenRofPhi()     ),
  numRegions_      ( settings->dtcNumRegions()    ),
  numSectorsPhi_   ( settings->gpNumSectorsPhi()  ),
  numSectorsEta_   ( settings->gpNumSectorsEta()  ),
  numSectors_      ( settings->numSectors()       ),
  numBinsPhi_      ( settings->htNumBinsPhi()     ),
  numBinsPt_       ( settings->htNumBinsPt()      ),
  numCellsHT_      ( settings->htNumCells()       ),
  baseRegion_      ( settings->baseRegion()       ),
  baseSector_      ( settings->baseSector()       ),
  baseR_           ( settings->baseR()            ),
  basePhi_         ( settings->basePhi()          ),
  baseM_           ( settings->baseM()            ),
  baseC_           ( settings->baseC()            ),
  baseCFull_       ( settings->baseCFull()        ),
  mhtNumBinsPt_    ( settings->mhtNumBinsPt()     ),
  mhtNumBinsPhi_   ( settings->mhtNumBinsPhi()    ),
  numCells_        ( settings->mhtNumCells()      ),
  numLayers_       ( settings->trackerNumLayers() ),
  minLayers_       ( settings->mhtMinLayers()     ),
  numMaxStubs_     ( settings->numFramesAlgoIn()  ) {}


void MHT::produce() const {

  const Tracks& tracksHT = data_->tracksHT();

  // organise found tracks into HT streams
  Trackss streams( numRegions_ * numBinsPt_ );
  for ( Track* track : tracksHT )
    streams[ track->streamId() ].push_back( track );

  int id( 0 );
  for ( Tracks& stream : streams ) {

    if ( stream.empty() )
      continue;

    // restore track order
    sort( stream.begin(), stream.end(), []( Track* lhs, Track* rhs ){ return lhs->streamPos() < rhs->streamPos(); } );
    TSss inputStreams( numCells_ );
    for ( TSs& inputStream : inputStreams )
      inputStream.reserve( 256 );

    const int region = id   / numBinsPt_;
    const int link   = id++ % numBinsPt_;

    // perform finer pattern recognition per track
    for ( Track* track : stream ) {

      const int&   region = track->region();
      const int&   binPt  = track->binPt ();
      const int&   binPhi = track->binPhi();
      const int&   secPhi = track->secPhi();
      const Stubs& stubs  = track->stubs ();

      Stubss mhtCells( numCells_ );
      for ( Stubs& cell : mhtCells )
        cell.reserve( stubs.size() );

      for ( Stub* stub : stubs ) {

        // restore track phi aka c
        const double r    = enableDigitize_ ? digi( stub->r() - chosenRofPhi_, baseR_ ) : stub->r() - chosenRofPhi_;
        const double phi  = enableDigitize_ ? digi( digi( reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ), basePhi_ ) - ( secPhi - .5 ) * baseSector_, basePhi_ ) : reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ) - ( secPhi - .5 ) * baseSector_;
        const double c    = enableDigitize_ ? digi( phi + ( binPt + 0.5 ) * baseM_ * r, basePhi_ ) : phi + ( binPt + 0.5 ) * baseM_ * r;
        // restore stub residual
        const double chi  = c - ( binPhi + .5 ) * baseC_;
        const bool compA  = 2. * fabs( chi ) < baseC_;
        const bool compB  = 2. * fabs( chi ) < fabs( r * baseM_ );
        const bool compAB = compA && compB;

        // identify finer track candidates for this stub
        Ints ids; // 0 and 1 belong to smaller sub q/pt; 0 and 2 belong to smaller sub track phi
        if ( chi >= 0. && r <  0. ) { ids.push_back( 3 ); if ( compA ) ids.push_back( 1 ); if( compAB ) ids.push_back( 2 ); }
        if ( chi >= 0. && r >= 0. ) { ids.push_back( 1 ); if ( compA ) ids.push_back( 3 ); if( compAB ) ids.push_back( 0 ); }
        if ( chi <  0. && r <  0. ) { ids.push_back( 0 ); if ( compA ) ids.push_back( 2 ); if( compAB ) ids.push_back( 1 ); }
        if ( chi <  0. && r >= 0. ) { ids.push_back( 2 ); if ( compA ) ids.push_back( 0 ); if( compAB ) ids.push_back( 3 ); }
        // organise stubs in finer track candidates
        for ( const int& id : ids )
          mhtCells[ id ].push_back( stub );

      }

      // organise stubs in streams
      for ( int sel = 0; sel < numCells_; sel++ ) {

        Stubs& mhtCell = mhtCells[ sel ];
        SLV hitPattern( 0, numLayers_ );
        for ( const Stub* stub : mhtCell )
          hitPattern.set( stub->layerId() );
        if ( hitPattern.count() < minLayers_ )
          mhtCell.clear();
        mhtCell.resize( stubs.size(), nullptr );

        for ( Stub* stub : mhtCell )
          inputStreams[ sel ].push_back( make_pair( track, stub ) );

      }

    }

    // read out fine track cannot start before rough track has read in completely, add gaps to take this into account
    for ( TSs& inputStream : inputStreams ) {

      int pos( 0 );
      Track* prevTrack = nullptr;
      for ( int n = 0; n < (int)inputStream.size(); n++ ) {

        Track* track = inputStream[ n ].first;
        if ( track != prevTrack ) {
          pos += track->size();
          prevTrack = track;
        }
        if ( n < pos ) {
          inputStream.insert( inputStream.begin() + n, pos - n, nullTS );
          n = pos;
        }

      }

      // adjust stream start so that first output stub is in first place in case of quickest track (5 stubs)
      inputStream.erase( inputStream.begin(), inputStream.begin() + minLayers_ );

    }

    // cosmetics -- remove gaps at the end of streams
    for( TSs& inputStream : inputStreams )
      for( auto it = inputStream.end(); it != inputStream.begin(); )
        it = (*--it).second == nullptr ? inputStream.erase( it ) : inputStream.begin();

    // mux 4 streams to 1 stream
    vector< pair< TS, int > > outputStream;
    outputStream.reserve( 256 );

    // input fifos
    TSss stacks ( numCells_ );
    for ( TSs& stack : stacks )
      stack.reserve( 128 );

    // helper for handshake
    SLV empty ( -1, numCells_, true );
    SLV enable(  0, numCells_ );

    while( !all_of( inputStreams.begin(), inputStreams.end(), []( const TSs& stubs ){ return stubs.empty(); } ) or
           !all_of( stacks      .begin(), stacks      .end(), []( const TSs& stubs ){ return stubs.empty(); } ) ) { // 1 clock tick

      // store stub in fifo
      for( int input = 0; input < numCells_; input++ ){
        TSs& inputStream = inputStreams[ input ];
        if ( inputStream.empty() )
          continue;
        const TS& ts = inputStream.front();
        if ( ts.second != nullptr )
          stacks[ input ].push_back( ts );
        inputStream.erase( inputStream.begin() );
      }

      // identify empty fifos
      for ( int input = 0; input < numCells_; input++ )
        empty[ input ] = stacks[ input ].empty();

      // chose new fifo to read from if current fifo got empty
      const int iEnableOld = enable.plEncode();
      if ( iEnableOld == numCells_ || empty[ iEnableOld ] ) {
        enable.reset();
        const int iNotEmpty = empty.plEncode( false );
        if ( iNotEmpty < numCells_ )
          enable.set( iNotEmpty );
      }

      // read from chosen fifo
      const int iEnable = enable.plEncode();
      if ( iEnable < numCells_ ) {
        TSs& stack = stacks[ iEnable ];
        outputStream.push_back( make_pair( stack.front(), iEnable ) );
        stack.erase( stack.begin() );
      }
      else
        outputStream.push_back( make_pair( nullTS, -1 ) ); // gap if no fifo has been chosen

    }
    outputStream.push_back( make_pair( nullTS, -1 ) ); // helps downstream new candidate identification

    // build tracks out of stream of stubs
    Tracks& tracks     = data_->tracksMHT();

    Track*  prevTrack  = nullptr; // previous track pointer
    int     prevCellId = -1;// previous track sub parameter
    Stubs stubs; // stubs which form a track
    int pos( 0 );
    for ( const auto& frame : outputStream ) {

      Track*     track  = frame.first.first;
      Stub*      stub   = frame.first.second;
      const int& cellId = frame.second;

      if ( track != prevTrack || prevCellId != cellId ) { // new track or gap
        if ( prevTrack != nullptr && stubs.size() > 0 && prevCellId != -1 ) { // no gap
          // create track
          tracks.push_back( new Track( prevCellId, prevTrack, stubs, pos ) );
          stubs.clear(); // empty stubs for next track
        }
        prevTrack  = track;
        prevCellId = cellId;
      }
      if ( cellId != -1 && ( !enableTruncation_ || pos < numMaxStubs_ ) ) // no gap, valid stub
        stubs.push_back( stub );
      pos++;

    }

  }

}

}