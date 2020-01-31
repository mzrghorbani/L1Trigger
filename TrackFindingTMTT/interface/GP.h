#ifndef __GP_H__
#define __GP_H__

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

/**
 * Class to assign Stubs to sectors
 */
class GP {

public:

  GP( const Settings* settings, Data* data );

  ~GP(){};

  void produce() const;

private:

  typedef pair< Ints, Stub* > Frame; // pair of requested outputs of a stub and that stub
  typedef vector< Frame   >   Frames;
  typedef vector< Frames  >   Framess;
  typedef vector< Framess >   Framesss;

  /* stage               - stage number
   * ioStreams           - vector < vector < vector < const Stub* > > >
   *                       1st Index: router number of previous stage, 2nd index: output number of that router
   * region              - nonant number
   */
  void stage( const int& stage, Stubsss& ioStreams, const int& region ) const;

  Ints sectorsPhi ( Stub* stub ) const;
  Ints sectorsEta ( Stub* stub ) const;

  /*
   * inputStreams  - vector < vector < pair < vector < int >, const stub* > > >
   *                 1st index: input number, 2nd index: a stub paired with a vector of the output numbers, where that stub would like o go
   * outputStreams - vector < vector < const Stub* > >
   *                 1st index: output number, 2nd index: a stub or gap (nullptr), which was routed to that output number
   */
  void route( Framess& inputStreams, Stubss& outputStreams ) const;

  const Settings* settings_;
        Data*     data_;
  const int       numRegions_;    // number of TFPs per TMP
  const int       numDTCsPerTFP_; // number of DTCs connecetd to one TFP
  const int       fracPhi_;       // number of phi sectors per phi region (nonant)
  const int       fracEta_;       // number od rough eta sectors
  const int       numSectorsEta_;
  const int       numSectors_;
  const int       numStages_;     // number of stages
  
  Ints numInputs_;      // number of router inputs per stage
  Ints numOutputs_;     // number of router ouputs per stage
  Ints numRouter_;      // number of router per stage


};

}

#endif