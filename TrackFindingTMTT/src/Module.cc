#include "L1Trigger/TrackFindingTMTT/interface/Module.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"


namespace TMTT {

Module::Module( const   Settings*                  settings,
                const ::TrackerGeometry*           trackerGeometry,
                const ::TrackerTopology*           trackerTopology,
                const ::DetId&                     detId,
                const   int&                       dtcId
              ) : settings_( settings ), dtcId_( dtcId )
{

  const GeomDetUnit*            det0    = trackerGeometry->idToDetUnit( detId );
  const GeomDetUnit*            det1    = trackerGeometry->idToDetUnit( trackerTopology->partnerDetId( detId ) );
  const GlobalPoint             posMod1 = GlobalPoint( det1->position() );
  const PixelTopology*          topol   = dynamic_cast< const PixelTopology* >( &(dynamic_cast< const PixelGeomDetUnit* >( det0 )->specificTopology()) );
  const pair< double, double >& pitch   = topol->pitch();

  pos_      = GlobalPoint( det0->position() );
  pitchRow_ = pitch.first;
  pitchCol_ = pitch.second;
  ncolumns_ = topol->ncolumns();
  nrows_    = topol->nrows();

  flipped_ = pos_.mag() > posMod1.mag();
  side_    = pos_.z() > 0.;
  tilt_    = flipped_ ? atan2( posMod1.z() - pos_.z(), pos_.perp() - posMod1.perp() ) : atan2( pos_.z() - posMod1.z(), posMod1.perp() - pos_.perp() );
  dr_      = ( posMod1 - pos_ ).mag() / ( cos( tilt_ ) - sin( tilt_ ) * pos_.z() / pos_.perp() );

  barrel_   = !GeomDetEnumerators::isEndcap( det0->subDetector() );
  psModule_ = trackerGeometry->getDetectorType( detId ) == TrackerGeometry::ModuleType::Ph2PSP;

  uniqueLayerId_ = barrel_ ? trackerTopology->layer( detId ) : trackerTopology->tidWheel( detId ) +10;
  layerId_ = uniqueLayerId_;
  if (layerId_ == 6) layerId_ = 11;
  if (layerId_ == 5) layerId_ = 12;
  if (layerId_ == 4) layerId_ = 13;
  if (layerId_ == 3) layerId_ = 15;
  if (layerId_ > 10) layerId_ -= 8;
  layerId_--;

  const Doubles& separations = settings->trackerSeparations();
  const double spacing = fabs( ( posMod1 - pos_ ).mag() );
  if ( spacing > separations[0]-1e-4 && spacing < separations[0]+1e-4 ) {
    modType_ = 0;
    if ( fabs( tilt_ ) > 1e-4 )
      modType_ = 1;
  } else if ( spacing > separations[2]-1e-4 && spacing < separations[2]+1e-4 ) {
    modType_ = 2;
  } else if ( spacing > separations[3]-1e-4 && spacing < separations[3]+1e-4 ) {
    modType_ = 3;
    if ( !barrel_ )
      modType_ = psModule_ ? 5 : 7;
  } else if ( spacing > separations[4]-1e-4 && spacing < separations[4]+1e-4 ) {
    modType_ = 4;
    if ( !barrel_ )
      modType_ = 6;
  } else
    throw cms::Exception( "Module::Module invalid sensor spacing " + to_string( spacing ) );

}


}