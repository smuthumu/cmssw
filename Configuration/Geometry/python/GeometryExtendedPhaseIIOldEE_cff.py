import FWCore.ParameterSet.Config as cms
#
# Geometry master configuration
#
# Ideal geometry, needed for simulation
from Geometry.CMSCommonData.cmsExtendedGeometryPhaseIIOldEEXML_cfi import *
from Geometry.TrackerNumberingBuilder.trackerNumberingSLHCGeometry_cfi import *
from Geometry.HcalCommonData.hcalSimNumberingInitialization_cfi import *

# Reconstruction geometry services
#  Tracking Geometry
#bah - well, this is not a cfi!
from Geometry.CommonDetUnit.globalTrackingSLHCGeometry_cfi import *

#Tracker
from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *
from Geometry.TrackerNumberingBuilder.trackerTopologyConstants_cfi import *

#Muon
from Geometry.MuonNumbering.muonNumberingInitialization_cfi import *
from RecoMuon.DetLayers.muonDetLayerGeometry_cfi import *
from Geometry.GEMGeometry.gemGeometry_cfi import *

#  Alignment
from Geometry.TrackerGeometryBuilder.idealForDigiTrackerSLHCGeometry_cff import *
from Geometry.CSCGeometryBuilder.idealForDigiCscGeometry_cff import *
from Geometry.DTGeometryBuilder.idealForDigiDtGeometry_cff import *
trackerSLHCGeometry.applyAlignment = cms.bool(False)

#  Calorimeters
from Geometry.CaloEventSetup.CaloTopology_cfi import *

from Geometry.CaloEventSetup.CaloGeometryBuilder_cfi import *

from Geometry.EcalAlgo.EcalGeometry_cfi import *
from Geometry.HcalEventSetup.HcalGeometry_cfi import *
from Geometry.HcalEventSetup.CaloTowerGeometry_cfi import *
from Geometry.HcalEventSetup.CaloTowerTopology_cfi import *
from Geometry.HcalEventSetup.HcalTopology_cfi import *
from Geometry.ForwardGeometry.ForwardGeometry_cfi import *
from Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi import *
from Geometry.EcalMapping.EcalMapping_cfi import *
from Geometry.EcalMapping.EcalMappingRecord_cfi import *
from Geometry.HcalCommonData.hcalRecNumberingInitialization_cfi import *
