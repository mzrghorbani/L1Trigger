#ifndef __TP_H__
#define __TP_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "Track.h"

#include <vector>
#include <bitset>

using namespace std;

namespace TMTT {

class Settings;  
class Stub;
class Track;

class TP {

public:

  TP( const Settings* settings, const TrackingParticlePtr& tpPtr, const bool& tight, vector< const reco::GenJet* > jets );

  ~TP(){}

  void assoc( Stub* stub ) { assocStubs_.push_back( stub ); }

  void assocComplete();
  void routingComplete();

  int  numMatchedLayers( const Stubs& stubs ) const;
  int  numAssocLayers() const { return numMatchedLayers( assocStubs_ ); }

  TrackingParticlePtr edmPtr() const { return edmPtr_; }

  double eta()          const { return eta_;          }
  double phi()          const { return phi_;          }
  double pt()           const { return pt_;           }
  double qOverPt()      const { return qOverPt_;      }
  Stubs  assocStubs()   const { return assocStubs_;   }
  bool   useForEff()    const { return useForEff_;    }
  bool   tpInJet()      const { return tpInJet_;      }
  double nearestJetPt() const { return nearestJetPt_; }
  bool   useForAlgEff() const { return useForAlgEff_; }

  bool   reconstructableAfterDTC() const { return reconstructableAfterDTC_; }
  bool   reconstructableAfterGP () const { return reconstructableAfterGP_;  }

public:

  bool reconstructable( const Stubs&  stubs  );
  bool reconstructable( const Stubss& stubss );

  const Settings*           settings_;
  const TrackingParticlePtr edmPtr_;
  const double              eta_;
  const double              phi_;
  const double              pt_;
  const double              qOverPt_;

  Stubs  assocStubs_;
  bool   useForEff_;
  bool   useForAlgEff_;
  bool   tpInJet_;
  double nearestJetPt_;

  bool reconstructableAfterDTC_;
  bool reconstructableAfterGP_;

};

}

#endif
