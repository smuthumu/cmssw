#!/bin/bash

LOG=$1
OUT=$2

grep "HcalSiPMntuple" ${LOG} | grep -v "MSG" >& ${OUT}
