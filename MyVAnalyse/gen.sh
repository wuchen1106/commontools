#!/bin/bash

nEvents=1082116
nSplits=10

prefix="A9.mum.EW.g40cm10mm.003T.He071.g4s.QBH."

for (( i=0; i<nSplits; i++ ))
do
	bEvents=`echo "$i*$nEvents"|bc`
	eEvents=`echo "$bEvents+$nEvents"|bc`
	name=$prefix$i
	nohup ./MyVAnalyse -B $bEvents -n $eEvents -r $name &
done
