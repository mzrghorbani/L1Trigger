#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Module.h"


using namespace std;

namespace TMTT {

Stub::Stub( const Settings* settings, Module* module, const TTStubRef& ttStubRef ) : settings_( settings ), module_( module ), ttStubRef_( ttStubRef ) {

  const bool& barrel   = module->barrel();
  const bool& flipped  = module->flipped();
  const bool& side     = module->side();
  const int&  ncolumns = module->ncolumns();
  const int&  nrows    = module->nrows();

  // get stub raw data
  const MeasurementPoint& mp = ttStubRef->getClusterRef( 0 )->findAverageLocalCoordinatesCentered();
  col_  = barrel || side                                                                ? mp.y() - ncolumns / 2        : ncolumns / 2 - mp.y();
  row_  = (  (barrel && flipped) || (!barrel && ( (side && flipped) || (!side && !flipped) )) ) ? nrows / 2 - mp.x()           : mp.x() - nrows / 2;
  //bend_ = ( !barrel && side || barrel )                                      ? -ttStubRef->getTriggerBend() : ttStubRef->getTriggerBend();
  bend_ = ttStubRef->getHardwareBend();
  if ( (!barrel) && side ) bend_ *= -1;
  if ( barrel ) bend_ *= -1;

  const GlobalPoint& posMod   = module->pos();
  const double&      tilt     = module->tilt();
  const double&      pitchRow = module->pitchRow();
  const double&      pitchCol = module->pitchCol();

  // calculate stub global position
  const double s = row_ * pitchRow;
  const double l = col_ * pitchCol;
  const double d = posMod.perp() + l * sin( tilt );

  r_   = sqrt( d * d + s * s );
  phi_ = reco::deltaPhi( atan2( s, d ) + posMod.phi(), 0. );
  z_   = posMod.z() + l * cos( tilt );

  const double&  chosenRofPhi            = settings->chosenRofPhi();
  const double&  baseR                   = settings->baseR();
  const double&  baseZ                   = settings->baseZ();
  const double&  barrelEndcapTransitionZ = settings->trackerTransitionZ();
  const int&     numEndcapWheels         = settings->trackerNumLayersEndcap();
  const int&     numBarrelLayers         = settings->trackerNumLayersBarrel();
  const Doubles& wheelLimitsZ            = settings->trackerLimitsEndcap();
  const Doubles& layerLimitsR            = settings->trackerLimitsBarrel();

  // identify layer and module type as f/w is doing it
  const double r = digi( r_ - chosenRofPhi, baseR ) + chosenRofPhi;
  const double z = digi( z_, baseZ );
  const bool   b = fabs( z ) < barrelEndcapTransitionZ;
  int moduleType = -1;
  int layer( 0 );
  if ( !b ) {

    for ( int n = 0; n < numEndcapWheels - 1; n++ )
      if ( abs( z ) > wheelLimitsZ[ n ] )
          layer = n + 1;
    if ( layer == 0 ) moduleType = r > 66.40 ? 6 : 5;
    if ( layer == 1 ) moduleType = r > 66.40 ? 6 : 5;
    if ( layer == 2 ) moduleType = r > 64.55 ? ( r > 72.95 ? 6 : 7 ) : 5;
    if ( layer == 3 ) moduleType = r > 64.55 ? ( r > 72.95 ? 6 : 7 ) : 5;
    if ( layer == 4 ) moduleType = r > 64.55 ? ( r > 82.35 ? 6 : 7 ) : 5;
    layer += 10;
    if ( layer == 10 ) layer = 2;
    if ( layer == 11 ) layer = 3;
    if ( layer == 12 ) layer = 4;
    if ( layer == 13 ) layer = 5;
    if ( layer == 14 ) layer = 6;

  } else {

    for ( int n = 0; n < numBarrelLayers - 1; n++ )
      if ( r > layerLimitsR[ n ]  )
          layer = n + 1;
    if( layer == 0 ) moduleType = fabs( z ) > 15.5 ? ( fabs( z ) > 29.9 ? 3 : 1 ) : 0;
    if( layer == 1 ) moduleType = fabs( z ) > 24.9 ? ( fabs( z ) > 69.3 ? 3 : 1 ) : 2;
    if( layer == 2 ) moduleType = fabs( z ) > 34.3 ? 1 : 2;
    if( layer == 3 ) moduleType = 4;
    if( layer == 4 ) moduleType = 4;
    if( layer == 5 ) moduleType = 4;
    layer += 10;
    if ( layer == 10 ) layer = 0;
    if ( layer == 11 ) layer = 1;
    if ( layer == 12 ) layer = 6;
    if ( layer == 13 ) layer = 4;
    if ( layer == 14 ) layer = 3;
    if ( layer == 15 ) layer = 2;

  }
  // compare with Truth
  if ( b != barrel )
    throw cms::Exception( "Stub::Stub barrelEndcapTransitionZ bad configured. (" + to_string( b ) + " vs. " + to_string( barrel ) + " for r = " + to_string( r ) + " and |z| = " + to_string( fabs( z ) ) + ")" );
  if ( layer != module->layerId() )
    throw cms::Exception( "Stub::Stub layer rages bad configured. (" + to_string( layer ) + " vs. " + to_string( module->layerId() ) + " for r = " + to_string( r ) + " and |z| = " + to_string( fabs( z ) ) + ")" );
  if ( moduleType != module->modType() )
    throw cms::Exception( "Stub::Stub moduleType ranges bad configured. (" + to_string( moduleType ) + " vs. " + to_string( module->modType() ) + " for r = " + to_string( r ) + " and |z| = " + to_string( fabs( z ) ) + ")" );

  const double& basePhi       = settings->basePhi();
  const double& baseSector    = settings->baseSector();
  const int&    numBinsPhi    = settings->htNumBinsPhi();
  const int&    numSectorsPhi = settings->numSectorsPhi();
  const double& baseM         = settings->baseM();
  const int&    numBinsPt     = settings->htNumBinsPt();
  const double& bendRes       = settings->stubBendRes();

  // calculate stub q/pt range
  const double& dr        = module->dr();
  double  qOverPtOverBend = pitchRow / r_ / dr;

  const double& baseInvR    = settings->baseInvR();
  const double& baseRforLut = settings->baseRforLUT();
  const double& invR        = digi( 1. / ( digi( r_ - chosenRofPhi, baseRforLut ) + chosenRofPhi ), baseInvR );

  const double& baseCot = settings->baseCot();
  const double  cot     = digi( digi( z_, baseZ ) * invR, baseCot );

  const bool&    useApproxB        = settings->useApproxB();
  const double&  baseRMoB          = settings->baseRMoB();
  const double&  baseMoB           = settings->baseMoB();
  const Doubles& alphas            = settings->gpAlphas();
  const Doubles& betas             = settings->gpBetas ();
  const int&     modType           = module->modType();
  const double&  alpha             = alphas[ moduleType ];
  const double&  beta              = betas [ moduleType ];
  const double&  baseAlpha         = settings->baseAlpha();
  const double&  baseBeta          = settings->baseBeta();

  const double RMoB = digi( digi( alpha, baseAlpha ) * fabs( cot ) + digi( beta, baseBeta ), baseRMoB );
  const double MoB  = digi( RMoB * invR, baseMoB );
  if ( useApproxB )
    qOverPtOverBend  = MoB;

  const double qOverPtMin = ( bend_ - bendRes ) * qOverPtOverBend;
  const double qOverPtMax = ( bend_ + bendRes ) * qOverPtOverBend;

  const int ptMin = floor( qOverPtMin / baseM ) + numBinsPt / 2;
  const int ptMax = floor( qOverPtMax / baseM ) + numBinsPt / 2;
  rangePt_ = make_pair( max( ptMin, 0 ), min( ptMax, numBinsPt - 1 ) );

  const int&    gpNumSectorsPhi = settings->gpNumSectorsPhi();
  const int&    numRegions = settings->dtcNumRegions();
  const double& baseRegion = settings->baseRegion();

  // calculate phi sector range
  const double rT = digi( ( r_ - chosenRofPhi ), baseR );
  SLV phiSecs( 0, numSectorsPhi );
  for ( int region = 0; region < numRegions; region++ ) {
    const double phi = digi( reco::deltaPhi( phi_ - ( region + 1 ) * baseRegion, .0 ), basePhi );
    double phiTMin = phi + rT * ( rangePt_.first  - numBinsPt / 2 + 0 ) * baseM;
    double phiTMax = phi + rT * ( rangePt_.second - numBinsPt / 2 + 1 ) * baseM;
    if ( phiTMin > phiTMax )
      swap( phiTMin, phiTMax );
    if ( phiTMin >= baseRegion / 2. || phiTMax < - baseRegion / 2. )
      continue;
    const int iPhi = region * gpNumSectorsPhi;
    phiSecs[ iPhi     ] = phiTMin <  0.;
    phiSecs[ iPhi + 1 ] = phiTMax >= 0.;
  }

  rangePhi_.first  = phiSecs.plEncode();
  rangePhi_.second = phiSecs.pmEncode();
  if ( rangePhi_.second == numSectorsPhi - 1 && rangePhi_.first == 0 )
    swap( rangePhi_.first, rangePhi_.second );

  const double&  baseDCot      = settings->baseDCot();
  const double&  chosenRofZ    = settings->chosenRofZ();
  const double&  beamWindowZ   = settings->trackerBeamWindowZ();
  const int&     numSectorsEta = settings->gpNumSectorsEta();
  const Doubles& bounderiesCot = settings->bounderiesCot();

  // calculate eta sector range
  const double cotRes = digi( beamWindowZ / chosenRofZ * fabs( 1. - chosenRofZ * invR ), baseDCot );
  const double cotMin = cot - cotRes;
  const double cotMax = cot + cotRes;
  for ( int bin = 0; bin < numSectorsEta; bin++ )
    if( cotMin < bounderiesCot[ bin + 1 ] ) {
      rangeEta_.first = bin;
      break;
    }
  rangeEta_.second = rangeEta_.first;
  for ( int bin = rangeEta_.first; bin < numSectorsEta; bin++ )
    if( cotMax < bounderiesCot[ bin + 1 ] ) {
      rangeEta_.second = bin;
      break;
    }

  // kill stub in DTC if |eta| too big or pt to small
  const double& stubMaxCot = settings->stubMaxCot();
  passDTC_ = true;
  if ( fabs( cot ) > stubMaxCot )
    passDTC_ = false;
  if ( rangePt_.second < 0 || rangePt_.first > numBinsPt - 1 )
    passDTC_ = false;

  passFE_ = true;

  valid_ = true;

}

}