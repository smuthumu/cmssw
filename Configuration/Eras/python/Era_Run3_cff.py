import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018
from Configuration.Eras.Modifier_run3_common_cff import run3_common
from Configuration.Eras.Modifier_run3_GEM_cff import run3_GEM
from Configuration.Eras.Modifier_run3_HB_cff import run3_HB

Run3 = cms.ModifierChain(Run2_2018, run3_common, run3_GEM, run3_HB)

