#!/bin/bash

bin_array=(PT_0_80 PT_80_120 PT_120_200 PT_200_270 PT_270_350 PT_350_450 PT_450_550 PT_550_750 PT_750_INF)

for ibin in {0..8}
do
    BIN=${bin_array[${ibin}]}

    TOYFILE=higgsCombineMass.GenerateOnly.mH125.BIN${BIN}.123456.root
    OUTFILE=higgsCombineMass.MultiDimFit.mH125.BIN${BIN}.root


    combine HiggsMassDatacard.txt -M GenerateOnly --toysFrequentist -t -1 --expectSignal 1 --saveToys --freezeNuisanceGroups=all -n Mass -m 125 --keyword-value BIN=${BIN}

combine HiggsMassDatacard.txt -M MultiDimFit --algo grid --points 50 -P Mass --setParameterRanges Mass=-1,1 -t -1 --floatOtherPOIs=1 --toysFile ${TOYFILE} -n HiggsMass -m 125 --keyword-value BIN=${BIN}

    echo $TOYFILE, $OUTFILE
done

