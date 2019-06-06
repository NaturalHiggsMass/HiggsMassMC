# To run code

```
.L Research_1.C
Research_1 t
t.GetEntry(-1)
t.Loop()
hHiggsPt->Draw();
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
t.GetEntry(12); // Fill t data members with entry number 12
t.Show();       // Show values of entry 12
t.Show(16);     // Read and show values of entry 16
t.Loop();       // Loop on all entries
```
# Higgs signal generated in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/HiggsGG/root-files`

# Gamma-Gamma background generated in pT bins:

`/eos/cms/store/user/kropiv/HiggsMass/BackGroundGG/root-files/`
