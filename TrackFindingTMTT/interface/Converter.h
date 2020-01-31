#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

class Settings;

/**
 * Class to convert ED products into handier objects and back
 */
class Converter {

public:

  Converter(  const Settings*                                       settings,
                    Data*                                           data,
              const edm::EDGetTokenT< DetSetVec >&                  stubInputTag,
              const edm::EDGetTokenT< TrackingParticleCollection >& tpInputTag,
              const edm::EDGetTokenT< TTClusterAssMap >&            clusterTruthInputTag,
              const edm::EDGetTokenT< reco::GenJetCollection >&     genJetInputTag        );

  ~Converter();

  void consume( const edm::EventSetup& iSetup ) const;

  void consume( const edm::Event& iEvent ) const;

  /*!
  convert and store fitted tracks in ED product
  */
  void put( edm::Event& iEvent ) const;

private:

  const Settings*                                      settings_;
        Data*                                          data_;
  const TrackingParticleSelector*                      looseSelector_;
  const TrackingParticleSelector*                      tightSelector_;
  const bool                                           useMCtruth_;
  const edm::EDGetTokenT< DetSetVec >                  stubInputTag_;
  const edm::EDGetTokenT< TrackingParticleCollection > tpInputTag_;
  const edm::EDGetTokenT< TTClusterAssMap >            clusterTruthInputTag_;
  const edm::EDGetTokenT< reco::GenJetCollection >     genJetInputTag_;

  CablingMap cablingMap_;

};

}

#endif