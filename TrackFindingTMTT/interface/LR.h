#ifndef __LR_H__
#define __LR_H__

#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

using namespace std;

namespace TMTT {

class LR {

public:

  LR( const Settings* settings, Data* data );

  ~LR(){}

  void produce();

private:

  void initFit();

  void create();

  bool checkValidity( const Stubs& stubs ) const;

  int countLayers( const Stubs& stubs, const bool& onlySeed = false ) const;

  void calcHelix();

  void calcResidual();

  bool killLargestResidual();

  void findLargestResidual();

  int countStubs( const Stubs& stubs, const bool& onlyPS = false ) const;


  const Settings* settings_;
        Data*     data_;
  const int       numLayers_;
  const int       minLayers_;
  const int       minLayersPS_;
  const double    residPhi_;
  const double    residZ2S_;
  const double    residZPS_;
  const int       maxIteartions_;

  struct LRTrack {
    double qOverPt;
    double phiT;
    double cotTheta;
    double zT;
    LRTrack( double qOverPt = 0., double phiT = 0., double cotTheta = 0., double zT = 0. ) : qOverPt( qOverPt ), phiT( phiT ), cotTheta( cotTheta ), zT( zT ) {}
    LRTrack( const LRTrack& a ) : qOverPt( a.qOverPt ), phiT( a.phiT ), cotTheta( a.cotTheta ), zT( a.zT ) {}
    LRTrack& operator += ( const LRTrack& a ) {
      qOverPt  += a.qOverPt;
      phiT      = reco::deltaPhi( phiT + a.phiT, 0. );
      cotTheta += a.cotTheta;
      zT       += a.zT;
      return *this;
    }
    LRTrack& operator -= ( const LRTrack& a ) {
      qOverPt  -= a.qOverPt;
      phiT      = reco::deltaPhi( phiT - a.phiT, 0. );
      cotTheta -= a.cotTheta;
      zT       -= a.zT;
      return *this;
    }
    bool operator == ( const LRTrack& a ) {
      return qOverPt == a.qOverPt && phiT == a.phiT && cotTheta == a.cotTheta && zT == a.zT;
    }
    void convert_back( const Settings* settings ) {
      phiT    = phiT - qOverPt * settings->chosenRofPhi();
      qOverPt = -qOverPt / settings->invPtToDphi();
      zT      = zT - cotTheta * settings->chosenRofZ();
    }
    void convert( const Settings* settings ) {
      qOverPt  = -qOverPt * settings->invPtToDphi();
      phiT     = phiT + qOverPt * settings->chosenRofPhi();
      zT       = zT + cotTheta * settings->chosenRofZ();
    }
  };
 
  struct LRStub {
    double RPhi;
    double Phi;
    double RZ;
    double Z;
    LRStub( double RPhi = 0., double Phi = 0., double RZ = 0., double Z = 0. ) : RPhi( RPhi ), Phi( Phi ), RZ( RZ ), Z( Z ) {}
    LRStub( Stub* stub, const LRTrack& HTParameter, const Settings* settings ) :
      RPhi( stub->r() - settings->chosenRofPhi() ),
      Phi( reco::deltaPhi( stub->phi() - HTParameter.phiT - HTParameter.qOverPt * RPhi, 0. ) ),
      RZ( stub->r() - settings->chosenRofZ() ),
      Z( stub->z() - HTParameter.zT - HTParameter.cotTheta * RZ ) {}
    LRStub& operator <= ( const LRStub& a ) {
      RPhi = std::min( RPhi, a.RPhi );
      Phi = std::min( Phi, a.Phi );
      RZ = std::min( RZ, a.RZ );
      Z = std::min( Z, a.Z );
      return *this;
    }
    LRStub& operator >= ( const LRStub& a ) {
      RPhi = std::max( RPhi, a.RPhi );
      Phi = std::max( Phi, a.Phi );
      RZ = std::max( RZ, a.RZ );
      Z = std::max( Z, a.Z );
      return *this;
    }
    LRStub operator + ( const LRStub& a ) const {
      return LRStub( RPhi + a.RPhi, Phi + a.Phi, RZ + a.RZ, Z + a.Z );
    }
    LRStub& operator /= ( const double& a ) {
      RPhi /= a;
      Phi /= a;
      RZ /= a;
      Z /= a;
      return *this;
    }
  };
 
  struct Sum {
    unsigned int n;
    double xy;
    double x;
    double y;
    double xx;
    Sum( unsigned int n = 0, double xy = 0., double x = 0., double y = 0., double xx = 0. ) : n( n ), xy( xy ), x( x ), y( y ), xx( xx ) {}
    Sum& operator += ( const std::pair< double, double >& stub ) {
      n ++;
      xy += stub.first * stub.second;
      x += stub.first;
      y += stub.second;
      xx += stub.first * stub.first;
      return *this;
    }
    pair< double, double > calcLinearParameter() const {
      double denominator = n * xx - x * x;
      double slope = ( n * xy - x * y ) / denominator;
      double intercept = ( y * xx - x * xy ) / denominator;
      return make_pair( slope, intercept );
    }
  };
 
  struct Residual {
    double phi;
    double z;
    unsigned int layerId;
    unsigned int stubId;
    bool ps;
    Residual() : phi( 0. ), z( 0. ), layerId( 0 ), stubId( 0 ), ps( false ) {}
    Residual( double phi, double z, unsigned int layerId, unsigned int stubId, bool ps ) : phi( phi ), z( z ), layerId( layerId ), stubId( stubId ), ps( ps ) {}
    Residual( double x ) : phi( x ), z( x ), layerId( 0 ), stubId( 0 ), ps( false ) {}
    Residual& operator <= ( const Residual& a ) {
      phi = std::min( phi, a.phi );
      z = std::min( z, a.z );
      return *this;
    }
    Residual& operator -= ( const Residual& a ) {
      phi -= a.phi;
      z -= a.z;
      return *this;
    }
    double combined() const {
      return phi+z;
    }
    double max() const {
      return std::max( phi, z );
    }
  };
  typedef vector< Residual > Residuals;

  Track*                track_;
  int                   secPhi_;
  int                   secEta_;
  LRTrack               HTParameter_;
  LRTrack               LRParameter_;
  Stubs                 stubs_;
  bool                  valid_;
  map< int, Stubs >     stubMap_;
  map< int, int >       layerPopulation_;
  map< int, Residuals > residuals_;
  Residual              largestResid_;
  int                   nIterations_;

};

}

#endif