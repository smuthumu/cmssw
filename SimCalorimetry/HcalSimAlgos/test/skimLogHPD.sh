#!/bin/bash

LOG=$1
OUT=$2

grep "HcalHPDntuple" ${LOG} | grep -v "MSG" >& ${OUT}
