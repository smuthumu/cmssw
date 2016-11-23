#!/bin/bash

root -l 'plotPulses.C+("step2prec10K_HcalSiPMntuple.root")'
root -l 'plotPulses.C+("step2prec10Ktof_HcalSiPMntuple.root")'
root -l 'plotPulses.C+("step2hpdprec10K_HcalHPD2ntuple.root")'
root -l 'plotPulses.C+("step2hpdprec10Ktof_HcalHPD2ntuple.root")'
