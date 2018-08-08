# Relevant analysis

- [CADI SMP-13-001: Diphoton differential cross sections at 7 TeV](http://cms.cern.ch/iCMS/analysisadmin/cadilines?id=1071&ancode=SMP-13-001&tp=an&line=SMP-13-001)

# Relevant data cards at CMS

- [Card at github `GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_UpPS/RunIISummer15wmLHEGS-MCRUN2_71_V1-v1/GEN-SIM`](https://github.com/cms-sw/genproductions/tree/7282e21844f34c9ce1c242356bd78443593d80a6/bin/MadGraph5_aMCatNLO/cards/production/13TeV/higgs/ggh012j_5f_NLO_FXFX_125)
- [Card at github `/DiPhotonJetsBox_MGG-80toInf_14TeV-Sherpa/PhaseIISummer17GenOnly-93X_upgrade2023_realistic_v5-v1/GEN`](https://github.com/cms-sw/genproductions/blob/91a7e73b1ffb7bf16f9c68997e7200c2ab8b9849/bin/Sherpa/cards/production/2017/13TeV/higgs/Run.dat_13TeV_gamgam_3j_loop_Mgg80-13000)

# Some relative links

- [Ntuplized for Delphes at GitHub](https://github.com/fabio-mon/PhaseTwoAnalysis/tree/master/delphesInterface/ntupler)
- GammaGammaJets Delphes background at uscms: `/eos/uscms/store/user/snowmass/noreplica/YR_Delphes/Delphes342pre15_prod-Luca/DiPhotonJetsBox_MGG-80toInf_14TeV-Sherpa_200PU`
# Install git code at lxplus

This code is running outside of CMSSW, but if you want to install it at lxplus, you have to setup environment.
The trick solution is to go to any CSMSW and set up environment there, for example:
```
cd CMSSW_10_6_0/src
cmsenv
```
return to your working directory and download MadGraph repository:
```
git clone https://github.com/NaturalHiggsMass/HiggsMassMC.git
```

You have to setup environment each time, when you want run any git commands, like:
* Update from github:
```
git pull
```
* Commit to githup:
```
git add fileName
git commit -m"info text here"
git push
```
# Install root at MAC OS

- Install root
- Each time run a script to make root initiallisation:
```
source /Application/RootDir/bin/thisroot.sh
```
- When you start root 1st time you could get an error, related to `LC_ALL`
- Run command, which will install missing parts:
```
echo | LC_ALL=C clang++ -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fno-common -Wcast-qual -fno-strict-aliasing -xc++ -E -v - 2>&1
```
