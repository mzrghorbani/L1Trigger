#include "L1Trigger/TrackFindingTMTT/interface/Demo.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"

#include <fstream>


namespace TMTT {

Demo::Demo( const Settings* settings, Data* data ) :
  settings_      ( settings                     ),
  data_          ( data                         ),
  stageIn_       ( settings->demoStageIn()      ),
  stageOut_      ( settings->demoStageOut()     ),
  numRegions_    ( settings->dtcNumRegions()    ),
  numDTCsPerTFP_ ( settings->dtcNumPerTFP()     ),
  numSectorsPhi_ ( settings->gpNumSectorsPhi()  ),
  numSectorsEta_ ( settings->gpNumSectorsEta()  ),
  numSectors_    ( settings->numSectors()       ),
  widthBend_     ( settings->widthBend()        ),
  widthR_        ( settings->widthR()           ),
  widthPhi_      ( settings->widthPhi()         ),
  widthZ_        ( settings->widthZ()           ),
  widthEta_      ( settings->widthEta()         ),
  widthCBin_     ( settings->widthCBin()        ),
  widthMBin_     ( settings->widthMBin()        ),
  widthLayer_    ( settings->widthLayer()       ),
  widthChiPhi_   ( settings->widthChiPhi()      ),
  widthChiZ_     ( settings->widthChiZ()        ),
  widthFrame_    ( settings->demoWidthFrame()   ),
  widthBinPt_    ( settings->widthBinPt()       ),
  widthBinPhi_   ( settings->widthBinPhi()      ),
  baseBend_      ( settings->baseBend()         ),
  baseM_         ( settings->baseM()            ),
  baseC_         ( settings->baseC()            ),
  baseR_         ( settings->baseR()            ),
  basePhi_       ( settings->basePhi()          ),
  baseZ_         ( settings->baseZ()            ),
  baseRegion_    ( settings->baseRegion()       ),
  baseSector_    ( settings->baseSector()       ),
  baseCotTheta_  ( settings->baseCotTheta()     ),
  chosenRofPhi_  ( settings->chosenRofPhi()     ),
  chosenRofZ_    ( settings->chosenRofZ()       ),
  htNumBinsPt_   ( settings->htNumBinsPt()      ),
  htNumBinsPhi_  ( settings->htNumBinsPhi()     ),
  mhtNumBinsPt_  ( settings->mhtNumBinsPt()     ),
  mhtNumBinsPhi_ ( settings->mhtNumBinsPhi()    ),
  numCells_      ( settings->htNumCells()       ),
  numLayers_     ( settings->trackerNumLayers() ),
  sectorsCot_    ( settings->cots()             ),
  numFramesInfra_( settings->numFramesInfra()   ),
  dtcLenString_  ( settings->dtcLenString()     ),
  gpLenString_   ( settings->gpLenString()      ),
  htLenString_   ( settings->htLenString()      ),
  mhtLenString_  ( settings->mhtLenString()     ),
  numLinksDTC_   ( settings->dtcNumPerTFP()     ),
  numLinksGP_    ( numSectors_                  ),
  numLinksHT_    ( htNumBinsPt_                 ),
  numLinksMHT_   ( htNumBinsPt_                 ),
  dirModelSim_   ( settings->demoDirModelSim()  ),
  enableDemo_    ( settings_->demoEnable()      )
{

  if ( stageIn_ == "DTC" ) {
    inputLinks_     = settings->dtcLinks();
    lenInputString_ = dtcLenString_;
  } else if ( stageIn_ == "GP" ) {
    inputLinks_     = settings->gpLinks();
    lenInputString_ = gpLenString_;
  } else if ( stageIn_ == "HT" ) {
    inputLinks_     = settings->htLinks();
    lenInputString_ = htLenString_;
  } else if ( stageIn_ == "MHT" ) {
    inputLinks_     = settings->mhtLinks();
    lenInputString_ = mhtLenString_;
  } else
    throw cms::Exception( "Demo::Demo unknown input stage (" + stageIn_ + ") specified." );
  if ( stageOut_ == "GP" ) {
    widthStub_        = 1 + widthR_ + widthPhi_ + widthChiZ_ + widthLayer_ + widthMBin_ + widthMBin_;
    numPayloadFrames_ = settings->numFramesAlgoIn();
    numOutputLinks_   = numLinksGP_;
    lenOutputString_  = gpLenString_;
    capOffset_        = settings->offsetDTCGP();
    runTime_          = settings->runTimeDTCGP();
    if ( stageIn_ != "DTC" )
      throw cms::Exception( "Demo::Demo bad Demo config " + stageIn_ + " -> " + stageOut_ + "." );
  } else if ( stageOut_ == "HT" ) {
    widthStub_        = 1 + 1 + widthR_ + widthChiPhi_ + widthChiZ_ + widthCBin_ + widthEta_ + widthLayer_;
    numPayloadFrames_ = settings->numFramesAlgoOut();
    numOutputLinks_   = numLinksHT_;
    lenOutputString_  = htLenString_;
    capOffset_        = settings->offsetDTCHT();
    runTime_          = settings->runTimeDTCHT();
    if ( stageIn_ != "DTC" )
      throw cms::Exception( "Demo::Demo bad Demo config " + stageIn_ + " -> " + stageOut_ + "." );
  } else if ( stageOut_ == "MHT" ) {
    widthStub_        = 1 + widthBinPhi_ + widthBinPt_ + widthLayer_ + widthZ_ + widthEta_ + widthR_ + widthPhi_;
    numPayloadFrames_ = settings->numFramesAlgoIn();
    numOutputLinks_   = numLinksMHT_;
    lenOutputString_  = mhtLenString_;
    capOffset_        = settings->offsetHTMHT();
    runTime_          = settings->runTimeHTMHT();
    if ( stageIn_ != "HT" )
      throw cms::Exception( "Demo::Demo bad Demo config " + stageIn_ + " -> " + stageOut_ + "." );
  } else
    throw cms::Exception( "Demo::Demo unknown output stage (" + stageOut_ + ") requested." );
  numInputLinks_ = inputLinks_.size();
  modelsimDir_ = dirModelSim_ + stageIn_ + stageOut_;
  cmd_ << "cwd=$(pwd) && cp in.txt " << modelsimDir_ << " && cd " << modelsimDir_ << " && ./vsim -quiet -c work.top -Gsourcefile=in.txt -Gsinkfile=out.txt -Gcapoffset=" << capOffset_ << " -do 'run " << runTime_ << "us' -do 'quit' &>tmp && cp out.txt $cwd";

}

void Demo::analyze() {

  if ( !enableDemo_ )
    return;

  debugStream_.str(string());
  debugStream_.clear();

  Stringss swInStrings ( numRegions_ * numInputLinks_  );
  Stringss fwInStrings ( numRegions_ * numInputLinks_  );
  Stringss swOutStrings( numRegions_ * numOutputLinks_ );
  Stringss fwOutStrings( numRegions_ * numOutputLinks_ );
  Stringss dummyStrings( numRegions_ * numOutputLinks_ );

  predict( swInStrings,  fwInStrings, stageIn_ );

  write( swInStrings, fwInStrings );

  system( cmd_.str().c_str() );

  predict( swOutStrings, dummyStrings, stageOut_ );
  read( fwOutStrings );

  compare( swOutStrings, fwOutStrings );

}


void Demo::write( Stringss& swStrings, Stringss& fwStrings ) {

  stringstream ss;
  ss << "Board CMSSW" << endl << " Quad/Chan :";
  for ( const int& link : inputLinks_ )
    ss << "        q" << setfill('0') << setw( 2 ) << link / 4 << "c" << link % 4 << "      ";
  ss << endl << "      Link :";
  for ( const int& link : inputLinks_ )
    ss << "         " << setfill('0') << setw( 2 ) << link << "        ";
  ss << endl;
  int frame( 0 );
  for ( int region = 0; region < numRegions_ + 1; region++ ) {
    for ( int gap = 0; gap < numFramesInfra_; gap++ ) {
      ss << "Frame " << setfill('0') << setw( 4 ) << frame++ << " :";
      for ( int link = 0; link < numInputLinks_; link++ )
        ss << " 0v" << setfill('0') << setw( widthFrame_ / 4 ) << 0;
      ss << endl;
    }
    for ( int payload = 0; payload < numPayloadFrames_; payload++ ) {
      ss << "Frame " << setfill('0') << setw( 4 ) << frame << " :";
      for ( int link = 0; link < numInputLinks_; link++ ) {
        if ( region < numRegions_ )
          ss << " 1v" << fwStrings[ region * numInputLinks_ + link ][ payload ];
        else
          ss << " 1v" << setfill('0') << setw( widthFrame_ / 4 ) << 0;
      }
      ss << endl;
      frame++;
    }
  }

  fstream fs;
  fs.open( "in.txt", fstream::out );
  fs << ss.rdbuf();
  fs.close();

  for ( int region = 0; region < numRegions_; region++ ) {
    debugStream_ << "region " << region << endl;
    int maxPos( 0 );
    for ( int link = 0; link < numInputLinks_; link++ )
      maxPos = max( maxPos, (int)swStrings[ region * numInputLinks_ + link ].size() );
    debugStream_ << string( 6, ' ' );
    for ( int link = 0; link < numInputLinks_; link++ )
      debugStream_ << "Link " << setw(2) << link << string( lenInputString_ - 1, ' ' );
    debugStream_ << endl;
    for ( int pos = 0; pos < maxPos; pos++ ) {
      debugStream_ << setw( 3 ) << pos << " : ";
      for ( int link = 0; link < numInputLinks_; link++ ) {
        Strings& strings = swStrings[ region * numInputLinks_ + link ];
        if ( (int)strings.size() > pos )
          debugStream_ << strings[ pos ] << "  ||  ";
        else
          debugStream_ << string( lenInputString_, ' ' ) << "  ||  ";
      }
      debugStream_ << endl;
    }
  }

}

void Demo::compare( const Stringss& swStrings, const Stringss& fwStrings ) {

  bool bug( false );
  for ( int region = 0; region < numRegions_; region++ ) {
    int first( 0 );
    for ( int link = 0; link < numOutputLinks_; link++ ) {
      const int      id     = region * numOutputLinks_ + link;
      const Strings& sws    = swStrings[ id ];
      const Strings& fws    = fwStrings[ id ];
      const int      maxPos = max( (int)sws.size(), (int)fws.size() );
      if ( maxPos > 0 ) {
        if ( !first++ )
          debugStream_ << "region " << region << endl;
        debugStream_ << "link " << link << endl;
      }
      for ( int pos = 0; pos < maxPos; pos++ ) {
        debugStream_ << setw( 3 ) << pos << " : ";
        const string sw = ( (int)sws.size() > pos ? sws[ pos ] : string( lenOutputString_, ' ' ) );
        const string fw = ( (int)fws.size() > pos ? fws[ pos ] : string( lenOutputString_, ' ' ) );
        debugStream_ << sw << " || " << fw << ( sw == fw ? "  " : " *" ) << endl;
        if ( sw != fw )
          bug = true;
      }
    }
  }

  fstream fs;
  fs.open( "debug.txt", fstream::out );
  fs << debugStream_.rdbuf();
  fs.close();
  if ( bug )
    //throw cms::Exception( "..." );
    cout << "bug" << endl;  

}

void Demo::read( Stringss& strings ) const {

  stringstream ss;
  fstream fs;
  fs.open( "out.txt", fstream::in );
  ss << fs.rdbuf();
  fs.close();

  string token;
  for ( int n = 0; n < 3; n++ )
    getline( ss, token, '\n' );

  for ( int region = 0; region < numRegions_; region++ ) {
    for ( int payload = 0; payload < numPayloadFrames_; payload++ )
      for ( int link = 0; link < numOutputLinks_; link++ ) {
        Strings& stream = strings[ region * numOutputLinks_ + link ];
        do
          getline( ss, token, 'v' );
        while ( token.back() == '0' );
        getline( ss, token, ' ' );
        strings[ region * numOutputLinks_ + link ].push_back( read( token, stageOut_ ) );
      }
    for ( int n = 0; n < numFramesInfra_; n++ )
      getline( ss, token, '\n' );
  }
  for ( Strings& stream : strings )
    for( auto it = stream.end(); it != stream.begin(); )
      it = (*--it) == string( lenOutputString_, ' ' ) ? stream.erase( it ) : stream.begin();

}


string Demo::read( const string& token, const string& type ) const {

  const unsigned long long int hex = strtoull( token.c_str(), nullptr, 16 );
  SLV stub( hex, widthStub_ );

  string s;
  if ( type == "GP"  )
    s = readGP ( stub );
  if ( type == "HT"  )
    s = readHT ( stub );
  if ( type == "MHT" ) 
    s = readMHT( stub );

  return s;

}


void Demo::predict( Stringss& swStrings, Stringss& fwStrings, const string& type ) const {

  if ( type == "DTC" )
    predictDTC( swStrings, fwStrings );
  if ( type == "GP"  )
    predictGP ( swStrings, fwStrings );
  if ( type == "HT"  )
    predictHT ( swStrings, fwStrings );
  if ( type == "MHT" )
    predictMHT( swStrings, fwStrings );

  for ( Strings& stream : fwStrings )
    stream.resize( numPayloadFrames_, string( widthFrame_ / 4, '0' ) );
  for ( Strings& stream : swStrings )
    stream.resize( min( (int)stream.size(), numPayloadFrames_ ) );

}


void Demo::predictDTC( Stringss& swStrings, Stringss& fwStrings ) const {

  const Stubs& stubs = data_->stubs();

  Stubss stubss( numRegions_ * numLinksDTC_ );
  for ( Stub* stub: stubs )
    for( const int& id :stub->dtcStreamIds() )
      stubss[ id ].push_back( stub );

  for ( int id = 0; id < numRegions_ * numLinksDTC_; id++ ) {
    const int region = id / numLinksDTC_;
    const int link   = id % numLinksDTC_;
    Stubs&    stubs  = stubss[ id ];
    sort( stubs.begin(), stubs.end(), [id]( Stub* lhs, Stub* rhs ) { return lhs->dtcStreamPos( id ) < rhs->dtcStreamPos( id ); } );
    for ( int n = 0; n < (int)stubs.size(); n++ ) {
      const int& pos = stubs[ n ]->dtcStreamPos( id );
      stubs.insert( stubs.begin() + n, pos - n, nullptr );
      n = pos;
    }
    stubs.resize( min( (int)stubs.size(), numPayloadFrames_) );
    for ( const Stub* stub : stubs ) {

      stringstream fw;
      stringstream sw;
      if ( stub == nullptr ) {
        sw << string( dtcLenString_, ' ' );
        fw << string( widthFrame_ / 4, '0' );
      } else {

        const double r    = stub->r() - chosenRofPhi_;
        const double phi  = reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, 0. );
        const double z    = stub->z();
        const double bend = stub->bend();

        const int gap = widthFrame_ - 1 - widthBend_ - widthZ_ - widthR_ - widthPhi_ - 1;
        fw << string( gap, '0' );
        fw << '1';
        fw << SLV( bend, baseBend_, widthBend_,    true ).str();
        fw << SLV( z,    baseZ_,    widthZ_,       true ).str();
        fw << SLV( r,    baseR_,    widthR_,       true ).str();
        fw << SLV( phi,  basePhi_,  widthPhi_ + 1, true ).str();

        sw << setw( 5 ) << digii( r,    baseR_    )                  << " ";
        sw << setw( 5 ) << digii( phi,  basePhi_  )                  << " ";
        sw << setw( 5 ) << digii( z,    baseZ_    )                  << " ";
        sw << setw( 3 ) << digii( bend, baseBend_ )                  << " | ";
        sw << setw( 1 ) << stub->layerId()                           << " | ";
        sw << setw( 2 ) << stub->rangePt().first  - htNumBinsPt_ / 2 << " ";
        sw << setw( 2 ) << stub->rangePt().second - htNumBinsPt_ / 2 << " | ";
        sw << setw( 2 ) << stub->rangePhi().first                    << " ";
        sw << setw( 2 ) << stub->rangePhi().second                   << " | ";
        sw << setw( 2 ) << stub->rangeEta().first                    << " ";
        sw << setw( 2 ) << stub->rangeEta().second;

      }

      stringstream hexStream;
      unsigned long int val = strtoul ( fw.str().c_str(), nullptr, 2 );
      hexStream << setfill('0') << setw( widthFrame_ / 4 ) << hex << val;
      fwStrings[ id ].push_back( hexStream.str() );
      swStrings[ id ].push_back( sw.str() );

    }

  }

}

void Demo::predictGP( Stringss& swStrings, Stringss& fwStrings ) const {

  const Stubs& stubs = data_->stubs();

  Stubss stubss( numRegions_ * numLinksGP_ );
  for ( Stub* stub: stubs )
    for( const int& id :stub->gpStreamIds() )
      stubss[ id ].push_back( stub );

  for ( int id = 0; id < numRegions_ * numLinksGP_; id++ ) {
    const int region   = id / numLinksGP_;
    const int link     = id % numLinksGP_;
    const int secPhi   = link / numSectorsEta_;
    const int secEta   = link % numSectorsEta_;
    Stubs&    stubs    = stubss   [ id ];
    Strings&  fwStream = fwStrings[ id ];
    Strings&  swStream = swStrings[ id ];
    sort( stubs.begin(), stubs.end(), [id]( Stub* lhs, Stub* rhs ) { return lhs->gpStreamPos( id ) < rhs->gpStreamPos( id ); } );
    for ( int n = 0; n < (int)stubs.size(); n++ ) {
      const int& pos = stubs[ n ]->gpStreamPos( id );
      stubs.insert( stubs.begin() + n, pos - n, nullptr );
      n = pos;
    }
    stubs.resize( min( (int)stubs.size(), numPayloadFrames_ ) );
    for ( const Stub* stub : stubs ) {

      stringstream fw;
      stringstream sw;
      if ( stub == nullptr ) {
        fwStream.push_back( string( widthFrame_ / 4, '0' ) );
        swStream.push_back( string( gpLenString_,    ' ' ) );
      } else {

        const Range& rangePt = stub->rangePt();
        const int    r       = digii( stub->r() - chosenRofPhi_, baseR_ );
        const int    phi     = digii( digif( reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ), basePhi_ ) - ( secPhi - .5 ) * baseSector_, basePhi_ );
        const int    z       = digii( digif( stub->z(), baseZ_ ) - ( r + digii( chosenRofPhi_, baseR_ ) + 1 ) * baseR_ * digif( sectorsCot_[ secEta ], baseCotTheta_ ), baseZ_ );
        const int    layer   = stub->layerId();
        const int    ptMin   = rangePt.first  - htNumBinsPt_ / 2;
        const int    ptMax   = rangePt.second - htNumBinsPt_ / 2;

        const int gap = widthFrame_ - 1 - widthR_ - widthPhi_ - widthChiZ_ - widthLayer_ - widthMBin_ - widthMBin_;
        fw << string( gap, '0' );
        fw << '1';
        fw << SLV( r,    baseR_,    widthR_,    true ).str();
        fw << SLV( phi,  basePhi_,  widthPhi_ , true ).str();
        fw << SLV( z,    baseZ_,    widthChiZ_, true ).str();
        fw << SLV( layer,           widthLayer_      ).str();
        fw << SLV( ptMin,           widthMBin_, true ).str();
        fw << SLV( ptMax,           widthMBin_, true ).str();

        stringstream hexStream;
        unsigned long int val = strtoul ( fw.str().c_str(), nullptr, 2 );
        hexStream << setfill('0') << setw( widthFrame_ / 4 ) << hex << val;
        fwStream.push_back( hexStream.str() );
        swStream.push_back( readGP( ptMax, ptMin, layer, z, phi, r ) );

      }

    }

  }

}

void Demo::predictHT( Stringss& swStrings, Stringss& fwStrings ) const {

  const Tracks& tracks = data_->tracksHT();

  Trackss trackStreams( numRegions_ * numLinksHT_ );
  for ( Track* track : tracks )
    trackStreams[ track->streamId() ].push_back( track );

  for ( int id = 0; id < numRegions_ * numLinksHT_; id++ ) {
    Tracks&   trackStream = trackStreams[ id ];
    const int region      = id / numLinksHT_;
    const int link        = id % numLinksHT_;
    sort( trackStream.begin(), trackStream.end(), []( Track* lhs, Track* rhs ){ return lhs->streamPos() < rhs->streamPos(); } );
    for ( Track* track : trackStream ) {

      const int&   binPt  = track->binPt();
      const int&   binPhi = track->binPhi();
      const int&   secEta = track->secEta();
      const int&   secPhi = track->secPhi();
      const Stubs& stubs = track->stubs();
      for ( Stub* stub : stubs ) {

        const int r     = digii( stub->r() - chosenRofPhi_, baseR_ );
        const int phi   = digii( digif( reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ), basePhi_ ) - ( secPhi - .5 ) * baseSector_ - ( binPhi + .5 ) * baseC_ + ( binPt +.5 ) * baseM_ * ( r + .5 ) * baseR_, basePhi_ );
        const int z     = digii( digif( stub->z(), baseZ_ ) - ( r + digii( chosenRofPhi_, baseR_ ) + 1 ) * baseR_ * digif( sectorsCot_[ secEta ], baseCotTheta_ ), baseZ_ );
        const int layer = stub->layerId() + 1;
        const int mSel  = binPt >= 0;

        stringstream fw;
        const int gap = widthFrame_ - 1 - 1 - widthR_ - widthChiPhi_ - widthChiZ_ - widthCBin_ - widthEta_ - widthLayer_;
        fw << string( gap, '0' );
        fw << '1';
        fw << to_string( mSel );
        fw << SLV( r,      widthR_,      true ).str();
        fw << SLV( phi,    widthChiPhi_, true ).str();
        fw << SLV( z,      widthChiZ_,   true ).str();
        fw << SLV( binPhi, widthCBin_,   true ).str();
        fw << SLV( secEta, widthEta_          ).str();
        fw << SLV( layer,  widthLayer_        ).str();

        stringstream hexStream;
        unsigned long int val = strtoul ( fw.str().c_str(), nullptr, 2 );
        hexStream << setfill('0') << setw( widthFrame_ / 4 ) << hex << val;
        fwStrings[ id ].push_back( hexStream.str() );
        swStrings[ id ].push_back( readHT( layer, secEta, binPhi, z, phi, r, mSel ) );

      }

    }

  }

}

void Demo::predictMHT( Stringss& swStrings, Stringss& fwStrings ) const {

  const Tracks& tracks = data_->tracksMHT();

  Trackss trackStreams( numRegions_ * numLinksMHT_ );
  for ( Track* track : tracks )
    trackStreams[ track->streamId() ].push_back( track );

  for ( int id = 0; id < numRegions_ * numLinksMHT_; id++ ) {
    Strings&  fwStream    = fwStrings   [ id ];
    Strings&  swStream    = swStrings   [ id ];
    Tracks&   trackStream = trackStreams[ id ];
    const int region      = id / numLinksMHT_;
    const int link        = id % numLinksMHT_;
    sort( trackStream.begin(), trackStream.end(), []( Track* lhs, Track* rhs ){ return lhs->streamPos() < rhs->streamPos(); } );
    int pos( 0 );
    Tracks inputStream;
    for ( Track* track : trackStream ) {
      const int& startPos = track->streamPos();
      const int& size     = track->size();
      if ( pos < startPos ) {
        inputStream.insert( inputStream.end(), startPos - pos, nullptr );
        pos = startPos;
      }
      pos += size;
      inputStream.push_back( track );
    }
    for ( Track* track : inputStream ) {

      if ( track == nullptr ) {
        fwStream.push_back( string( widthFrame_ / 4, '0' ) );
        swStream.push_back( string( mhtLenString_,   ' ' ) );
      } else {
        const int&   binPt  = track->binPt();
        const int&   binPhi = track->binPhi();
        const int&   secEta = track->secEta();
        const int&   secPhi = track->secPhi();
        const Stubs& stubs  = track->stubs();
        for ( Stub* stub : stubs ) {

          const int r     = digii( stub->r() - chosenRofPhi_, baseR_ );
          const int phi   = digii( digif( reco::deltaPhi( stub->phi() - ( region + 1 ) * baseRegion_, .0 ), basePhi_ ) - ( secPhi - .5 ) * baseSector_, basePhi_ );
          const int z     = digii( stub->z(), baseZ_ );
          const int layer = stub->layerId() + 1;

          stringstream fw;
          const int gap = widthFrame_ - 1 - 1 - widthR_ - widthPhi_ - widthZ_ - widthLayer_ - widthEta_ - widthCBin_ - widthMBin_;
          fw << string( gap, '0' );
          fw << '1';
          fw << SLV( r,      widthR_,      true ).str();
          fw << SLV( phi,    widthChiPhi_, true ).str();
          fw << SLV( z,      widthChiZ_,   true ).str();
          fw << SLV( layer,  widthLayer_        ).str();
          fw << SLV( secEta, widthEta_          ).str();
          fw << SLV( binPhi, widthCBin_,   true ).str();
          fw << SLV( binPt,  widthCBin_,   true ).str();

          stringstream hexStream;
          unsigned long int val = strtoul ( fw.str().c_str(), nullptr, 2 );
          hexStream << setfill('0') << setw( widthFrame_ / 4 ) << hex << val;
          fwStream.push_back( hexStream.str() );  
          swStream.push_back( readMHT( phi, r, secEta, z, layer, binPt, binPhi ) );

        }

      }
    }

  }

}

string Demo::readGP( SLV& stub ) const {

  const bool valid = stub.msb();
  if ( !valid )
    return string( gpLenString_, ' ' );

  const int ptMax  = stub.val( widthMBin_, 0, true ); stub >>= widthMBin_;
  const int ptMin  = stub.val( widthMBin_, 0, true ); stub >>= widthMBin_;
  const int layer  = stub.val( widthLayer_ );         stub >>= widthLayer_;
  const int z      = stub.val( widthChiZ_, 0, true ); stub >>= widthChiZ_;
  const int phi    = stub.val( widthPhi_,  0, true ); stub >>= widthPhi_;
  const int r      = stub.val( widthR_,    0, true );

  return readGP( ptMax, ptMin, layer, z, phi, r );

}


string Demo::readGP( const int& ptMax, const int& ptMin, const int& layer, const int& z, const int& phi, const int& r ) const {

  stringstream ss;
  ss << setw( 5 ) << r     << " ";
  ss << setw( 5 ) << phi   << " ";
  ss << setw( 5 ) << z     << " | ";
  ss << setw( 1 ) << layer << " | ";
  ss << setw( 2 ) << ptMin << " ";
  ss << setw( 2 ) << ptMax;
  return ss.str();

}


string Demo::readHT( SLV& stub ) const {

  const bool valid = stub.msb();
  if ( !valid )
    return string( htLenString_, ' ' );

  const int layer  = stub.val( widthLayer_ );           stub >>= widthLayer_;
  const int secEta = stub.val( widthEta_   );           stub >>= widthEta_;
  const int binPhi = stub.val( widthCBin_,   0, true ); stub >>= widthCBin_;
  const int z      = stub.val( widthChiZ_,   0, true ); stub >>= widthChiZ_;
  const int phi    = stub.val( widthChiPhi_, 0, true ); stub >>= widthChiPhi_;
  const int r      = stub.val( widthR_,      0, true ); stub >>= widthR_;
  const int mSel   = stub.msb();

  return readHT( layer, secEta, binPhi, z, phi, r, mSel );

}


string Demo::readHT( const int& layer, const int& secEta, const int& binPhi, const int& z, const int& phi, const int& r, const int& mSel ) const {

  stringstream ss;
  ss << setw( 5 ) << r      << " ";
  ss << setw( 4 ) << phi    << " ";
  ss << setw( 5 ) << z      << " | ";
  ss << setw( 2 ) << secEta << " ";
  ss << setw( 3 ) << binPhi << " ";
  ss << setw( 1 ) << layer;
  return ss.str();

}


string Demo::readMHT( SLV& stub ) const {

  const bool valid = stub.msb();
  if ( !valid )
    return string( mhtLenString_, ' ' );

  const int phi    = stub.val( widthPhi_,    0, true ); stub >>= widthPhi_;
  const int r      = stub.val( widthR_,      0, true ); stub >>= widthR_;
  const int secEta = stub.val( widthEta_   );           stub >>= widthEta_;
  const int z      = stub.val( widthZ_,      0, true ); stub >>= widthZ_;
  const int layer  = stub.val( widthLayer_ );           stub >>= widthLayer_;
  const int binPt  = stub.val( widthBinPt_,  0, true ); stub >>= widthBinPt_;
  const int binPhi = stub.val( widthBinPhi_, 0, true ); stub >>= widthBinPhi_;

  return readMHT( phi, r, secEta, z, layer, binPt, binPhi );

}


string Demo::readMHT( const int& phi, const int& r, const int& secEta, const int& z, const int& layer, const int& binPt, const int& binPhi ) const {

  stringstream ss;
  ss << setw( 5 ) << r      << " ";
  ss << setw( 5 ) << phi    << " ";
  ss << setw( 5 ) << z      << " | ";
  ss << setw( 1 ) << layer  << " ";
  ss << setw( 2 ) << secEta << " | ";
  ss << setw( 3 ) << binPt  << " ";
  ss << setw( 3 ) << binPhi;
  return ss.str();

}

}