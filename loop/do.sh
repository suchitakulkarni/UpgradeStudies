#!/bin/bash

make -j 8 || return $?

OUTDIR=output/test
LOGDIR=logs/
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

#most recent MC
INDIR=/eos/cms/store/user/gzevi/UpgradeSkims/advanced5May17/
#INDIR=~gzevi/public/UpgradeSkims/
declare -a Samples=(BB-4p-300-700)
#declare -a Samples=(BB-4p-0-300 BB-4p-300-700 BB-4p-700-1300 BB-4p-1300-2100 BB-4p-2100-100000 BBB-4p-0-600 BBB-4p-600-1300 BBB-4p-1300-100000 )

for SAMPLE in ${Samples[@]}; do

    # Get the xsec (in fb) and nevts   <-- Doesn't work for signal yet. Hack below
    # weight = (xsect*lumi)/nevents
    XSEC=`grep ${SAMPLE} configSkimmer.txt | awk '{print $6*1}'` # [fb]
    NEVTS=`grep ${SAMPLE} configSkimmer.txt | awk '{print $7*1}'`
    echo $XSEC, $NEVTS

    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS}
    eval "nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

declare -a Samples=(rns_c2n4_decayedALL)

for SAMPLE in ${Samples[@]}; do

    XSEC=250.72 # from Suchita's email [fb]
    NEVTS=30000
    echo $XSEC, $NEVTS

    echo nice -n 10 ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS}
    eval "nohup ./runLooper ${INDIR} ${SAMPLE} ${OUTDIR} ${XSEC} ${NEVTS} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done
