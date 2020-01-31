#ifndef __TMTRACKPRODUCERFwd_H__
#define __TMTRACKPRODUCERFwd_H__

#include "FWCore/Framework/interface/EDProducer.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
//#include "CondFormats/Phase2TrackerObjects/interface/OuterTrackerDTCCablingMap.h"
//#include "CondFormats/DataRecord/interface/OuterTrackerDTCCablingMapRcd.h"
#include "SimTracker/Common/interface/TrackingParticleSelector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
//#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

namespace TMTT {

  typedef edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > > DetSetVec;
  typedef TTStubAssociationMap< Ref_Phase2TrackerDigi_ >           TTStubAssMap;
  typedef TTClusterAssociationMap< Ref_Phase2TrackerDigi_ >        TTClusterAssMap;
  typedef std::vector< TTTrack< Ref_Phase2TrackerDigi_ > >         TTTrackCollection;
  typedef std::unique_ptr< TTTrackCollection >                     TTTracksPtr;

  typedef edmNew::DetSet< TTStub< Ref_Phase2TrackerDigi_ > >                                                           DetSet;
  typedef edm::Ref< DetSetVec, TTStub< Ref_Phase2TrackerDigi_ > >                                                      TTStubRef;
  typedef edm::Ref< edmNew::DetSetVector< TTCluster< Ref_Phase2TrackerDigi_ > >, TTCluster< Ref_Phase2TrackerDigi_ > > TTClusterRef;
  typedef edm::Ptr< TrackingParticle >                                                                                 TrackingParticlePtr;

}

#endif