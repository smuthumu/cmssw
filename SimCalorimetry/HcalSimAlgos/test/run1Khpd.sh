#!/bin/bash

cmsRun step2hpdprec1K_DIGI_L1_DIGI2RAW_HLT.py > step2hpdprec1K_HcalHPD2ntuple.log 2>&1
root -b -l -q 'LogToTreeHPD2.C+("step2hpdprec1K_HcalHPD2ntuple")'
root -b -l -q 'plotPulses.C+("step2hpdprec1K_HcalHPD2ntuple.root")'

FILE=histos_step2hpdprec1K_HcalHPD2ntuple.root
NAME=step2hpdprec1K
HPD=1
root -b -l -q 'comparePreciseSamplesE.C+("'${FILE}'","'${NAME}'",0,'${HPD}')'

