# Install git code at lxplus

This code is running outside of CMSSW, but if you want to install it at lxplus, you have to setup environment.
The trick solution is to go to any CSMSW and set up environment there, for example:
```
cd CMSSW_10_6_0/src
cmsenv
```
return to your working directory and download MadGraph repository:
```
git clone https://github.com/NaturalHiggsMass/MadGraph.git
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

# MadGraph

## Install MadGraph and all components outside of this repository:

   - Download **MadGraph5_aMC@NLO** from official site: [MadGraph site](http://madgraph.physics.illinois.edu/)
   - Unpack it: 
``` 
tar -xvvf MG5_aMC_v2.6.3.2.tar.gz
```
   - Run MadGraph: 
```
    MG5_aMC_v2_6_3_2/bin/mg5_aMC 
```
   - Inside MadGrap install Pythia8, Delphes and ExRootAnalysis 
(In principle tab helps to make a choice of available programs. If something is missing for installation, you will get help information; please, follow it.):
```
install pythia8
install Delphes
install ExRootAnalysis
```

## Generate Higgs --> GammaGamma in MadGraph:

   - Instructions are coming...

## Generate GammaGamma background in MadGraph:

   - Instructions are coming...

## Analyze MC with Delphes:

   - Instructions are coming...
   
