#!/bin/bash

cmsRun step2prec1K_DIGI_L1_DIGI2RAW_HLT.py > step2prec1Kbin_HcalSiPMntuple.log 2>&1
root -b -l -q 'LogToTree.C+("step2prec1Kbin_HcalSiPMntuple")'
root -b -l -q 'plotPulses.C+("step2prec1Kbin_HcalSiPMntuple.root")'

FILE=histos_step2prec1Kbin_HcalSiPMntuple.root
NAME=step2prec1Kbin
root -b -l -q 'comparePreciseSamplesE.C+("'${FILE}'","'${NAME}'",0)'
