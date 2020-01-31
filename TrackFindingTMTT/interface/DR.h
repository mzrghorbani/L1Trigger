#ifndef __DR_H__
#define __DR_H__

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

class Settings;
class Track;

class DR {

public:

  DR( const Settings* settings, Data* data );

  ~DR() {}

  void produce();

private:

  void clean( Tracks& tracks ) const;

  const Settings* settings_;
        Data*     data_;
  const bool      strict_;

  Ints mhtIds_;

};

}

#endif