#!/bin/bash

bin_array=(PT_0_80 PT_80_120 PT_120_200 PT_200_270 PT_270_350 PT_350_450 PT_450_550 PT_550_750 PT_750_INF)
kfact_array=(1.2 1.2 1.2 1.2 1.2 1.2 1.2 1.2 1.2)


RES=1p2

for ibin in {0..8}
do
    BIN=${bin_array[${ibin}]}
    KFACT=${kfact_array[${ibin}]}

    TOYFILE=higgsCombineMass.GenerateOnly.mH125.BIN${BIN}.RES${RES}.123456.root
    OUTFILE=higgsCombineMass.MultiDimFit.mH125.BIN${BIN}.RES${RES}.root
    OUTFILERANGE=higgsCombineMassRange.MultiDimFit.mH125.BIN${BIN}.RES${RES}.root


    COMMON="-m 125 --keyword-value BIN=${BIN}  --keyword-value RES=${RES} --setParameters kfact=${KFACT}"

    GENERATING="combine HiggsMassDatacard.txt -M GenerateOnly --toysFrequentist -t -1 --expectSignal 1 --saveToys --freezeNuisanceGroups=all -n Mass ${COMMON}"
    echo "ASIMOV GENERATION: " $GENERATING
    eval $GENERATING

    LIKELIHOOD="combine HiggsMassDatacard.txt -M MultiDimFit --algo grid --points 50 -P Mass --setParameterRanges Mass=-1,1 -t -1 --floatOtherPOIs=1 --toysFile ${TOYFILE} -n Mass ${COMMON}"

    echo "LIKELIHOOD SCAN: " $LIKELIHOOD
    eval $LIKELIHOOD

    MASSPRECISION="combine HiggsMassDatacard.txt -M MultiDimFit --algo singles --cl=0.68 --points 50 -P Mass --setParameterRanges Mass=-1,1 -t -1 --floatOtherPOIs=1 --toysFile ${TOYFILE} -n MassRange ${COMMON}"

    echo "MASS UNCERTAINTY:" ${MASSPRECISION}
    eval ${MASSPRECISION}

    echo "PRODUCED FILES: " $TOYFILE, $OUTFILE, $OUTFILERANGE
done

