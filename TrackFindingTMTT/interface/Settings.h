#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "L1Trigger/TrackFindingTMTT/plugins/TMTrackProducerFwd.h"
#include "L1Trigger/TrackFindingTMTT/interface/Data.h"


using namespace std;

namespace TMTT {

/**
 * Class to store, calculate and provide run-time constants
 */
class Settings {

public:

  Settings( const edm::ParameterSet& iConfig );

  ~Settings(){}

  // Producer
  bool          useMCtruth()           const { return useMCtruth_;           }
  bool          enableHistos()         const { return enableHistos_;         }
  string        outputString()         const { return outputString_;         }
  edm::InputTag inputTagTP()           const { return inputTagTP_;           }
  edm::InputTag inputTagStub()         const { return inputTagStub_;         }
  edm::InputTag inputTagClusterTruth() const { return inputTagClusterTruth_; }
  edm::InputTag inputTagGenJet()       const { return inputTagGenJet_;       }
  double        magneticField()        const { return magneticField_;        }
  double        chosenRofPhi()         const { return chosenRofPhi_;         }
  double        chosenRofZ()           const { return chosenRofZ_;           }
  bool          useApproxB()           const { return useApproxB_;           }
  double        bApprox_gradient()     const { return bApprox_gradient_;     }
  double        bApprox_intercept()    const { return bApprox_intercept_;    }
  bool          enableTruncation()     const { return enableTruncation_;     }
  int           numFramesInfra()       const { return numFramesInfra_;       }
  int           numFramesTMP()         const { return numFramesTMP_;         }
  bool          enableDigitize()       const { return enableDigitize_;       }
  int           widthDSPa()            const { return widthDSPa_;            }
  int           widthDSPb()            const { return widthDSPb_;            }
  int           widthDSPc()            const { return widthDSPc_;            }
  double        invPtToDphi()          const { return invPtToDphi_;          }
  int           numFramesAlgoIn()      const { return numFramesAlgoIn_;      }
  int           numFramesAlgoOut()     const { return numFramesAlgoOut_;     }

  // TP
  double tpMinPt()          const { return tpMinPt_;          }
  double tpMaxEta()         const { return tpMaxEta_;         }
  double tpMaxVertR()       const { return tpMaxVertR_;       }
  double tpMaxVertZ()       const { return tpMaxVertZ_;       }
  double tpMaxD0()          const { return tpMaxD0_;          }
  int    tpMinLayers()      const { return tpMinLayers_;      }
  int    tpMinLayersPS()    const { return tpMinLayersPS_;    }
  int    tpMinLayersMatch() const { return tpMinLayersMatch_; }

  // Stub
  bool   stubBendResReduced() const { return stubBendResReduced_; }
  double stubMaxEta()         const { return stubMaxEta_;         }
  double stubMinPt()          const { return stubMinPt_;          }
  double stubBendRes()        const { return stubBendRes_;        }
  double stubMaxCot()         const { return stubMaxCot_;         }
  double baseBend()           const { return baseBend_;           }
  int    widthBend()          const { return widthBend_;          }
  int    widthR()             const { return widthR_;             }
  int    widthPhi()           const { return widthPhi_;           }
  int    widthZ()             const { return widthZ_;             }

  // Tracker
  double  trackerTransitionZ()     const { return trackerTransitionZ_;     }
  double  trackerBeamWindowZ()     const { return trackerBeamWindowZ_;     }
  int     trackerNumModules()      const { return trackerNumModules_;      }
  int     trackerNumLayers()       const { return trackerNumLayers_;       }
  int     trackerMaxLayerId()      const { return trackerMaxLayerId_;       }
  int     trackerNumLayersBarrel() const { return trackerNumLayersBarrel_; }
  int     trackerNumLayersEndcap() const { return trackerNumLayersEndcap_; }
  Doubles trackerSeparations()     const { return trackerSeparations_;     }
  Doubles trackerLimitsBarrel()    const { return trackerLimitsBarrel_;    }
  Doubles trackerLimitsEndcap()    const { return trackerLimitsEndcap_;    }

  // DTC
  int  dtcNumRegions()       const { return dtcNumRegions_;       }
  int  dtcNum10GPS()         const { return dtcNum10GPS_;         }
  int  dtcNum5GPS()          const { return dtcNum5GPS_;          }
  int  dtcNum2S()            const { return dtcNum2S_;            }
  bool dtcOrderStubsByBend() const { return dtcOrderStubsByBend_; }
  int  dtcNumPerRegion()     const { return dtcNumPerRegion_;     }
  int  dtcNumPerTFP()        const { return dtcNumPerTFP_;        }
  int  dtcLenString()        const { return dtcLenString_;        }
  Ints dtcLinks()            const { return dtcLinks_;            }

  // GP
  int     gpNumSectorsPhi() const { return gpNumSectorsPhi_; }
  Doubles gpBounderiesEta() const { return gpBounderiesEta_; }
  int     gpNumSectorsEta() const { return gpNumSectorsEta_; }
  Doubles gpAlphas()        const { return gpAlphas_;        }
  Doubles gpBetas()         const { return gpBetas_;         }
  int     gpLenString()     const { return gpLenString_;     }
  Ints    gpLinks()         const { return gpLinks_;         }

  // HT
  double htMinPt()      const { return htMinPt_;      }
  int    htNumBinsPt()  const { return htNumBinsPt_;  }
  int    htNumBinsPhi() const { return htNumBinsPhi_; }
  int    htMinLayers()  const { return htMinLayers_;  }
  int    htSizeBuffer() const { return htSizeBuffer_; }
  int    htNumCells()   const { return htNumCells_;   }
  int    htLenString()  const { return htLenString_;  }
  Ints   htLinks()      const { return htLinks_;      }

  // MHT
  int  mhtNumBinsPt()  const { return mhtNumBinsPt_;  }
  int  mhtNumBinsPhi() const { return mhtNumBinsPhi_; }
  int  mhtMinLayers()  const { return mhtMinLayers_;  }
  int  mhtNumCells()   const { return mhtNumCells_;   }
  int  mhtLenString()  const { return mhtLenString_;  }
  Ints mhtLinks()      const { return mhtLinks_;      }

  // LR
  int    lrNumIterations() const { return lrNumIterations_; }
  int    lrMinLayers()     const { return lrMinLayers_;     }
  int    lrMinLayersPS()   const { return lrMinLayersPS_;   }
  double lrResidPhi()      const { return lrResidPhi_;      }
  double lrResidZ2S()      const { return lrResidZ2S_;      }
  double lrResidZPS()      const { return lrResidZPS_;      }
  int    lrLenString()     const { return lrLenString_;     }
  Ints   lrLinks()         const { return lrLinks_;         }

  // DR
  bool drStrict()    const { return drStrict_;    }
  int  drLenString() const { return drLenString_; }
  Ints drLinks()     const { return drLinks_;     }

  // Demo
  bool   demoEnable()      const { return demoEnable_;      }
  string demoStageIn()     const { return demoStageIn_;     }
  string demoStageOut()    const { return demoStageOut_;    }
  int    demoWidthFrame()  const { return demoWidthFrame_;  }
  string demoDirModelSim() const { return demoDirModelSim_; }
  int    offsetDTCGP()     const { return offsetDTCGP_;     }
  int    offsetDTCHT()     const { return offsetDTCHT_;     }
  int    offsetHTMHT()     const { return offsetHTMHT_;     }
  double runTimeDTCGP()    const { return runTimeDTCGP_;    }
  double runTimeDTCHT()    const { return runTimeDTCHT_;    }
  double runTimeHTMHT()    const { return runTimeHTMHT_;    }

  // rest
  Doubles bounderiesCot() const { return bounderiesCot_; }
  Doubles cots()          const { return cots_;          }
  int     numSectorsPhi() const { return numSectorsPhi_; }
  int     numSectors()    const { return numSectors_;    }
  double  baseRegion()    const { return baseRegion_;    }
  double  baseSector()    const { return baseSector_;    }
  double  baseM()         const { return baseM_;         }
  double  baseC()         const { return baseC_;         }
  double  baseCFull()     const { return baseCFull_;     }
  double  baseR()         const { return baseR_;         }
  double  baseRforLUT()   const { return baseRforLUT_;   }
  double  basePhi()       const { return basePhi_;       }
  double  baseZ()         const { return baseZ_;         }
  double  baseCotTheta()  const { return baseCotTheta_;  }
  double  baseInvR()      const { return baseInvR_;      }
  double  baseCot()       const { return baseCot_;       }
  double  baseDCot()      const { return baseDCot_;      }
  double  baseAlpha()     const { return baseAlpha_;     }
  double  baseBeta()      const { return baseBeta_;      }
  double  baseRMoB()      const { return baseRMoB_;      }
  double  baseMoB()       const { return baseMoB_;       }
  int     widthEta()      const { return widthEta_;      }
  int     widthCBin()     const { return widthCBin_;     }
  int     widthMBin()     const { return widthMBin_;     }
  int     widthLayer()    const { return widthLayer_;    }
  int     widthChiPhi()   const { return widthChiPhi_;   }
  int     widthChiZ()     const { return widthChiZ_;     }
  int     widthBinPt()    const { return widthBinPt_;    }
  int     widthBinPhi()   const { return widthBinPhi_;   }

public:

  Doubles rangeLimit( Doubles ranges ) const;

  //TMTrackProducer_params
  const edm::ParameterSet paramsProducer_;
  const edm::ParameterSet paramsTP_;
  const edm::ParameterSet paramsStub_;
  const edm::ParameterSet paramsTracker_;
  const edm::ParameterSet paramsDTC_;
  const edm::ParameterSet paramsGP_;
  const edm::ParameterSet paramsHT_;
  const edm::ParameterSet paramsMHT_;
  const edm::ParameterSet paramsLR_;
  const edm::ParameterSet paramsDR_;
  const edm::ParameterSet paramsDemo_;

  // Producer
  const bool          useMCtruth_;
  const bool          enableHistos_;
  const string        outputString_;
  const edm::InputTag inputTagTP_;
  const edm::InputTag inputTagStub_;
  const edm::InputTag inputTagClusterTruth_;
  const edm::InputTag inputTagGenJet_;
  const double        speedOfLight_;
  const double        magneticField_;
  const double        chosenRofPhi_;
  const double        chosenRofZ_;
  const bool          useApproxB_;
  const double        bApprox_gradient_;
  const double        bApprox_intercept_;
  const bool          enableTruncation_;
  const int           numFramesInfra_;
  const int           numFramesTMP_;
  const bool          enableDigitize_;
  const int           widthDSPa_;
  const int           widthDSPb_;
  const int           widthDSPc_;
  const double        invPtToDphi_;
  const int           numFramesAlgoIn_;
  const int           numFramesAlgoOut_;

  // TP
  const double tpMinPt_;
  const double tpMaxEta_;
  const double tpMaxVertR_;
  const double tpMaxVertZ_;
  const double tpMaxD0_;
  const int    tpMinLayers_;
  const int    tpMinLayersPS_;
  const int    tpMinLayersMatch_;

  // Stub
  const bool   stubBendResReduced_;
  const double stubMaxEta_;
  const double stubMinPt_;
  const double stubBendRes_;
  const double stubMaxCot_;
  const double baseBend_;
  const int    widthBend_;
  const int    widthR_;
  const int    widthPhi_;
  const int    widthZ_;
  const int    widthRforLUT_;

  // Tracker
  const double  trackerOuterRadius_;
  const double  trackerInnerRadius_;
  const double  trackerHalfLength_;
  const double  trackerTransitionZ_;
  const double  trackerBeamWindowZ_;
  const int     trackerNumModules_;
  const int     trackerNumLayers_;
  const int     trackerMaxLayerId_;
  const int     trackerNumLayersBarrel_;
  const int     trackerNumLayersEndcap_;
  const int     trackerNumModuleTypes_;
  const Doubles trackerPitches_;
  const Doubles trackerSeparations_;
  const Doubles trackerRangesBarrel_;
  const Doubles trackerRangesEndcap_;
  const Ints    trackerNumRangesModuleType_;
  const Doubles trackerRangesModuleTypeR_;
  const Doubles trackerRangesModuleTypeZ_;
  const Doubles trackerLimitsBarrel_;
  const Doubles trackerLimitsEndcap_;

  // DTC 
  const int    dtcNumRegions_;
  const int    dtcNum10GPS_;
  const int    dtcNum5GPS_;
  const int    dtcNum2S_;
  const bool   dtcOrderStubsByBend_;
  const int    dtcNumPerRegion_;
  const int    dtcNumPerTFP_;
  const int    dtcLenString_;
  const Ints   dtcLinks_;

  // GP
  const int     gpNumSectorsPhi_;
  const Doubles gpBounderiesEta_;
  const int     gpNumSectorsEta_;
  const Doubles gpAlphas_;
  const Doubles gpBetas_;
  const int     gpLenString_;
  const Ints    gpLinks_;

  // HT
  const double htMinPt_;
  const int    htNumBinsPt_;
  const int    htNumBinsPhi_;
  const int    htMinLayers_;
  const int    htSizeBuffer_;
  const int    htNumCells_;
  const int    htLenString_;
  const Ints   htLinks_;

  // MHT
  const int  mhtNumBinsPt_;
  const int  mhtNumBinsPhi_;
  const int  mhtMinLayers_;
  const int  mhtNumCells_;
  const int  mhtLenString_;
  const Ints mhtLinks_;

  // LR
  const int    lrNumIterations_;
  const int    lrMinLayers_;
  const int    lrMinLayersPS_;
  const double lrResidPhi_;
  const double lrResidZ2S_;
  const double lrResidZPS_;
  const int    lrLenString_;
  const Ints   lrLinks_;

  // DR
  const bool drStrict_;
  const int  drLenString_;
  const Ints drLinks_;

  //DemoSpec
  const bool   demoEnable_;
  const string demoStageIn_;
  const string demoStageOut_;
  const int    demoWidthFrame_;
  const string demoDirModelSim_;
  const int    offsetDTCGP_;
  const int    offsetDTCHT_;
  const int    offsetHTMHT_;
  const double runTimeDTCGP_;
  const double runTimeDTCHT_;
  const double runTimeHTMHT_;

  // rest
  Doubles bounderiesCot_;
  Doubles cots_;
  int     numSectorsPhi_;
  int     numSectors_;
  double  baseRegion_;
  double  baseSector_;
  double  baseM_;
  double  baseC_;
  double  baseCFull_;
  double  baseR_;
  double  baseRforLUT_;
  double  basePhi_;
  double  baseZ_;
  double  baseCotTheta_;
  double  baseInvR_;
  double  baseCot_;
  double  baseDCot_;
  double  baseAlpha_;
  double  baseBeta_;
  double  baseRMoB_;
  double  baseMoB_;
  int     widthEta_;  
  int     widthCBin_;  
  int     widthMBin_; 
  int     widthLayer_;
  int     widthChiPhi_;
  int     widthChiZ_;
  int     widthBinPt_;
  int     widthBinPhi_;

};

}

#endif
