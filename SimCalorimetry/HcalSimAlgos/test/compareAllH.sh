#!/bin/bash

FILE1=histos_step2prec10K_HcalSiPMntuple.root
FILE2=histos_step2prec10Ktof_HcalSiPMntuple.root
NAME1=step2prec10K
NAME2=step2prec10Ktof

for i in {0..7}; do
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},0,100,'${i}')'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},100,200,'${i}')'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},200,300,'${i}')'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},300,400,'${i}')'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},400,500,'${i}')'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},500,600,'${i}')'
done

FILE1=histos_step2hpdprec10K_HcalHPD2ntuple.root
FILE2=histos_step2hpdprec10Ktof_HcalHPD2ntuple.root
NAME1=step2hpdprec10K
NAME2=step2hpdprec10Ktof

for i in {0..7}; do
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},0,100,'${i}',1)'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},100,200,'${i}',1)'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},200,300,'${i}',1)'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},300,400,'${i}',1)'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},400,500,'${i}',1)'
	root -b -l -q 'comparePreciseSamplesH.C+({"'${FILE1}'","'${FILE2}'"},{"'${NAME1}'","'${NAME2}'"},500,600,'${i}',1)'
done
