#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducer.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Converter.h"
#include "L1Trigger/TrackFindingTMTT/interface/DTC.h"
#include "L1Trigger/TrackFindingTMTT/interface/GP.h"
#include "L1Trigger/TrackFindingTMTT/interface/HT.h"
#include "L1Trigger/TrackFindingTMTT/interface/MHT.h"
#include "L1Trigger/TrackFindingTMTT/interface/LR.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS.h"
#include "L1Trigger/TrackFindingTMTT/interface/KF.h"
#include "L1Trigger/TrackFindingTMTT/interface/DR.h"
#include "L1Trigger/TrackFindingTMTT/interface/Demo.h"
#include "L1Trigger/TrackFindingTMTT/interface/Histos.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

using namespace std;

namespace TMTT {

TMTrackProducer::TMTrackProducer( const edm::ParameterSet& iConfig ) {

  // helper classes to manage configurations
  settings_  = new Settings ( iConfig );

  // specification of ED input products
  edm::EDGetTokenT< DetSetVec >                  stubInputTag;
  edm::EDGetTokenT< TrackingParticleCollection > tpInputTag;
  edm::EDGetTokenT< TTClusterAssMap >            clusterTruthInputTag;
  edm::EDGetTokenT< reco::GenJetCollection >     genJetInputTag;

  stubInputTag           = consumes< DetSetVec >                 ( settings_->inputTagStub()         );
  if ( settings_->useMCtruth() ) {
    tpInputTag           = consumes< TrackingParticleCollection >( settings_->inputTagTP()           );
    clusterTruthInputTag = consumes< TTClusterAssMap >           ( settings_->inputTagClusterTruth() );
    genJetInputTag       = consumes< reco::GenJetCollection >    ( settings_->inputTagGenJet()       );
  }

  // helper classes to manage and convert event data
  data_      = new Data     ( settings_        );
  converter_ = new Converter( settings_, data_, stubInputTag, tpInputTag, clusterTruthInputTag, genJetInputTag );

  // track reconstruction algorithm blocks
  dtc_       = new DTC   ( settings_, data_ );
  gp_        = new GP    ( settings_, data_ );
  ht_        = new HT    ( settings_, data_ );
  mht_       = new MHT   ( settings_, data_ );
  lr_        = new LR    ( settings_, data_ );
  lrhls_     = new LRHLS ( settings_, data_ );
  kf_        = new KF    ( settings_, data_ );
  dr_        = new DR    ( settings_, data_ );

  // helper classes to analyse algorithms and to compare with f/w
  demo_      = new Demo  ( settings_, data_ );
  histos_    = new Histos( settings_, data_ );

  // book ED output product
  const string outputString = settings_->outputString();
  produces< TTTrackCollection >( outputString ).setBranchAlias( outputString );

  cout.setf( ios::fixed, ios::floatfield );
  cout.precision( 4 );

}


TMTrackProducer::~TMTrackProducer(){

//  delete settings_;
//  delete data_;
//  delete converter_;
//  delete dtc_;
//  delete gp_;
//  delete ht_;
//  delete mht_;
//  delete lr_;
//  delete lrhls_;
//  delete kf_;
//  delete dr_;
//  delete demo_;
//  delete histos_;

}


void TMTrackProducer::beginRun( const edm::Run& iRun, const edm::EventSetup& iSetup ) {

  // read in detector parameter
  converter_->consume( iSetup );

}


void TMTrackProducer::produce( edm::Event& iEvent, const edm::EventSetup& iSetup ) {

  // delete last event data
  data_->clear();

  // read in and convert event content
  converter_->consume( iEvent );

  // route Stubs into DTC channels
  dtc_->produce();

  // route stubs into spatial sectors
  gp_->produce();

  // route stubs into r-phi track candidates
  ht_->produce();

  // route stubs into finer r-phi track candidates
  mht_->produce();

  // fit based on Linear Regression
  lr_->produce();

  // fit based on HLS impl of Linear Regression
  lrhls_->produce();

  // fit based on Kalman Filter
  kf_->produce();

  // remove duplicates
  dr_->produce();

  // convert and read out fitted tracks in ED product
  converter_->put( iEvent );

  // analyze algorithms
  histos_->analyze();

  // play data through modelsim and compare with sw
//  demo_->analyze();

}

void TMTrackProducer::endJob() {

  // print out brief performance report
  histos_->endJob();

}


DEFINE_FWK_MODULE( TMTrackProducer );

}