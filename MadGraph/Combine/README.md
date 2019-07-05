
The typical datacard is attached:

* Check that if you inject signal = 1, you extract signal = 1 and the associated uncertainty
```
combine HiggsMassDatacard.txt -M FitDiagnostics --expectSignal 1 -t -1 --freezeParameter Mass
```

* Generate asimov dataset with signal=1
```
combine HiggsMassDatacard.txt -M GenerateOnly --toysFrequentist -t -1 --expectSignal 1 --saveToys --freezeNuisanceGroups=all
```

* Use this asimov dataset to scan the mass always fitting the best signal strength and profiling other systematics:
```
combine HiggsMassDatacard.txt -M MultiDimFit --algo grid --points 50 -P Mass --setParameterRanges Mass=-1,1 -t -1 --floatOtherPOIs=1 --toysFile higgsCombineTest.GenerateOnly.mH120.123456.root
```

* Plot deltaLL as function of Mass parameter.
```
root -l higgsCombineTest.MultiDimFit.mH120.root
limit->Draw("2*deltaNLL:Mass>>Likelihood","","L", 50, 1);
x-axis scale: 0= 125 GeV; 1 = 127 GeV; -1 = 123 GeV
y-axis scale: 1 = 1 sigma; 4 = 2 sigma
```
* How to plot Likelihood (Example of bin 0-80 with 300 fb-1. No systematics. ~80 MeV precision):
```
limit->Draw("2*deltaNLL:Mass*2+125>>Likelihood","","PL", 50, 1);
TLine* L = new TLine(124.6, 1, 125.4, 1)
L->Draw()
```
## Useful links:

* [Combine Tool](https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/part3/runningthetool/#output-from-combine)

useful options
```
-keyword-value WORD=VALUE
-m 125
```
* [Istructions to install Combine](https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/)

Recommended version `CMSSW_10_2_X` and tag is `v8.0.1`

## Useful information:

* [update in CERN HL-LHC YR 2019](https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019)

N3LO QCD + NLO EV ggH xsection:
```
13 TeV: 48.61 pb
14 TeV: 54.72 pb
```
Error ~ 7%.

Madgraph and Delphes:
```
13 TeV: 40.65 +- 0.04 +0.05-0.21pb
```

* [Recommended predictions for the boosted-Higgs xsection (k-factors)](https://cds.cern.ch/record/2669113/files/LHCHXSWG-2019-002.pdf)

