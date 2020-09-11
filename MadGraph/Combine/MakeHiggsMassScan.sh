#!/bin/bash

# standard
#bin_array=(PT_0_80 PT_80_120 PT_120_200 PT_200_270 PT_270_350 PT_350_450 PT_450_550 PT_550_750 PT_750_INF)
#kfact_array=(1.2 1.2 1.2 1.2 1.2 1.2 1.2 1.2 1.2)

bin_array=(PT_0_120 PT_120_200 PT_200_270 PT_270_350 PT_350_450 PT_450_550 PT_550_750 PT_750_INF)
kfact_array=(1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0) # no scale from 13 TeV to 14 TeV
#kfactbkg_array=(1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0) # no scale from 13 TeV to 14 TeV
kfactbkg_array=(2.0 2.0 2.0 2.0 2.0 2.0 2.0 2.0) # no scale from 13 TeV to 14 TeV

# chain 0-120 and 550-INF bins
#bin_array=(PT_0_120 PT_120_200 PT_200_270 PT_270_350 PT_350_450 PT_450_550 PT_550_INF)
#kfact_array=(1.2 1.2 1.2 1.2 1.2 1.2 1.2)
#kfact_array=(2.5 2.5 2.5 2.5 2.5 2.5 2.5)
#kfactbkg_array=(1.0 1.0 1.0 1.0 1.0 1.0 1.0) # no scale from 13 TeV to 14 TeV
#kfactbkg_array=(2.0 2.0 2.0 2.0 2.0 2.0 2.0) # extream: 2 times more BKG no scale from 13 TeV to 14 TeV

# in pb: 45000/39600 873/747 208/175 86.9/72.2 39.8/32.6 15.2/12.2 13.4/10.4
#kfact_array=(1.0 1.0 1.0 1.0 1.0 1.0 1.0) # no scale from 13 TeV to 14 TeV
#kfact_array=(   1.14 1.17    1.9     1.20      1.22      1.25      1.29) #scale from 13 TeV to 14 TeV



#RES=1p2
RES=1p0

#for ibin in {0..8}
for ibin in {0..7}
#for ibin in {0..6}
do
    BIN=${bin_array[${ibin}]}
    KFACT=${kfact_array[${ibin}]}
    KFACT_BKG=${kfactbkg_array[${ibin}]}

    TOYFILE=higgsCombineMass.GenerateOnly.mH125.BIN${BIN}.RES${RES}.123456.root
    OUTFILE=higgsCombineMass.MultiDimFit.mH125.BIN${BIN}.RES${RES}.root
    OUTFILERANGE=higgsCombineMassRange.MultiDimFit.mH125.BIN${BIN}.RES${RES}.root


    #COMMON="-m 125 --keyword-value BIN=${BIN}  --keyword-value RES=${RES} --setParameters [kfact=${KFACT}, kfact_bkg=${KFACT_BKG}]"
    COMMON="-m 125 --keyword-value BIN=${BIN}  --keyword-value RES=${RES} --setParameters kfact=${KFACT},kfact_bkg=${KFACT_BKG}"
    #COMMON="-m 125 --keyword-value BIN=${BIN}  --keyword-value RES=${RES} --setParameters kfact=${KFACT}"

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

