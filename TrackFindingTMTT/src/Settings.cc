#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <algorithm>

using namespace std;

namespace TMTT {

Settings::Settings( const edm::ParameterSet& iConfig ) :

  paramsProducer_( iConfig.getParameter< edm::ParameterSet >( "ParamsProducer" ) ),
  paramsTP_      ( iConfig.getParameter< edm::ParameterSet >( "ParamsTP"       ) ),
  paramsStub_    ( iConfig.getParameter< edm::ParameterSet >( "ParamsStub"     ) ),
  paramsTracker_ ( iConfig.getParameter< edm::ParameterSet >( "ParamsTracker"  ) ),
  paramsDTC_     ( iConfig.getParameter< edm::ParameterSet >( "ParamsDTC"      ) ),
  paramsGP_      ( iConfig.getParameter< edm::ParameterSet >( "ParamsGP"       ) ),
  paramsHT_      ( iConfig.getParameter< edm::ParameterSet >( "ParamsHT"       ) ),
  paramsMHT_     ( iConfig.getParameter< edm::ParameterSet >( "ParamsMHT"      ) ),
  paramsLR_      ( iConfig.getParameter< edm::ParameterSet >( "ParamsLR"       ) ),
  paramsDR_      ( iConfig.getParameter< edm::ParameterSet >( "ParamsDR"       ) ),
  paramsDemo_    ( iConfig.getParameter< edm::ParameterSet >( "ParamsDemo"     ) ),

  // Producer
  useMCtruth_          ( paramsProducer_.getParameter< bool >         ( "UseMCtruth"           ) ),
  enableHistos_        ( paramsProducer_.getParameter< bool >         ( "EnableHistos"         ) ),
  outputString_        ( paramsProducer_.getParameter< string >       ( "OutputString"         ) ),
  inputTagTP_          ( paramsProducer_.getParameter< edm::InputTag >( "InputTagTP"           ) ),
  inputTagStub_        ( paramsProducer_.getParameter< edm::InputTag >( "InputTagStub"         ) ),
  inputTagClusterTruth_( paramsProducer_.getParameter< edm::InputTag >( "InputTagClusterTruth" ) ),
  inputTagGenJet_      ( paramsProducer_.getParameter< edm::InputTag >( "InputTagGenJet"       ) ),
  speedOfLight_        ( paramsProducer_.getParameter< double >       ( "SpeedOfLight"         ) ),
  magneticField_       ( paramsProducer_.getParameter< double >       ( "MagneticField"        ) ),
  chosenRofPhi_        ( paramsProducer_.getParameter< double >       ( "ChosenRofPhi"         ) ),
  chosenRofZ_          ( paramsProducer_.getParameter< double >       ( "ChosenRofZ"           ) ),
  useApproxB_          ( paramsProducer_.getParameter< bool >         ( "UseApproxB"           ) ),
  bApprox_gradient_    ( paramsProducer_.getParameter< double >       ( "BApprox_gradient"     ) ),
  bApprox_intercept_   ( paramsProducer_.getParameter< double >       ( "BApprox_intercept"    ) ),
  enableTruncation_    ( paramsProducer_.getParameter< bool >         ( "EnableTruncation"     ) ),
  numFramesInfra_      ( paramsProducer_.getParameter< int >          ( "NumFramesInfra"       ) ),
  numFramesTMP_        ( paramsProducer_.getParameter< int >          ( "NumFramesTMP"         ) ),
  enableDigitize_      ( paramsProducer_.getParameter< bool >         ( "EnableDigitize"       ) ),
  widthDSPa_           ( paramsProducer_.getParameter< int >          ( "WidthDSPa"            ) ),
  widthDSPb_           ( paramsProducer_.getParameter< int >          ( "WidthDSPb"            ) ),
  widthDSPc_           ( paramsProducer_.getParameter< int >          ( "WidthDSPc"            ) ),
  invPtToDphi_         ( magneticField_ * speedOfLight_ / 2.E3                                   ),
  numFramesAlgoIn_     ( numFramesTMP_ - 1                                                       ),
  numFramesAlgoOut_    ( numFramesTMP_ - numFramesInfra_                                         ),

  // TP
  tpMinPt_         ( paramsTP_.getParameter< double >( "MinPt"          ) ),
  tpMaxEta_        ( paramsTP_.getParameter< double >( "MaxEta"         ) ),
  tpMaxVertR_      ( paramsTP_.getParameter< double >( "MaxVertR"       ) ),
  tpMaxVertZ_      ( paramsTP_.getParameter< double >( "MaxVertZ"       ) ),
  tpMaxD0_         ( paramsTP_.getParameter< double >( "MaxD0"          ) ),
  tpMinLayers_     ( paramsTP_.getParameter< int >   ( "MinLayers"      ) ),
  tpMinLayersPS_   ( paramsTP_.getParameter< int >   ( "MinLayersPS"    ) ),
  tpMinLayersMatch_( paramsTP_.getParameter< int >   ( "MinLayersMatch" ) ),

  // Stub
  stubBendResReduced_( paramsStub_.getParameter< bool >  ( "BendResReduced" ) ),
  stubMaxEta_        ( paramsStub_.getParameter< double >( "MaxEta"         ) ),
  stubMinPt_         ( paramsStub_.getParameter< double >( "MinPt"          ) ),
  stubBendRes_       ( paramsStub_.getParameter< double >( "BendRes"        ) ),
  stubMaxCot_        ( sinh( stubMaxEta_ )                                    ),
  baseBend_          ( paramsStub_.getParameter< double >( "BaseBend"       ) ),
  widthBend_         ( paramsStub_.getParameter< int >   ( "WidthBend"      ) ),
  widthR_            ( paramsStub_.getParameter< int >   ( "WidthR"         ) ),
  widthPhi_          ( paramsStub_.getParameter< int >   ( "WidthPhi"       ) ),
  widthZ_            ( paramsStub_.getParameter< int >   ( "WidthZ"         ) ),
  widthRforLUT_      ( paramsStub_.getParameter< int >   ( "WidthRforLUT"   ) ),

  // Tracker
  trackerOuterRadius_        ( paramsTracker_.getParameter< double > ( "OuterRadius"         ) ),
  trackerInnerRadius_        ( paramsTracker_.getParameter< double > ( "InnerRadius"         ) ),
  trackerHalfLength_         ( paramsTracker_.getParameter< double > ( "HalfLength"          ) ),
  trackerTransitionZ_        ( paramsTracker_.getParameter< double > ( "TransitionZ"         ) ),
  trackerBeamWindowZ_        ( paramsTracker_.getParameter< double > ( "BeamWindowZ"         ) ),
  trackerNumModules_         ( paramsTracker_.getParameter< int >    ( "NumModules"          ) ),
  trackerNumLayers_          ( paramsTracker_.getParameter< int >    ( "NumLayers"           ) ),
  trackerMaxLayerId_         ( paramsTracker_.getParameter< int >    ( "MaxLayerId"          ) ),
  trackerNumLayersBarrel_    ( paramsTracker_.getParameter< int >    ( "NumLayersBarrel"     ) ),
  trackerNumLayersEndcap_    ( paramsTracker_.getParameter< int >    ( "NumLayersEndcap"     ) ),
  trackerNumModuleTypes_     ( paramsTracker_.getParameter< int >    ( "NumModuleTypes"      ) ),
  trackerPitches_            ( paramsTracker_.getParameter< Doubles >( "Pitches"             ) ),
  trackerSeparations_        ( paramsTracker_.getParameter< Doubles >( "Separations"         ) ),
  trackerRangesBarrel_       ( paramsTracker_.getParameter< Doubles >( "RangesBarrel"        ) ),
  trackerRangesEndcap_       ( paramsTracker_.getParameter< Doubles >( "RangesEndcap"        ) ),
  trackerNumRangesModuleType_( paramsTracker_.getParameter< Ints >   ( "NumRangesModuleType" ) ),
  trackerRangesModuleTypeR_  ( paramsTracker_.getParameter< Doubles >( "RangesModuleTypeR"   ) ),
  trackerRangesModuleTypeZ_  ( paramsTracker_.getParameter< Doubles >( "RangesModuleTypeZ"   ) ),
  trackerLimitsBarrel_       ( rangeLimit( trackerRangesBarrel_ )                              ),
  trackerLimitsEndcap_       ( rangeLimit( trackerRangesEndcap_ )                              ),

  // DTC
  dtcNumRegions_      ( paramsDTC_.getParameter< int > ( "NumRegions"       ) ),
  dtcNum10GPS_        ( paramsDTC_.getParameter< int > ( "Num10GPS"         ) ),
  dtcNum5GPS_         ( paramsDTC_.getParameter< int > ( "Num5GPS"          ) ),
  dtcNum2S_           ( paramsDTC_.getParameter< int > ( "Num2S"            ) ),
  dtcOrderStubsByBend_( paramsDTC_.getParameter< bool >( "OrderStubsByBend" ) ),
  dtcNumPerRegion_    ( dtcNum10GPS_ + dtcNum5GPS_ + dtcNum2S_                ),
  dtcNumPerTFP_       ( 2 * dtcNumPerRegion_                                  ),
  dtcLenString_       ( paramsDTC_.getParameter< int > ( "LenString"        ) ),
  dtcLinks_           ( paramsDTC_.getParameter< Ints >( "Links"            ) ),

  // GP
  gpNumSectorsPhi_( paramsGP_.getParameter< int >    ( "NumSectorsPhi" ) ),
  gpBounderiesEta_( paramsGP_.getParameter< Doubles >( "BounderiesEta" ) ),
  gpNumSectorsEta_( gpBounderiesEta_.size() - 1                          ),
  gpAlphas_       ( { 0., trackerPitches_[1] / trackerSeparations_[1] * bApprox_gradient_, 0., trackerPitches_[3] / trackerSeparations_[3] * bApprox_gradient_, 0., trackerPitches_[5] / trackerSeparations_[5], trackerPitches_[6] / trackerSeparations_[6], trackerPitches_[7] / trackerSeparations_[7] }   ),
  gpBetas_        ( { trackerPitches_[0] / trackerSeparations_[0], trackerPitches_[1] / trackerSeparations_[1] * bApprox_intercept_, trackerPitches_[2] / trackerSeparations_[2], trackerPitches_[3] / trackerSeparations_[3] * bApprox_intercept_, trackerPitches_[4] / trackerSeparations_[4], 0., 0., 0. } ),
  gpLenString_    ( paramsGP_.getParameter< int >    ( "LenString"     ) ),
  gpLinks_        ( paramsGP_.getParameter< Ints >   ( "Links"         ) ),

  // HT
  htMinPt_     ( paramsHT_.getParameter< double >( "MinPt"      ) ),
  htNumBinsPt_ ( paramsHT_.getParameter< int >   ( "NumBinsPt"  ) ),
  htNumBinsPhi_( paramsHT_.getParameter< int >   ( "NumBinsPhi" ) ),
  htMinLayers_ ( paramsHT_.getParameter< int >   ( "MinLayers"  ) ),
  htSizeBuffer_( paramsHT_.getParameter< int >   ( "SizeBuffer" ) ),
  htNumCells_  ( htNumBinsPt_ * htNumBinsPhi_                     ),
  htLenString_ ( paramsHT_.getParameter< int >   ( "LenString"  ) ),
  htLinks_     ( paramsHT_.getParameter< Ints >  ( "Links"      ) ),

  // MHT
  mhtNumBinsPt_ ( paramsMHT_.getParameter< int > ( "NumBinsPt"  ) ),
  mhtNumBinsPhi_( paramsMHT_.getParameter< int > ( "NumBinsPhi" ) ),
  mhtMinLayers_ ( paramsMHT_.getParameter< int > ( "MinLayers"  ) ),
  mhtNumCells_  ( mhtNumBinsPt_ * mhtNumBinsPhi_                  ),
  mhtLenString_ ( paramsMHT_.getParameter< int > ( "LenString"  ) ),
  mhtLinks_     ( paramsMHT_.getParameter< Ints >( "Links"      ) ),

  // LR
  lrNumIterations_( paramsLR_.getParameter< int >   ( "NumIterations" ) ),
  lrMinLayers_    ( paramsLR_.getParameter< int >   ( "MinLayers"     ) ),
  lrMinLayersPS_  ( paramsLR_.getParameter< int >   ( "MinLayersPS"   ) ),
  lrResidPhi_     ( paramsLR_.getParameter< double >( "ResidPhi"      ) ),
  lrResidZ2S_     ( paramsLR_.getParameter< double >( "ResidZ2S"      ) ),
  lrResidZPS_     ( paramsLR_.getParameter< double >( "ResidZPS"      ) ),
  lrLenString_    ( paramsLR_.getParameter< int >   ( "LenString"     ) ),
  lrLinks_        ( paramsLR_.getParameter< Ints >  ( "Links"         ) ),

  // DR
  drStrict_   ( paramsDR_.getParameter< bool >( "Strict"    ) ),
  drLenString_( paramsDR_.getParameter< int > ( "LenString" ) ),
  drLinks_    ( paramsDR_.getParameter< Ints >( "Links"     ) ),

  // Demo
  demoEnable_      ( paramsDemo_.getParameter< bool >  ( "Enable"       ) ),
  demoStageIn_     ( paramsDemo_.getParameter< string >( "StageIn"      ) ),
  demoStageOut_    ( paramsDemo_.getParameter< string >( "StageOut"     ) ),
  demoWidthFrame_  ( paramsDemo_.getParameter< int >   ( "WidthFrame"   ) ),
  demoDirModelSim_ ( paramsDemo_.getParameter< string >( "DirModelSim"  ) ),
  offsetDTCGP_     ( paramsDemo_.getParameter< int >   ( "OffsetDTCGP"  ) ),
  offsetDTCHT_     ( paramsDemo_.getParameter< int >   ( "OffsetDTCHT"  ) ),
  offsetHTMHT_     ( paramsDemo_.getParameter< int >   ( "OffsetHTMHT"  ) ),
  runTimeDTCGP_    ( paramsDemo_.getParameter< double >( "RunTimeDTCGP" ) ),
  runTimeDTCHT_    ( paramsDemo_.getParameter< double >( "RunTimeDTCHT" ) ),
  runTimeHTMHT_    ( paramsDemo_.getParameter< double >( "RunTimeHTMHT" ) )
{

  // transform eta sector boundaries into cot(Theta) boundaries
  for ( const double& eta : gpBounderiesEta_ )
    bounderiesCot_.push_back( sinh( eta ) );
  // cot(Theta) sector centres
  for ( int n = 0; n < gpNumSectorsEta_; n++ )
    cots_.push_back( ( bounderiesCot_[ n ] + bounderiesCot_[ n + 1 ] ) / 2. );

  // calculation of integer related constants (ranges, widths, bases) in the next ~150 lines
  numSectorsPhi_ = dtcNumRegions_ * gpNumSectorsPhi_;
  numSectors_    = gpNumSectorsPhi_ * gpNumSectorsEta_;
  baseRegion_ = 2. * M_PI / (double)dtcNumRegions_;
  baseSector_ = baseRegion_ / (double)gpNumSectorsPhi_;

  Doubles rangesCot;
  for ( int n = 0; n < gpNumSectorsEta_; n++ )
    rangesCot.push_back( bounderiesCot_[ n + 1 ] - bounderiesCot_[ n ] );
  const double maxRangeZT = (*max_element( rangesCot.begin(), rangesCot.end() )) * chosenRofZ_;

  const double neededRangeM   = 2. * invPtToDphi_ / htMinPt_;
  const double neededRangeC   = 2. * M_PI / (double)( dtcNumRegions_ * gpNumSectorsPhi_ );
  const double neededRangeR   = 2. * max( fabs( chosenRofPhi_ - trackerOuterRadius_ ), fabs( chosenRofPhi_ - trackerInnerRadius_ ) );
  const double neededRangeZ   = 2. * trackerHalfLength_;
  const double neededRangePhi = neededRangeC + neededRangeM * neededRangeR / 2.;
  const double neededRangeCot = 2. * stubMaxCot_;

  const int widthCotTheta = widthDSPb_ - 1;
  const int widthInvR     = widthDSPb_ - 1;

  baseM_ = neededRangeM / htNumBinsPt_;
  baseC_ = neededRangeC / htNumBinsPhi_;

  const int baseDiffPhi = widthPhi_ - ceil( log2( neededRangePhi / baseC_          ) );
  const int baseDiffR   = widthR_   - ceil( log2( neededRangeR   / baseC_ * baseM_ ) );

  basePhi_ = pow( 2, -baseDiffPhi ) * baseC_;
  baseR_   = pow( 2, -baseDiffR   ) * baseC_ / baseM_;

  const int baseDiffZ = widthZ_ - ceil( log2( neededRangeZ / baseR_ ) );
  baseZ_ = pow( 2, -baseDiffZ ) * baseR_;

  const int baseDiffCotTheta = widthCotTheta - ceil( log2( neededRangeCot ) );
  const int baseDiffInvR     = - floor( log2( ( pow( 2, widthInvR - 1 ) - 1.5 ) * trackerInnerRadius_ / baseR_ ) );
  baseRforLUT_ = baseR_ * pow( 2, widthR_ - widthRforLUT_ );

  baseCotTheta_ = pow( 2, -baseDiffCotTheta );
  baseInvR_     = pow( 2,  baseDiffInvR     ) / baseR_;
  baseCFull_    = pow( 2, baseDiffPhi - baseDiffR ) * basePhi_;

  widthEta_    = ceil( log2( gpNumSectorsEta_  ) );
  widthCBin_   = ceil( log2( htNumBinsPhi_     ) );
  widthMBin_   = ceil( log2( htNumBinsPt_      ) );
  widthLayer_  = ceil( log2( trackerNumLayers_ ) );

  const double neededRangeChiPhi = 2. * baseC_;
  const double neededRangeChiZ   = maxRangeZT + 4.0 * trackerBeamWindowZ_ * ( trackerOuterRadius_ / chosenRofZ_ - 1.0 );
  widthChiPhi_ = ceil( log2( neededRangeChiPhi / basePhi_ ) );
  widthChiZ_   = ceil( log2( neededRangeChiZ   / baseZ_   ) );

  widthBinPt_  = widthMBin_ + ceil( log2( mhtNumBinsPt_  ) );
  widthBinPhi_ = widthCBin_ + ceil( log2( mhtNumBinsPhi_ ) );

  const double stubMaxCot = sinh( stubMaxEta_ );
  Doubles maxCots;
  int m( 0 );
  for ( int modType = 0; modType < trackerNumModuleTypes_; modType++ )
    for ( int r = 0; r < trackerNumRangesModuleType_.at( modType ); r++ ) {
      const double& minR = trackerRangesModuleTypeR_[ m + 1 ];
      const double& maxZ = trackerRangesModuleTypeZ_[ m ];
      const double  maxCot = maxZ / minR;
      maxCots.push_back( min( maxCot, stubMaxCot ) );
      m += 2;
    }
  const double maxCot = *max_element( maxCots.begin(), maxCots.end() );

  const int    widthCotMax     = widthDSPa_ - 1;
  const int    baseDiffCotFull = baseDiffInvR - baseDiffZ;
  const double baseCotFull     = pow( 2, baseDiffCotFull );
  const int    widthCotNeeded  = ceil( log2( maxCot / baseCotFull ) ) + 1;
  const int    widthCot        = min( widthCotMax, widthCotNeeded );
  const int    dropedLSBCot    = widthCotNeeded - widthCot;
  const int    baseDiffCot     = baseDiffCotFull + dropedLSBCot;
               baseCot_        = pow( 2, baseDiffCot );

  const double maxAlpha = *max_element( gpAlphas_.begin(), gpAlphas_.end() );
  const double maxBeta  = *max_element( gpBetas_ .begin(), gpBetas_ .end() );

  const int widthAlpha    = widthDSPb_ - 1;
  const int baseDiffAlpha = ceil( log2( maxAlpha / baseC_ ) ) + 1 - widthAlpha;
            baseAlpha_    = baseC_ * pow( 2, baseDiffAlpha );
  const int baseDiffBeta  = baseDiffCot + baseDiffAlpha;
            baseBeta_     = baseC_ * pow( 2, baseDiffBeta );
  const int widthBeta     = ceil( log2( maxBeta / baseBeta_ ) ) + 1;

  Doubles maxRMoBs;
  m = 0;
  for ( int modType = 0; modType < trackerNumModuleTypes_; modType++ ) {
    const double& alpha   = gpAlphas_[ modType ];
    const double& beta    = gpBetas_ [ modType ];
    for ( int r = 0; r < trackerNumRangesModuleType_.at( modType ); r++ ) {
      const double& maxCotT  = maxCots[ m++ ];
      const double  maxRMoB = alpha * maxCotT + beta;
      maxRMoBs.push_back( maxRMoB );
    }
  }
  const double maxRMoB = *max_element( maxRMoBs.begin(), maxRMoBs.end() );

  const int    widthRMoBMax     = widthDSPa_ - 1;
  const int    baseDiffRMoBFull = baseDiffBeta;
  const double baseRMoBFull     = baseBeta_;
  const int    widthRMoBNeeded  = ceil( log2( maxRMoB / baseRMoBFull ) ) + 1;
  const int    widthRMoB        = min( widthRMoBMax, widthRMoBNeeded );
  const int    dropedLSBRMoB    = widthRMoBNeeded - widthRMoB;
  const int    baseDiffRMoB     = baseDiffRMoBFull + dropedLSBRMoB;
               baseRMoB_        = baseC_ * pow( 2, baseDiffRMoB );

  Doubles maxMoBs;
  m = 0;
  for ( int modType = 0; modType < trackerNumModuleTypes_; modType++ )
    for ( int r = 0; r < trackerNumRangesModuleType_.at( modType ); r++ ) {
      const double& maxRMoB = maxRMoBs[ m / 2 ];
      const double& minR   = trackerRangesModuleTypeR_[ m + 1 ];
      const double  maxMoB = maxRMoB / minR;
      maxMoBs.push_back( maxMoB );
      m += 2;
    }
  const double maxMoB = *max_element( maxMoBs.begin(), maxMoBs.end() );

  const int    widthMoBMax     = widthDSPb_ - 1;
  const int    baseDiffMoBFull = baseDiffRMoB + baseDiffInvR + baseDiffR;
  const double baseMoBFull     = baseM_ * pow( 2, baseDiffMoBFull );
  const int    widthMoBNeeded  = ceil( log2( maxMoB / baseMoBFull ) ) + 1;
  const int    widthMoB        = min( widthMoBMax, widthMoBNeeded );
  const int    dropedLSBMoB    = widthMoBNeeded - widthMoB;
  const int    baseDiffMoB     = baseDiffMoBFull + dropedLSBMoB;
               baseMoB_        = baseM_ * pow( 2, baseDiffMoB );

  const int    widthDCotMax    = widthDSPb_;
  const double maxDCot         = trackerBeamWindowZ_ / chosenRofZ_ * max( abs( 1.0 - trackerInnerRadius_ / chosenRofZ_ ), abs( 1.0 - trackerOuterRadius_ / chosenRofZ_ ) );
  const int    widthDCotNeeded = ceil( log2( maxDCot / baseCot_ ) );
  const int    widthDCot       = min( widthDCotMax, widthDCotNeeded );
  const int    baseDiffDCot    = widthDCotNeeded - widthDCot;
               baseDCot_       = baseCot_ * pow( 2, baseDiffDCot );

  if ( neededRangePhi > 2. * neededRangeC )
    throw cms::Exception("More then 2 overlapping regions. H/W cannot handle this.");

}

Doubles Settings::rangeLimit( Doubles ranges ) const {

  Doubles res;
  for ( int k = 0; k < (int)ranges.size() / 2; k++ )
    res.push_back( ( ranges[ 2 * ( k + 1 ) ] + ranges[ 2 * ( k + 1 ) - 1 ] ) / 2. );
  return res;

}

}
