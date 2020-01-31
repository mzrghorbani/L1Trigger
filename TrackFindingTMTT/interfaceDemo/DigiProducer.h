#ifndef __DEMONSTRATOR_PRODUCER_DIGIPRODUCER_HPP__
#define __DEMONSTRATOR_PRODUCER_DIGIPRODUCER_HPP__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/DTC.h"
#include "L1Trigger/TrackFindingTMTT/interface/GP.h"
#include "L1Trigger/TrackFindingTMTT/interface/HT.h"
#include "L1Trigger/TrackFindingTMTT/interface/MHT.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"
#include "L1Trigger/TrackFindingTMTT/interface/Module.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"

#include "L1Trigger/TrackFindingTMTT/interfaceDemo/DigiConverter.h"


namespace demo {


class DigiProducer : public edm::EDProducer {

public:

  explicit DigiProducer( const edm::ParameterSet& );

  ~DigiProducer();

private:

  virtual void beginRun( const edm::Run&, const edm::EventSetup& );

  virtual void produce( edm::Event&, const edm::EventSetup& );

  virtual void endJob(){}

private:

  edm::EDGetTokenT< TMTT::DetSetVec > stubInputTag_;

  TMTT::Settings*  settings_;
  TMTT::Data*      data_;

  TMTT::DTC* dtc_;
  TMTT::GP*  gp_;
  TMTT::HT*  ht_;
  TMTT::MHT* mht_;

  DigiConverter* digiConverter_;

  TMTT::CablingMap cablingMap_;

};

}

#endif

