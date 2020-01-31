import FWCore.ParameterSet.Config as cms

TMTrackProducer_params = cms.PSet (

  ParamsProducer = cms.PSet (
    UseMCtruth           = cms.bool    ( True                                                    ),
    EnableHistos         = cms.bool    ( True                                                    ),
    OutputString         = cms.string  ( "TML1Tracks"                                            ),
    InputTagTP           = cms.InputTag( "mix"                              , "MergedTrackTruth" ),
    InputTagStub         = cms.InputTag( "TTStubsFromPhase2TrackerDigis"    , "StubAccepted"     ),
    InputTagClusterTruth = cms.InputTag( "TTClusterAssociatorFromPixelDigis", "ClusterAccepted"  ),
    InputTagGenJet       = cms.InputTag( "ak4GenJets"                       , ""                 ),
    SpeedOfLight         = cms.double  (  2.9979                                                 ),
    MagneticField        = cms.double  (  3.8112                                                 ),
    ChosenRofPhi         = cms.double  ( 61.273                                                  ),
    ChosenRofZ           = cms.double  ( 50.                                                     ),
    UseApproxB           = cms.bool    ( True                                                    ),
    BApprox_gradient     = cms.double  (  0.886454                                               ),
    BApprox_intercept    = cms.double  (  0.504148                                               ),
    EnableTruncation     = cms.bool    ( True                                                    ),
    NumFramesInfra       = cms.int32   (   6                                                     ),
    NumFramesTMP         = cms.int32   ( 144                                                     ),
    EnableDigitize       = cms.bool    ( True                                                    ),
    WidthDSPa            = cms.int32   (  27                                                     ),
    WidthDSPb            = cms.int32   (  18                                                     ),
    WidthDSPc            = cms.int32   (  48                                                     ),
  ),

  #=== Cuts on MC truth particles (i.e., tracking particles) used for tracking efficiency measurements. And Rules for deciding when a reconstructed L1 track matches a MC truth particle (i.e. tracking particle)

  ParamsTP = cms.PSet (
    MinPt          = cms.double(  3.0 ),
    MaxEta         = cms.double(  2.4 ),
    MaxVertR       = cms.double(  1.0 ),
    MaxVertZ       = cms.double( 30.0 ),
    MaxD0          = cms.double(  5.0 ),
    MinLayers      = cms.int32 (  4   ),
    MinLayersPS    = cms.int32 (  0   ),
    MinLayersMatch = cms.int32 (  4   )
  ),

  #=== Cuts applied to stubs before arriving in L1 track finding board.

  ParamsStub = cms.PSet (
    BendResReduced = cms.bool  (  True   ),
    MaxEta         = cms.double(  2.4    ),
    MinPt          = cms.double(  3.     ),
    BendRes        = cms.double(  1.25   ),
    BaseBend       = cms.double(  0.25 ),
    WidthBend      = cms.int32 (  6    ),
    WidthR         = cms.int32 ( 12    ),
    WidthPhi       = cms.int32 ( 14    ),
    WidthZ         = cms.int32 ( 14    ),
    WidthRforLUT   = cms.int32 ( 10    )
  ),

  #=== Detector specifications.

  ParamsTracker = cms.PSet (
    OuterRadius         = cms.double (   112.7                                                                                                                                                                                                                                      ),
    InnerRadius         = cms.double (    21.8                                                                                                                                                                                                                                      ),
    HalfLength          = cms.double (   270.                                                                                                                                                                                                                                       ),
    TransitionZ         = cms.double (   125.                                                                                                                                                                                                                                       ),
    BeamWindowZ         = cms.double (    15.                                                                                                                                                                                                                                       ),
    NumModules          = cms.int32  ( 13296                                                                                                                                                                                                                                        ),
    NumLayers           = cms.int32  (     7                                                                                                                                                                                                                                        ),
    MaxLayerId          = cms.int32  (    25                                                                                                                                                                                                                                        ),
    NumLayersBarrel     = cms.int32  (     6                                                                                                                                                                                                                                        ),
    NumLayersEndcap     = cms.int32  (     5                                                                                                                                                                                                                                        ),
    NumModuleTypes      = cms.int32  (     8                                                                                                                                                                                                                                        ),
    Pitches             = cms.vdouble( 0.010, 0.010, 0.010, 0.010, 0.009, 0.010, 0.009, 0.009                                                                                                                                                                                       ),
    Separations         = cms.vdouble( 0.260, 0.260, 0.160, 0.400, 0.180, 0.400, 0.180, 0.400                                                                                                                                                                                       ),
    RangesBarrel        = cms.vdouble(  21.8892,  28.2077,  34.7025,  40.1651,  49.9315,  55.3447,  66.9739,  70.5494,  84.2932,  87.8435, 109.0890, 112.6142                                                                                                                       ),
    RangesEndcap        = cms.vdouble( 129.0642, 133.3437, 152.8783, 157.1577, 183.1871, 187.4665, 219.4871, 223.7666, 262.8358, 267.1153                                                                                                                                           ),
    NumRangesModuleType = cms.vint32 ( 1, 3, 2, 2, 1, 3, 3, 2                                                                                                                                                                                                                       ),
    RangesModuleTypeR   = cms.vdouble(  65.1,  70.3,  65.1,  80.8,  75.6, 107.6,  66.7, 107.6,  83.9, 107.6,  34.1,  63.9,  23.7,  66.1,  48.2,  64.0,  67.0, 112.6,  35.1,  40.0,  22.7,  28.1,  34.7,  37.9,  49.9,  53.1,  35.4,  40.2,  23.7,  28.2,  50.4,  55.3,  21.9,  25.2 ),
    RangesModuleTypeZ   = cms.vdouble( 183.2, 220.8, 262.8, 267.1, 183.2, 223.8, 129.1, 157.2, 262.8, 267.1, 183.3, 223.7, 129.2, 157.0, 263.0, 267.0,   2.2, 115.2,  71.9, 119.8,  30.5, 119.9,   0.1,  24.5,   0.4,  33.7,  25.3,  66.7,  15.7,  29.2,  34.8, 119.9,   0.2,  15.1 )
  ),

  #=== DTC specifications.

   ParamsDTC = cms.PSet (
    NumRegions       = cms.int32 (  9                                                 ),
    Num10GPS         = cms.int32 (  6                                                 ),
    Num5GPS          = cms.int32 (  6                                                 ),
    Num2S            = cms.int32 ( 12                                                 ),
    OrderStubsByBend = cms.bool  ( True                                               ),
    LenString        = cms.int32 ( 49                                                 ),
    Links            = cms.vint32( range( 4, 16 ) + range( 24, 48 ) + range( 56, 68 ) )
  ),

  #=== GP specifications.

   ParamsGP = cms.PSet (
    NumSectorsPhi = cms.int32  ( 2                                                                                                                    ),
    BounderiesEta = cms.vdouble( -2.4, -2.16, -1.95, -1.7, -1.43, -1.16, -0.89, -0.61, -0.31, 0.0, 0.31, 0.61, 0.89, 1.16, 1.43, 1.7, 1.95, 2.16, 2.4 ),
    LenString     = cms.int32  ( 29                                                                                                                   ),
    Links         = cms.vint32 ( range( 4, 13 ) + range( 24, 33 ) + range ( 36, 45 ) + range( 56, 65 )                                                )
  ),

  #=== HT specifications.

  ParamsHT = cms.PSet (
    MinPt      = cms.double(  3.                              ),
    NumBinsPt  = cms.int32 ( 18                               ),
    NumBinsPhi = cms.int32 ( 32                               ),
    MinLayers  = cms.int32 (  5                               ),
    SizeBuffer = cms.int32 ( 32                               ),
    LenString  = cms.int32 ( 27                               ),
    #Links      = cms.vint32( range( 4, 13 ) + range( 24, 33 ) )
    Links      = cms.vint32( 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34 )
  ),

  #=== MHT specifications.

  ParamsMHT = cms.PSet (
    NumBinsPt  = cms.int32 ( 2                                ),
    NumBinsPhi = cms.int32 ( 2                                ),
    MinLayers  = cms.int32 ( 5                                ),
    LenString  = cms.int32 ( 34                               ),
    #Links      = cms.vint32( range( 4, 13 ) + range( 24, 33 ) )
    Links      = cms.vint32( 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34 )
  ),

  #=== LR specifications.

  ParamsLR = cms.PSet (
    NumIterations = cms.int32 ( 12                                ),
    MinLayers     = cms.int32 (  4                                ),
    MinLayersPS   = cms.int32 (  2                                ),
    ResidPhi      = cms.double( 0.001                             ),
    ResidZ2S      = cms.double( 2.5                               ),
    ResidZPS      = cms.double( 0.07                              ),
    LenString     = cms.int32 ( 38                                ),
    Links         = cms.vint32( range( 60, 51 ) + range( 44, 35 ) )
  ),

  #=== DR specifications.

  ParamsDR = cms.PSet (
    Strict    = cms.bool  ( False                             ),
    LenString = cms.int32 ( 38                                ),
    Links     = cms.vint32( range( 60, 51 ) + range( 44, 35 ) )
  ),

  #=== Demo specifications.

  ParamsDemo = cms.PSet (
    Enable       = cms.bool  ( False                      ),
    StageIn      = cms.string( "HT"                       ),
    StageOut     = cms.string( "MHT"                      ),
    WidthFrame   = cms.int32 (  64                        ),
    DirModelSim  = cms.string( "/home/mghorbani/.xilinx_sim_libs/2018.3/modelsim_10.6d" ),
    OffsetDTCGP  = cms.int32 (  41                        ),
    OffsetDTCHT  = cms.int32 ( 240                        ),
    OffsetHTMHT  = cms.int32 (  22                        ),
    RunTimeDTCGP = cms.double(   6.15                     ),
    RunTimeDTCHT = cms.double(   6.17                     ),
    RunTimeHTMHT = cms.double(   6.045                    )
  )

)
