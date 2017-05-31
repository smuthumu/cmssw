import FWCore.ParameterSet.Config as cms

#common stuff
def load_HcalHardcode(process):
    process.load("CalibCalorimetry/HcalPlugins/Hcal_Conditions_forGlobalTag_cff")
    process.es_hardcode.toGet = cms.untracked.vstring(
                'GainWidths',
                'MCParams',
                'RecoParams',
                'RespCorrs',
                'QIEData',
                'QIETypes',
                'Gains',
                'Pedestals',
                'PedestalWidths',
                'ChannelQuality',
                'ZSThresholds',
                'TimeCorrs',
                'LUTCorrs',
                'LutMetadata',
                'L1TriggerObjects',
                'PFCorrs',
                'ElectronicsMap',
                'FrontEndMap',
                'CovarianceMatrices',
                'SiPMParameters',
                'SiPMCharacteristics',
                'TPChannelParameters',
                'TPParameters',
                'FlagHFDigiTimeParams',
                )

    # Special Upgrade trick (if absent - regular case assumed)
    process.es_hardcode.GainWidthsForTrigPrims = cms.bool(True)
                
    return process

def load_HcalAliases(process):
    #SO MUCH EASIER
    process.hcalDigis = cms.EDAlias(
        simHcalDigis =  cms.VPSet(
            cms.PSet(type = cms.string("HBHEDataFramesSorted")),
            cms.PSet(type = cms.string("HFDataFramesSorted")),
            cms.PSet(type = cms.string("HODataFramesSorted")),
            cms.PSet(
                type = cms.string('QIE10DataFrameHcalDataFrameContainer'),
                fromProductInstance = cms.string('HFQIE10DigiCollection'),
                toProductInstance = cms.string('')
            ),
            cms.PSet(
                type = cms.string('QIE11DataFrameHcalDataFrameContainer'),
                fromProductInstance = cms.string('HBHEQIE11DigiCollection'),
                toProductInstance = cms.string('')
            )
        ),
        simHcalTriggerPrimitiveDigis = cms.VPSet(
            cms.PSet(type = cms.string("HcalTriggerPrimitiveDigisSorted"))
        ),
    )

    # keep the content
    alist=['RAWDEBUG','FEVTDEBUG','FEVTDEBUGHLT','GENRAW','RAWSIMHLT','FEVT']
    for a in alist:
        b = a + 'output'
        if hasattr(process,b):
            getattr(process,b).outputCommands.extend([
                'keep *_hcalDigis_*_*',
                'drop *_simHcalDigis_*_*',
                'drop *_simHcalUnsuppressedDigis_*_*',
                'drop *_simHcalTriggerPrimitiveDigis_*_*',
                'keep ZDCDataFramesSorted_simHcalUnsuppressedDigis_*_*',
            ])

    return process

def customise_Hcal2019(process):
    process=load_HcalHardcode(process)
    process.es_hardcode.useHBUpgrade = cms.bool(True)
    process.es_hardcode.useHEUpgrade = cms.bool(True)
    process.es_hardcode.useHFUpgrade = cms.bool(True)

    if hasattr(process,'DigiToRaw'):
        process=customise_DigiToRaw(process)
    if hasattr(process,'RawToDigi'):
        process=customise_RawToDigi(process)
    if hasattr(process,'datamixing_step'):
        process=customise_mixing(process)
    if hasattr(process,'reconstruction_step'):
        process.zdcreco.digiLabel = cms.InputTag("simHcalUnsuppressedDigis")
        process.zdcreco.digiLabelhcal = cms.InputTag("simHcalUnsuppressedDigis")
    if hasattr(process,'validation_step'):
        process.AllHcalDigisValidation.dataTPs = cms.InputTag("hcalDigis")
        process.hltHCALdigisAnalyzer.dataTPs = cms.InputTag("hcalDigis")

    # needs to be after deletion of "real" hcalDigis
    process=load_HcalAliases(process)

    return process

def customise_DigiToRaw(process):
    process.digi2raw_step.remove(process.hcalRawData)

    return process

def customise_RawToDigi(process):
    process.raw2digi_step.remove(process.hcalDigis)

    return process

def customise_mixing(process):
    process.mixData.HBHEPileInputTag = cms.InputTag("simHcalUnsuppressedDigis")
    process.mixData.HOPileInputTag = cms.InputTag("simHcalUnsuppressedDigis")
    process.mixData.HFPileInputTag = cms.InputTag("simHcalUnsuppressedDigis")
    process.mixData.QIE10PileInputTag = cms.InputTag("simHcalUnsuppressedDigis","HFQIE10DigiCollection")
    process.mixData.QIE11PileInputTag = cms.InputTag("simHcalUnsuppressedDigis","HBHEQIE11DigiCollection")
    return process
