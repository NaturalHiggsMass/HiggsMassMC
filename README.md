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

* Download **MadGraph5_aMC@NLO** from official site: [MadGraph site](http://madgraph.physics.illinois.edu/)
* Unpack it: 
``` 
tar -xvvf MG5_aMC_v2.6.3.2.tar.gz
```
* Run MadGraph: 
```
MG5_aMC_v2_6_3_2/bin/mg5_aMC 
```
* Inside MadGrap install Pythia8, Delphes and ExRootAnalysis 
(In principle tab helps to make a choice of available programs. If something is missing for installation, you will get help information; please, follow it.):
```
install pythia8
install Delphes
install ExRootAnalysis
```
ExRootAnalysis you need to generate root-file directyly (not lhc-file).

## Generate Higgs --> GammaGamma in MadGraph:

* Login to MadGrap:
```
MG5_aMC_v2_6_3_2/bin/mg5_aMC
```
* Load model:
```
import model heft-full
```
* Generate process (a is a gamma in MadGraph):
```
generate p p > h > a a
```
* Add additional processes to have Higgs with non-zero pT:
```
add process p p > h j, h > a a
```
* Generate cards in the directory (DirName = HiggsJet, for example)
```
output DirName
```
* Run command to edit cards:
```
launch
```

**Be careful:** If you press `Enter` it will go to next step automatically.

* After you sould switch on processes, 
by typing corresponiding numbers 
(The `analysis = ExRoot` should be on itself):
```
shower = Pythia8
detector = Delphes
```
* Press Enter and type to modify run\_card.dat:
```
1 = ickkw     ### by default it is 0, it turn on jet matching
20.0 = xqcut  ### by default it is 0 
```
`xqcut` sets the minimum kt jet measure between partons. 
Changing this value will cause you to keep more or
fewer Higgs bosons. You can play with it if you want.

You also could change number of generated events there (10K events by default).

* Don't edit the param\_card.dat

* Edit pythia\_card.dat. 
This must match the value for xqcut in the run card.
Add in the end of the file:
```
QCUT = 20.0
```
* Don’t edit delphes\_card.dat

* When you are done editing the card files, MadGraph will begin generating the processes by pressing `Enter`.
Once it finishes you will have a root-file that is stored in /Events/run 01/ in the directory you created at the beginning.


## Generate GammaGamma background in MadGraph:

   - Instructions are coming...

## Analyze MC with Delphes:

   - Instructions are coming...
   
