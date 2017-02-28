import FWCore.ParameterSet.Config as cms

def hbheNZS(module):
    return module.clone(
        dropZSmarkedPassed = cms.bool(False),
        recoParamsFromDB = cms.bool(False),
        algorithm = dict(
            useM2 = cms.bool(False),
            useM3 = cms.bool(False)
        ),
    )

import RecoLocalCalo.Configuration.hcalLocalReco_cff
hbherecoMB = hbheNZS(RecoLocalCalo.Configuration.hcalLocalReco_cff.hbheprereco)

import RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hf_cfi
hfrecoMB = RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hf_cfi.hfreco.clone()

import RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_ho_cfi
horecoMB = RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_ho_cfi.horeco.clone()

# switch off "Hcal ZS in reco":
hfrecoMB.dropZSmarkedPassed = cms.bool(False)
horecoMB.dropZSmarkedPassed = cms.bool(False)

hcalLocalRecoSequenceNZS = cms.Sequence(hbherecoMB*hfrecoMB*horecoMB) 

_phase1_hbherecoMB = hbheNZS(RecoLocalCalo.Configuration.hcalLocalReco_cff._phase1_hbheprereco)
import RecoLocalCalo.HcalRecProducers.hfprereco_cfi
import RecoLocalCalo.HcalRecProducers.HFPhase1Reconstructor_cfi
hfprerecoMB = RecoLocalCalo.HcalRecProducers.hfprereco_cfi.hfprereco.clone(
    dropZSmarkedPassed = cms.bool(False)
)
_phase1_hfrecoMB = RecoLocalCalo.HcalRecProducers.HFPhase1Reconstructor_cfi.hfreco.clone(
    inputLabel = cms.InputTag("hfprerecoMB"),
    setNoiseFlags = cms.bool(False),
    algorithm = dict(
        Class = cms.string("HFSimpleTimeCheck"),
        rejectAllFailures = cms.bool(False)
    ),
)
from RecoLocalCalo.HcalRecProducers.hbheplan1_cfi import hbheplan1

_phase1_hcalLocalRecoSequenceNZS = hcalLocalRecoSequenceNZS.copy()
_phase1_hcalLocalRecoSequenceNZS.insert(0,hfprerecoMB)

from Configuration.Eras.Modifier_run2_HF_2017_cff import run2_HF_2017
run2_HF_2017.toReplaceWith( hcalLocalRecoSequenceNZS, _phase1_hcalLocalRecoSequenceNZS )
run2_HF_2017.toReplaceWith( hfrecoMB, _phase1_hfrecoMB )
from Configuration.Eras.Modifier_run2_HCAL_2017_cff import run2_HCAL_2017
run2_HCAL_2017.toReplaceWith( hbherecoMB, _phase1_hbherecoMB )

_plan1_hcalLocalRecoSequenceNZS = _phase1_hcalLocalRecoSequenceNZS.copy()
_plan1_hcalLocalRecoSequenceNZS += hbheplan1
from Configuration.Eras.Modifier_run2_HEPlan1_2017_cff import run2_HEPlan1_2017
run2_HEPlan1_2017.toReplaceWith(hcalLocalRecoSequenceNZS, _plan1_hcalLocalRecoSequenceNZS)
