#include "L1Trigger/TrackFindingTMTT/interface/GP.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"

#include <algorithm>

namespace TMTT {

GP::GP( const Settings* settings, Data* data ) :
  settings_     ( settings                                             ),
  data_         ( data                                                 ),
  numRegions_   ( settings->dtcNumRegions()                            ),
  numDTCsPerTFP_( settings->dtcNumPerTFP()                             ),
  fracPhi_      ( settings->gpNumSectorsPhi()                          ),
  fracEta_      ( 3                                                    ),
  numSectorsEta_( settings->gpNumSectorsEta()                          ),
  numSectors_   ( settings->numSectors()                               ),
  numStages_    ( 3                                                    ),
  numInputs_    ( { 0, 4, 4, fracEta_ }                                ),
  numOutputs_   ( { 1, numSectorsEta_ / fracEta_, fracEta_, fracPhi_ } )
{

  numRouter_.push_back( numDTCsPerTFP_ );
  for ( int stage = 1; stage <= numStages_; stage ++  )
    numRouter_.push_back( ( numRouter_[ stage - 1 ] * numOutputs_[ stage - 1 ] ) / numInputs_[ stage ] );

}


void GP::produce() const {

  const Stubs& stubs = data_->stubs();

  // organise stubs into DTCs
  Stubsss dtcStreams( numRegions_, Stubss( numDTCsPerTFP_ ) );
  for ( Stubss& streams : dtcStreams )
    for ( Stubs& stream : streams )
      stream.reserve( 256 );

  for ( Stub* stub: stubs )
    for( const int& id :stub->dtcStreamIds() )
      dtcStreams[ id / numDTCsPerTFP_ ][ id % numDTCsPerTFP_ ].push_back( stub );

  for ( int id = 0; id < numRegions_ * numDTCsPerTFP_; id++ ) {
    Stubs& stubs = dtcStreams[ id / numDTCsPerTFP_ ][ id % numDTCsPerTFP_ ];
    // restore stub ordering
    sort( stubs.begin(), stubs.end(), [id]( Stub* lhs, Stub* rhs ) { return lhs->dtcStreamPos( id ) < rhs->dtcStreamPos( id ); } );
    // restore gaps
    for ( int n = 0; n < (int)stubs.size(); n++ ) {
      const int& pos = stubs[ n ]->dtcStreamPos( id );
      stubs.insert( stubs.begin() + n, pos - n, nullptr );
      n = pos;
    }
    stubs.shrink_to_fit();
  }

  for ( int region = 0; region < numRegions_; region++ ) {

    Stubsss ioStreams( numRouter_[ 0 ], Stubss( numOutputs_[ 0 ] ) );
    for ( int id = 0; id < numDTCsPerTFP_; id++ )
      ioStreams[ id ][ 0 ].swap( dtcStreams[ region ][ id ] );

    for ( int stageNumber = 1; stageNumber < numStages_ + 1; stageNumber++ )
      stage( stageNumber, ioStreams, region );

    // teach all stubs their gp streams and their position in that streams
    for ( int router = 0; router < (int)ioStreams.size(); router++ )
      for ( int output = 0; output < (int)ioStreams[ router ].size(); output++ ) {
        Stubs& stream = ioStreams[ router ][ output ];
        const int eta = ( router % numOutputs_[ 1 ] ) * fracEta_ + router / numOutputs_[ 1 ];
        const int id  = eta + output * numSectorsEta_ + region * numSectors_;
        for ( int pos = 0; pos < (int)stream.size(); pos++ ) {
          // assign stub to sector
          Stub* stub = stream[ pos ];
          if ( stub != nullptr )
            stub->gpStreamPos( id, pos );
        }
      }

  }

}


void GP::stage( const int& stage, Stubsss& ioStreams, const int& region ) const {

  const int& numRouter  = numRouter_ [ stage ];
  const int& numInputs  = numInputs_ [ stage ];

  // create inputStreams for that stage
  Framesss inputStreamss( numRouter, Framess( numInputs ) );

  for ( int router = 0; router < numRouter; router++ )
    for ( int input = 0; input < numInputs; input++ ) {
      // map previous streams to that router output
      const int tmp        = numRouter_ [ stage - 1 ] / numInputs;
      const int prevRouter = numInputs * ( router % tmp ) + input;
      const int prevOutput = router / tmp;
      Stubs&  stubs  = ioStreams[ prevRouter ][ prevOutput ];
      Frames& frames = inputStreamss[ router ][ input ];
      frames.reserve( stubs.size() );
      // create list of requested outputs  per stub
      for ( Stub* stub : stubs ) {
        Ints outputs;
        if ( stub != nullptr )
          switch( stage ) {
            case 1 :
              for( const int& sector : sectorsEta( stub ) )
                if ( true ) {
                  const int& output = sector / fracEta_;
                  if ( outputs.empty() || outputs.back() != output )
                    outputs.push_back( output );
                }
              break;
            case 2 :
              for( const int& sector : sectorsEta( stub ) )
                if ( sector / fracEta_ == prevOutput ) {
                  const int& output = sector % fracEta_;
                  if ( outputs.empty() || outputs.back() != output )
                    outputs.push_back( output );
                }
              break;
            case 3 :
              for( const int& sector : sectorsPhi( stub ) )
                if ( sector / fracPhi_ == region ) {
                  const int& output = sector % fracPhi_;
                  if ( outputs.empty() || outputs.back() != output )
                    outputs.push_back( output );
                }
              break;
          }
        frames.push_back( make_pair( outputs, stub ) );
      }
    }

  // route inputStreams to outputStreams for that stage
  ioStreams = Stubsss( numRouter, Stubss( numOutputs_[ stage ] ) );
  for ( int router = 0; router < numRouter; router++ ) {
    Framess& inputStreams  = inputStreamss[ router ];
    Stubss&  outputStreams = ioStreams[ router ];
    for ( Stubs& outputStream : outputStreams )
      outputStream.reserve( 256 );
    route( inputStreams, outputStreams );
  }

}

void GP::route( Framess& inputStreams, Stubss& outputStreams ) const {

  reverse( inputStreams.begin(), inputStreams.end() );
  Framess stacks( inputStreams.size() );
  for ( Frames& stack : stacks )
    stack.reserve( 128 );

  while( !all_of( inputStreams.begin(), inputStreams.end(), []( const Frames& s ){ return s.empty(); } ) or
         !all_of( stacks      .begin(), stacks      .end(), []( const Frames& s ){ return s.empty(); } ) ) { // 1 clock tick

    for( int input = 0; input < (int)inputStreams.size(); input++ ){
      Frames& inputStream = inputStreams[ input ];
      Frames& stack       = stacks      [ input ];
      if ( inputStream.empty() )
        continue;
      Frame& frame = inputStream.front();
      if ( frame.second != nullptr )
        stack.push_back( frame );
      inputStream.erase( inputStream.begin() );
    }

    for( int output = outputStreams.size() - 1; output > - 1; output-- ) {
      Stubs& outputStream = outputStreams[ output ];
      bool empty( true );
      // feed output by an input if possible
      for( Frames& stack : stacks ) {
        if ( stack.empty() )
          continue;
        Frame& frame   = stack.front();
        Stub*  stub    = frame.second;
        Ints&  outputs = frame.first;
        auto matchedOutput = find( outputs.begin(), outputs.end(), output );
        if ( matchedOutput != outputs.end() ) {
          outputStream.push_back( stub );
          outputs.erase( matchedOutput );
          empty = false;
          break;
        }
      }
      if ( empty )
        // create a gap if an output can't be feed by any input
        outputStream.push_back( nullptr );
    }

    for( Frames& stack : stacks  )
      if ( !stack.empty() )
        if ( stack.front().first.empty() )
          stack.erase( stack.begin() );

  }

  // cosmetics - remove all gaps between end and last stub in outputStreams
  for( Stubs& outputStream : outputStreams )
    for( auto it = outputStream.end(); it != outputStream.begin(); )
      it = (*--it) == nullptr ? outputStream.erase( it ) : outputStream.begin();

}


Ints GP::sectorsPhi( Stub* stub ) const {
  Ints sectors;
  const Range& rangePhi = stub->rangePhi();
  int p = rangePhi.first;
  while ( true ) {
    sectors.push_back( p );
    if ( p == rangePhi.second )
      break;
    p = p == settings_->numSectorsPhi() - 1 ? 0 : p + 1;
  }
  return sectors;
};

Ints GP::sectorsEta( Stub* stub ) const {
  Ints sectors;
  const Range& rangeEta = stub->rangeEta();
  for ( int e = rangeEta.first; e <= rangeEta.second; e++ )
    sectors.push_back( e );
  return sectors;
};

}