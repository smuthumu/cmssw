# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:phase1_2017_hcaldev -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@fake --datatier GEN-SIM-DIGI-RAW -n 100 --geometry Extended2017dev --era Run2_2017_HCALdev --eventcontent FEVTDEBUGHLT --filein file:step1.root --fileout file:step2.root --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT',eras.Run2_2017_HCALdev)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2017devReco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_Fake_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:step1.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

#process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.categories.append("HcalHPDntuple")
process.MessageLogger.categories.append("HcalHPD2ntuple")
process.MessageLogger.cout.threshold = cms.untracked.string("INFO")
process.MessageLogger.cout.INFO = cms.untracked.PSet(limit = cms.untracked.int32(0))
#process.MessageLogger.cout.DEBUG = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cout.HcalHPDntuple = cms.untracked.PSet(limit = cms.untracked.int32(-1))
process.MessageLogger.cout.HcalHPD2ntuple = cms.untracked.PSet(limit = cms.untracked.int32(-1))

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(10485760),
    fileName = cms.untracked.string('file:step2hpdpu.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(35.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring([
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/0EDC9546-FC8B-E611-A0C4-0025905B860E.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/1E2A782B-048C-E611-AFBB-0CC47A7C357E.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/222B375F-FD8B-E611-9285-0CC47A74527A.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/5CB0B308-FF8B-E611-A6BE-0025905B8606.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/6E736009-FF8B-E611-92B2-0025905A6104.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/740B725F-FE8B-E611-97D4-0CC47A78A42C.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/803F2E0E-FE8B-E611-8472-0025905B85DC.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/A844DBFD-FC8B-E611-AD40-0CC47A4D7632.root',
'root://eoscms.cern.ch//store/relval/CMSSW_8_1_0_pre12/RelValMinBias_13/GEN-SIM/81X_upgrade2017_HCALdev_v2_NewBPix_BpixHcalGeom-v1/00000/AA66122D-048C-E611-A7AD-0025905A609E.root',
])
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_hcaldev', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step])

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim 

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforFullSim(process)

# End of customisation functions

# Customisation from command line
from SLHCUpgradeSimulations.Configuration.HCalCustoms import load_HcalHardcode
process = load_HcalHardcode(process)
process.es_hardcode.useHFUpgrade = cms.bool(True)
process.es_hardcode.useHEUpgrade = cms.bool(False)
process.mix.digitizers.hcal.he.photoelectronsToAnalog = cms.vdouble([0.3305]*14)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(6)
process.options.numberOfStreams=cms.untracked.uint32(0)

