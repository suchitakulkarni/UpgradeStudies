#! /bin/bash

LOGDIR=logs
mkdir -p ${LOGDIR}


OUTPATH=/home/gzevi/UpgradeSkims
#OUTPATH=.
mkdir -p ${OUTPATH}

# Basil's skims
#INPATH=root://eoscms.cern.ch//eos/cms/store/user/bschneid/SUSY_Upgrade_Skimmer/output12_BB
#declare -a Samples=(qcd_ht ttsl ttdl singletop ttw_mg_lo ttz_mg_lo ttg dyjetsll_ht wjets_ht gjets_dr0p05_ht zinv_ht)
#declare -a Samples=(BB0_ntuple)


# Central samples
INPATH=root://eoscms.cern.ch//eos/cms/store/group/upgrade/delphes_output/DelphesFromLHE_333pre16_2016Aug/

#declare -a Samples=(BB-4p-0-300-v1510_14TEV_200PU)
declare -a Samples=(BB-4p-0-300-v1510_14TEV_200PU BB-4p-1300-2100-v1510_14TEV_200PU BB-4p-2100-100000-v1510_14TEV_200PU BB-4p-300-700-v1510_14TEV_200PU BB-4p-700-1300-v1510_14TEV_200PU)
#declare -a Samples=(BBB-4p-0-600-v1510_14TEV_200PU BBB-4p-1300-100000-v1510_14TEV_200PU BBB-4p-600-1300-v1510_14TEV_200PU)

## Submit without using wildcard: figure out names of files in each directory, submit one job per file
for SAMPLE in ${Samples[@]}
  do
    echo Loopling over ${INPATH}/${SAMPLE}
    for FILE in `eos ls ${INPATH}/${SAMPLE}/`
    do
#	echo $FILE
	echo root -b -l -q skim.C\(\"$INPATH/${SAMPLE}\",\"$OUTPATH\",\"$FILE\"\)
    	nohup root -b -l -q skim.C\(\"$INPATH/${SAMPLE}\",\"$OUTPATH\",\"$FILE\"\) >& ${LOGDIR}/log_${FILE}.txt &
    done
done

## Submit with wildcards
#for SAMPLE in ${Samples[@]}
##  do root -b -l -q skim.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& ${LOGDIR}/log_${SAMPLE}.txt &
#  do root -b -l -q skimslim.C\(\"$INPATH\",\"$OUTPATH\",\"$SAMPLE\"\) >& ${LOGDIR}/log_${SAMPLE}.txt &
#done

