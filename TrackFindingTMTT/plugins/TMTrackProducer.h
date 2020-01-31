#ifndef __TMTRACKPRODUCER_H__
#define __TMTRACKPRODUCER_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"

using namespace std;

namespace TMTT {

class Settings;
class Data;
class Converter;
class DTC;
class GP;
class HT;
class MHT;
class LR;
class LRHLS;
class KF;
class DR;
class Demo;
class Histos;


/**
 * Class to produce TTracks out of TTStubs
 */
class TMTrackProducer : public edm::EDProducer {

public:

  explicit TMTrackProducer( const edm::ParameterSet& );

  ~TMTrackProducer();

private:

  virtual void beginRun( const edm::Run&, const edm::EventSetup& );

  virtual void produce( edm::Event&, const edm::EventSetup& );

  virtual void endJob();

private:

  Settings*  settings_;
  Data*      data_;

  Converter* converter_;

  DTC*    dtc_;
  GP*     gp_;
  HT*     ht_;
  MHT*    mht_;
  LR*     lr_;
  LRHLS*  lrhls_;
  KF*     kf_;
  DR*     dr_;
  Demo*   demo_;
  Histos* histos_;

};

}

#endif