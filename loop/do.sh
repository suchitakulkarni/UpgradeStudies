#!/bin/bash

make -j 8 || return $?

OUTDIR=output/test
LOGDIR=logs/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

#most recent MC
#INDIR=/home/gzevi/UpgradeSkims/
INDIR=~gzevi/public/UpgradeSkims/
#declare -a Samples=(BB-4p-300-700)
declare -a Samples=(BB-4p-0-300 BB-4p-300-700 BB-4p-700-1300 BB-4p-1300-2100 BB-4p-2100-100000 BBB-4p-0-600 BBB-4p-600-1300 BBB-4p-1300-100000)

for SAMPLE in ${Samples[@]}; do

    # Get the xsec (in pb) and nevts   <-- Doesn't work for signal yet. Need a hack.
    # weight = (xsect*lumi)/nevents
    XSEC=`grep ${SAMPLE} configSkimmer.txt | awk '{print $6*1}'`
    NEVTS=`grep ${SAMPLE} configSkimmer.txt | awk '{print $7*1}'`
    echo $XSEC, $NEVTS

    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS}
    eval "nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done
