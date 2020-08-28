# To run code


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

```
.L HiggsDistribution.C
HiggsDistribution t
t.GetEntry(-1)
t.Loop()
hHiggsPt->Draw();
```

To check proper simulation of the size of pT gen and reco bin:
```
.L NewData_Sept.C
NewData_Sept t
t.GetEntry(-1); 
t.Loop();
```


To create MyClass from severel files for Signal (check max size of variables each time, when you add new file!):
```
TChain chain("Delphes");
chain.Add("/eos/user/k/kropiv/root-files/HiggsMass/root-files-14TeV/run*.root");
chain.MakeClass("HiggsAnalysis_NewName");
```
or for Background:
```
chain.Add("/eos/cms/store/user/kropiv/HiggsMass/BackGroundGG/root-files/run*.root");

```

# Higgs signal generated in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/HiggsGG/root-files`

# Gamma-Gamma background generated in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/BackGroundGG/root-files/`
