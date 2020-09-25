# Root file describtion, produced at this directory:

`HiggsAnalysis_14TeV.C` produces the following root-files:

```
Background_14TeV.root
Signal_14TeV_mH123GeV.root
Signal_14TeV_mH125GeV.root
Signal_14TeV_mH127GeV.root
```

Files above are combine in one file for Combine tool (see Combine directory) with `ChainBins.C` (see instructions below):

```
HiggsMass_14TeV_Combine_ChainBins.root
```

All root-files above are available at CERN box directory:

```
/eos/user/k/kropiv/root-files/HiggsMass/root-Analysis
```


# Higgs signal and background generated in pT bins for 14 TeV:

`/eos/user/k/kropiv/root-files/HiggsMass/root-files-14TeV`

You could find links to the real files in that direcotry. To produce new links you have examples in that directory: `labelMass127.sh` and `label_pp2aa.sh`.

# To run code and produce pT bin distributions and root-files for each Signal and background:


```
root.exe -b 
.L HiggsAnalysis_14TeV.C
HiggsAnalysis_14TeV t
t.GetEntry(-1); 
t.Loop();
```
or

```
root -b RunMe.cxx++
```
it came from nice discussion here: `https://root-forum.cern.ch/t/compile-and-run-from-command-prompt/13799/7`


# Chain all signals and backgound and run merge bins program:

```
hadd HiggsMass_CombineTest.root Signal_14TeV_mH125GeV.root Signal_14TeV_mH123GeV.root Signal_14TeV_mH127GeV.root Background_14TeV.root
root.exe -b
.x ChainBins.C 
```

# Estimate Rate of Higgs and Background for 3000 fb-1 and produce bg+signal plots for paper:

```
root.exe -b 
.x HiggsRateEstimation_14TeV.C 
```

# To create MyClass from severel files for Signal (check max size of variables each time, when you add new file!):

You do it only if you generate new Delphes root-files to check if max size of each branching is not exceed, which is listed at `HiggsAnalysis_14TeV.h`
or if you want creat new code:

```
TChain chain("Delphes");
chain.Add("/eos/user/k/kropiv/root-files/HiggsMass/root-files-14TeV/run*.root");
chain.MakeClass("HiggsAnalysis_NewName");
```
or for Background:
```
chain.Add("/eos/cms/store/user/kropiv/HiggsMass/BackGroundGG/root-files/run*.root");

```

# Higgs signal generated for 13 TeV in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/HiggsGG/root-files`

# Gamma-Gamma background generated for 13 TeV in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/BackGroundGG/root-files/`
