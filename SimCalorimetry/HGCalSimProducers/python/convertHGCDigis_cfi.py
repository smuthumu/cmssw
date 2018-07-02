import FWCore.ParameterSet.Config as cms

# converts old format of HGC digis to new format (same data, just different DetId type)
# allows processing old files
def convertHGCDigis(process):
    from SimCalorimetry.HGCalSimProducers.HGCDigiConverter_cfi import HGCDigiConverter
    process.HGCDigiConverter = HGCDigiConverter.clone()
    
    # alias output collections
    process.mix = cms.EDAlias(
        HGCDigiConverter = cms.VPSet(
            # this is the "friendly name" for HGCalDigiCollection
            cms.PSet(type = cms.string('DetIdHGCSampleHGCDataFramesSorted'))
        )
    )

    # make sure it can be found
    if len(process.tasks_())>0: process.tasks_()[list(process.tasks_().keys())[0]].add(process.HGCDigiConverter)

    return process
