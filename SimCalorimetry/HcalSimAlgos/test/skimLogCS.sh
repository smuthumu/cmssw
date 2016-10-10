#!/bin/bash

LOG=$1
OUT=$2

grep "CaloSample " ${LOG} | grep -v "MSG" >& ${OUT}
