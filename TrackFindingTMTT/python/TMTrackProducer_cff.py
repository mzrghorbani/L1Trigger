import FWCore.ParameterSet.Config as cms

#---------------------------------------------------------------------------------------------------------
# This describes the full TMTT track reconstruction chain with 3 GeV threshold, where:
# the GP divides the tracker into 18 eta sectors;  
# the HT uses a  32x18 array, with transverese readout, 
# followed by the MHT (2x2) and the LR track fit; duplicate track removal is run.
#
# This usually corresponds to the current firmware.
#---------------------------------------------------------------------------------------------------------

#=== Import default values for all parameters & define EDProducer.

from L1Trigger.TrackFindingTMTT.TMTrackProducer_Defaults_cfi import TMTrackProducer_params

TMTrackProducer = cms.EDProducer('TMTrackProducer',
  # Load cfg parameters from TMTrackProducer_Defaults_cfi.py
  TMTrackProducer_params
)
