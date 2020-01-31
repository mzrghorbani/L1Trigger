#ifndef __MODULE_H__
#define __MODULE_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "Settings.h"


using namespace std;

namespace TMTT {

class Settings;

class Module {

public:

  Module( const   Settings*                  settings,
          const ::TrackerGeometry*           trackerGeometry,
          const ::TrackerTopology*           trackerTopology,
          const ::DetId&                     detId ,
          const   int&                       dtcId );

  ~Module(){};

  GlobalPoint pos()           const { return pos_;            }
  double      tilt()          const { return tilt_;           }
  double      dr()            const { return dr_;             }
  bool        flipped()       const { return flipped_;        }
  bool        side()          const { return side_;           }
  double      pitchRow()      const { return pitchRow_;       }
  double      pitchCol()      const { return pitchCol_;       }
  int         ncolumns()      const { return ncolumns_;       }
  int         nrows()         const { return nrows_;          }
  bool        barrel()        const { return barrel_;         }
  bool        psModule()      const { return psModule_;       }
  int         layerId()       const { return layerId_;        }
  int         uniqueLayerId() const { return uniqueLayerId_;  }
  int         dtcId()         const { return dtcId_;          }
  int         modType()       const { return modType_;        }

public:

  const Settings* settings_;
  const int       dtcId_;

  GlobalPoint pos_;
  double      tilt_;
  double      dr_;
  bool        flipped_;
  bool        side_;
  double      pitchRow_;
  double      pitchCol_;
  int         ncolumns_;
  int         nrows_;
  bool        barrel_;
  bool        psModule_;
  int         layerId_;
  int         uniqueLayerId_;
  int         modType_;

};

}

#endif