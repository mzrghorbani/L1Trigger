import FWCore.ParameterSet.Config as cms

#---------------------------------------------------------------------------------------------------------
# This describes the full TMTT track reconstruction chain with 2 GeV threshold, where:
# the GP divides the tracker into 18 eta and 18 phi sectors;  
# the HT uses a  32x18 array followed by 2x2 Mini-HT array, with transverese HT readout & 2 to 1 multiplexing, 
# followed by the KF and LR track fit; duplicate track removal is run.
#---------------------------------------------------------------------------------------------------------

#=== Import default values for all parameters & define EDProducer.

from L1Trigger.TrackFindingTMTT.TMTrackProducer_Defaults_cfi import TMTrackProducer_params

TMTrackProducer = cms.EDProducer('TMTrackProducer',
  # Load cfg parameters from TMTrackProducer_Defaults_cfi.py
  TMTrackProducer_params
)

#===================================================================================================
# Uncomment the following 2 lines to disable all use of MC truth info & all output histograms.
# THIS SAVES CPU, SO IS GOOD IDEA IF YOU ONLY CARE ABOUT PRODUCING TTTrack COLLECTION!
#===================================================================================================

#TMTrackProducer.UseMCtruth   = cms.bool( False )
#TMTrackProducer.EnableHistos = cms.bool( False )

#===================================================================================================
#=== The following override the default values.
#===================================================================================================

TMTrackProducer.ParamsProducer.EnableTruncation = cms.bool  ( False )
TMTrackProducer.ParamsTP.      MinPt            = cms.double(  2.   )
TMTrackProducer.ParamsStub.    MinPt            = cms.double(  2.   )
TMTrackProducer.ParamsHT.      MinPt            = cms.double(  2.   )
TMTrackProducer.ParamsHT.      NumBinsPt        = cms.int32 ( 28    )
TMTrackProducer.ParamsHT.      MinLayers        = cms.int32 (  4    )
TMTrackProducer.ParamsMHT.     MinLayers        = cms.int32 (  4    )