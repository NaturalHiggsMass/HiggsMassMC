# To run code


```
.L HiggsAnalysis.C
HiggsAnalysis t
t.GetEntry(-1); 
t.Loop();
```

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


To create MyClass from severel files for Signal:
```
TChain chain("Delphes");
chain.Add("/eos/cms/store/user/kropiv/HiggsMass/HiggsGG/root-files/run*.root");
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
