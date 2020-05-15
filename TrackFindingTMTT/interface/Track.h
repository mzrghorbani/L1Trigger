#ifndef __TRACK_H__
#define __TRACK_H__

#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <cmath>


using namespace std;

namespace TMTT {

class Stub;
class TP;

/**
 * Class to represent a Track
 */
class Track {

public:

  Track();

/*!
   Constructor used in HT.
  @param[in] settings Configuration.
  @param[in] htCell unique Track Candidate ID ( region ⊗ phi sector ⊗ eta sector ⊗ q/pt bin ⊗ phiT bin )
  @param[in] stubs Stubs forming this track
  @param[in] pos stream position of this track
  */
  Track( const Settings* settings, const int& htCell, Stubs  stubs, const int& pos );

  /*!
   Constructor used in MHT.
  @param[in] mhtCell mht cell number ( 0-3, sub q/pt bin ⊗ sub phiT bin )
  @param[in] htTrack pointer to original rough candidate
  @param[in] stubs Stubs forming this track
  @param[in] pos stream position of this track
  */
  Track( const int& mhtCell, const Track* htTrack, Stubs  stubs, const int& pos );

  /*!
   Constructor used in LR.
  @param[in] mhtTrack pointer to original candidate
  @param[in] stubs Stubs forming this track
  @param[in] qOverPt fitted q/pt
  @param[in] phi fitted phi at radius chosenRofPhi
  @param[in] cot fitted cot( Theta )
  @param[in] z fitted z at radius chosenRofZ
  */
  Track( const Track* mhtTrack, Stubs  stubs, const double& qOverPt, const double& phi, const double& cot, const double& z );

  ~Track(){}

  const Track* parent() const { return parent_; }

  Stubs   stubs()     const { return stubs_;     }
  int     binPt()     const { return binPt_;     }
  int     binPhi()    const { return binPhi_;    }
  int     secEta()    const { return secEta_;    }
  int     secPhi()    const { return secPhi_;    }
  int     region()    const { return region_;    }
  int     cellId()    const { return cellId_;    }
  int     streamId()  const { return streamId_;  }
  int     streamPos() const { return streamPos_; }
  bool    valid()     const { return valid_;     }
  double  qOverPt()   const { return qOverPt_;   }
  double  phi()       const { return phi_;       }
  double  z()         const { return z_;         }
  double  cot()       const { return cot_;       }
  double  chi2()      const { return chi2_;      }
  TPs     tps()       const { return tps_;       }

  int  size() const { return stubs_.size(); }
  void kill()       { valid_ = false;       }

public:

  /*!
   matches track with MCTruth.
  */
  void match();

  const Settings* settings_;
  const Track*    parent_; // pointer to parent track, nullptr if parentless
  Stubs     stubs_;

  int binPhi_;
  int binPt_;
  int secEta_;
  int secPhi_;
  int region_;

  int cellId_; // unique track id
  int streamId_; // id of the stream where this track is in
  int streamPos_; // position of this track in its stream

  bool   valid_;
  double qOverPt_;
  double phi_;
  double z_;
  double cot_;
  double chi2_;

  TPs  tps_; // associated tracking particles

};

}

#endif
