#include "L1Trigger/TrackFindingTMTT/interface/Converter.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/TP.h"
#include "L1Trigger/TrackFindingTMTT/interface/Module.h"

#include <fstream>
#include <sstream>

using namespace std;

namespace TMTT {

Converter::Converter( const Settings*                                       settings,
                            Data*                                           data,
                      const edm::EDGetTokenT< DetSetVec >&                  stubInputTag,
                      const edm::EDGetTokenT< TrackingParticleCollection >& tpInputTag,
                      const edm::EDGetTokenT< TTClusterAssMap >&            clusterTruthInputTag,
                      const edm::EDGetTokenT< reco::GenJetCollection >&     genJetInputTag        ) :
    settings_            ( settings               ),
    data_                ( data                   ),
    useMCtruth_          ( settings->useMCtruth() ),
    stubInputTag_        ( stubInputTag           ),
    tpInputTag_          ( tpInputTag             ),
    clusterTruthInputTag_( clusterTruthInputTag   ),
    genJetInputTag_      ( genJetInputTag         )
        
{

  // configuring thight track particle selector
  double       ptMin       = settings->tpMinPt();
  const double ptMax       = 9999999999.;
  double       etaMax      = settings->tpMaxEta();
  double       tip         = settings->tpMaxVertR();
  double       lip         = settings->tpMaxVertZ();
  const int    minHit      = 0;
  bool         signalOnly  = true;
  bool         intimeOnly  = true;
  const bool   chargedOnly = true;
  const bool   stableOnly  = false;
  tightSelector_ = new TrackingParticleSelector( ptMin , ptMax, -etaMax, etaMax, tip, lip, minHit, signalOnly, intimeOnly, chargedOnly, stableOnly );

  // configuring loose track particle selector
  ptMin          = min( ptMin , 0.7 * settings->stubMinPt()  );
  etaMax         = max( etaMax, 0.2 + settings->stubMaxEta() );
  tip            = max( tip   , 10. );
  lip            = max( lip   , 35. );
  signalOnly     = false;
  intimeOnly     = false;
  looseSelector_ = new TrackingParticleSelector( ptMin, ptMax, -etaMax, etaMax, tip, lip, minHit, signalOnly, intimeOnly, chargedOnly, stableOnly );

  // read in cabling map provided by tk layout
  stringstream ss;
  fstream fs;
  fs.open( "L1Trigger/TrackFindingTMTT/test/Cabling/ModulesToDTCsNegOuter.csv", fstream::in );
  ss << fs.rdbuf();
  fs.close();
  fs.open( "L1Trigger/TrackFindingTMTT/test/Cabling/ModulesToDTCsPosOuter.csv", fstream::in );
  ss << fs.rdbuf();
  fs.close();

  const int& numModules       = settings->trackerNumModules();
  const int& num2SPerRegion   = settings->dtcNum2S();
  const int& numDTCsPerRegion = settings->dtcNumPerRegion();

  // converting cabling map txt files into handy object
  cablingMap_.reserve( numModules );
  string token;
  for ( int module = 0; module < numModules; module++ ) {
    const bool side = module < numModules / 2;
    getline( ss, token, '\n' );
    getline( ss, token, ',' );
    if ( module == numModules / 2 ) {
      getline( ss, token, '\n' );
      getline( ss, token, ',' );
    }
    const unsigned int detId = stoul( token, nullptr, 10 );
    for ( int n = 0; n < 12; n++ )
      getline( ss, token, ',' );
    const int region = stoul( token, nullptr, 10 );
    getline( ss, token, ',' );
    const string type = token;
    getline( ss, token, ',' );
    const int slot = stoul( token, nullptr, 10 );
    int dtcId = slot - 1;
    if ( type == "2S" )
      dtcId += num2SPerRegion / 2;
    if ( !side )
      dtcId += numDTCsPerRegion / 2;
    dtcId  += region * numDTCsPerRegion;
    cablingMap_[ detId ] = dtcId;
  }

}


Converter::~Converter() {

  delete tightSelector_;
  delete looseSelector_;

}

void Converter::consume( const edm::EventSetup& iSetup ) const {

  // read in detector parameter
  edm::ESHandle< TrackerGeometry > trackerGeometryHandle;
  iSetup.get< TrackerDigiGeometryRecord >().get( trackerGeometryHandle );
  const TrackerGeometry* trackerGeometry = trackerGeometryHandle.product();

  edm::ESHandle< TrackerTopology > trackerTopologyHandle;
  iSetup.get< TrackerTopologyRcd >().get( trackerTopologyHandle );
  const TrackerTopology* trackerTopology = trackerTopologyHandle.product();

  edm::ESHandle<MagneticField> magneticFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
  const MagneticField* magneticField = magneticFieldHandle.product();

  // check magnetic field
  const double   mf = magneticField->inTesla( GlobalPoint( 0, 0, 0 ) ).z();
  const double& smf = settings_->magneticField();
  if ( fabs( smf - mf ) > 1e-4 )
    throw cms::Exception( "Tracker::consume magneticField config conflict (" + to_string( smf ) + " vs. " + to_string( mf ) + ")." );

  // create sensor modules objects for outer tracker
  Modules& modules = data_->modules();

  for( const auto& detId : trackerGeometry->detIds() ) { // all tracker modules
    if ( detId.subdetId() == 1 || detId.subdetId() == 2 ) // skip pixel detector
      continue;
    if ( !trackerTopology->isLower( detId ) )
      continue;
    const int& dtcId = cablingMap_.at( detId.rawId() - 1 );
    modules.insert( make_pair( detId.rawId(), new Module( settings_, trackerGeometry, trackerTopology, detId, dtcId ) ) );
  }

}


void Converter::consume( const edm::Event& iEvent ) const {

  // read in stubs
  edm::Handle< DetSetVec > ttStubHandle;
  iEvent.getByToken( stubInputTag_, ttStubHandle );

  const Modules& modules = data_->modules();
        Stubs&   stubs   = data_->stubs();

  // create Stubs
  for ( DetSetVec::const_iterator ttModule = ttStubHandle->begin(); ttModule != ttStubHandle->end(); ttModule++ ) { // outer tracker modules
    int n( 0 );
    Module* module = nullptr;
    for ( DetSet::const_iterator stub = ttModule->begin(); stub != ttModule->end(); stub++ ) { // TTStubs from one module
      const TTStubRef& ttStubRef = edmNew::makeRefTo( ttStubHandle, stub );
      if ( n++ == 0 ) // pick module object once per module
        module = modules.at( ttStubRef->getDetId().rawId() + 1 );
      stubs.push_back( new Stub( settings_, module, ttStubRef ) );
    }
  }

  // read in MCTruth
  if ( useMCtruth_ ) {

    edm::Handle< TrackingParticleCollection > tpHandle;
    iEvent.getByToken( tpInputTag_, tpHandle );

    edm::Handle< TTClusterAssMap > mcTruthTTClusterHandle;
    iEvent.getByToken( clusterTruthInputTag_, mcTruthTTClusterHandle );
    const TTClusterAssMap* clusterAssociation = mcTruthTTClusterHandle.product();

    edm::Handle< reco::GenJetCollection > jetHandle;
    iEvent.getByToken( genJetInputTag_, jetHandle );
    const reco::GenJetCollection* jetCollection = jetHandle.product();

    TPs&   tps   = data_->tps();

    // create jets
    const double minPt  = 30.;
    const double maxEta = 2.5;
    vector< const reco::GenJet* > jets;
    for ( const auto& jet : *jetCollection )
      if ( jet.pt() > minPt && fabs( jet.eta() ) < maxEta )
        jets.push_back( &jet );

    // create tracking particles
    for ( unsigned int n = 0; n < tpHandle->size(); n++ ) {
      const TrackingParticle& tp = tpHandle->at( n );
      if ( !looseSelector_->operator()( tp ) )
        continue;
      const bool tight = tightSelector_->operator()( tp );
      TrackingParticlePtr tpPtr( tpHandle, n );
      tps.push_back( new TP( settings_, tpPtr, tight, jets ) );
    }

    // associate stubs with tracking particles and vice versa
    for ( Stub* stub : stubs ) {

      vector< TrackingParticlePtr > tpPtrs;
      for ( unsigned int iClus = 0; iClus < 2; iClus++ ) {
        const vector< TrackingParticlePtr >& assocPtrs = clusterAssociation->findTrackingParticlePtrs( stub->ttRef()->getClusterRef( iClus ) );
        tpPtrs.insert( tpPtrs.end(), assocPtrs.begin(), assocPtrs.end() );
      }
      sort( tpPtrs.begin(), tpPtrs.end() );
      tpPtrs.erase( unique( tpPtrs.begin(), tpPtrs.end() ), tpPtrs.end() );

      vector< TP* > assocTPs;
      for ( TrackingParticlePtr tpPtr : tpPtrs ) {
        auto it = find_if( tps.begin(), tps.end(), [tpPtr]( TP* tp ) { return tp->edmPtr() == tpPtr; } );
        if ( it != tps.end() )
          assocTPs.push_back( *it );
      }
      for ( TP* tp : assocTPs )
        tp->assoc( stub );
      stub->assoc( assocTPs );

    }

    for ( TP* tp : tps )
      tp->assocComplete();

  }

}


void Converter::put( edm::Event& iEvent ) const {

  const Tracks& tracks = data_->tracksKF();

  // convert Tracks into TTTracks
  TTTrackCollection ttTracks;
  for ( Track* track : tracks ) {

    if ( !track->valid() )
      continue;

    const Stubs stubs = track->stubs();
    vector< TTStubRef > ttStubRefs;
    for ( Stub* stub : stubs )
      ttStubRefs.push_back( stub->ttRef() );

    const float& invPtToDphi = settings_  ->invPtToDphi();
    const int&   sectorPhi   = track->secPhi();
    const int&   sectorEta   = track->secEta();
    const int    nPar        = 4;
    const float& d0          = 0.;
    const float& phi0        = track->phi();
    const float& z0          = track->z();
    const float& qOverPt     = track->qOverPt();
    const float  pt          = 1. / qOverPt;
    const float& cot         = track->cot();
    const float& chi2        = track->chi2();
    const float  invR        = -2. * invPtToDphi * qOverPt;
    const GlobalPoint  beamSpot( d0 * sin( phi0 ), d0 * cos( phi0 ), z0 );
    const GlobalVector momentum( GlobalVector::Cylindrical( pt, phi0, pt * cot ) );

    TTTrack< Ref_Phase2TrackerDigi_ > ttTrack( ttStubRefs );
    ttTrack.setSector           ( sectorPhi );
    ttTrack.setWedge            ( sectorEta );
    ttTrack.setPOCA             ( beamSpot, nPar );
    ttTrack.setMomentum         ( momentum, nPar );
    ttTrack.setRInv             ( invR    , nPar );
    ttTrack.setChi2             ( chi2    , nPar );
    ttTrack.setStubPtConsistency( -1      , nPar );

    ttTracks.push_back( ttTrack );

  }
  ttTracks.shrink_to_fit();

  // store TTTracks in ED product
  iEvent.put( move( make_unique< TTTrackCollection >( ttTracks ) ) , settings_->outputString() );

}


}