#! /bin/bash

LOGDIR=logs
mkdir -p ${LOGDIR}


OUTPATH=/home/gzevi/UpgradeSkims/advanced5May17
#OUTPATH=.
mkdir -p ${OUTPATH}

# Basil's skims
#INPATH=root://eoscms.cern.ch//eos/cms/store/user/bschneid/SUSY_Upgrade_Skimmer/output12_BB
#declare -a Samples=(BB0_ntuple)

## Central samples
INPATH=root://eoscms.cern.ch//eos/cms/store/group/upgrade/delphes_output/DelphesFromLHE_333pre16_2016Aug/
##declare -a Samples=(BB-4p-0-300-v1510_14TEV_200PU)
declare -a Samples=(BB-4p-0-300-v1510_14TEV_200PU BB-4p-1300-2100-v1510_14TEV_200PU BB-4p-2100-100000-v1510_14TEV_200PU BB-4p-300-700-v1510_14TEV_200PU BB-4p-700-1300-v1510_14TEV_200PU)
#
## Suchita's C2N4 (eos)
##INPATH=root://eoscms.cern.ch//eos/cms/store/cmst3/group/susy/upgrade/C2N4_delphised/
##declare -a Samples=(delphised_substructure_200PU) # not a very descriptive name unfortunately
#
#
## Submit without using wildcard: "Samples" contains folder names, but then
## we figure out filenames in each folder and submit one job per file
for SAMPLE in ${Samples[@]}
  do
    echo Loopling over ${INPATH}/${SAMPLE}
    for FILE in `eos ls ${INPATH}/${SAMPLE}/`
    do
#	echo $FILE
	echo root -b -l -q advancedskim.C\(\"$INPATH/${SAMPLE}\",\"$OUTPATH\",\"$FILE\"\)
    	nohup root -b -l -q advancedskim.C\(\"$INPATH/${SAMPLE}\",\"$OUTPATH\",\"$FILE\"\) >& ${LOGDIR}/log_${FILE}.txt &
    done
done

# Suchita's C2N4 (local)
INPATH=/home/gzevi/C2N4delphes/
declare -a Samples=(C2N4full) # not a very descriptive name unfortunately

## Submit with wildcards. IF eos is mounted, then this work and can submit one job per folder
for SAMPLE in ${Samples[@]}
do
    echo root -b -l -q advancedskim.C\(\"$INPATH/${SAMPLE}\",\"$OUTPATH\",\"$FILE\"\)
    root -b -l -q advancedskim.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& ${LOGDIR}/log_${SAMPLE}.txt &
done

