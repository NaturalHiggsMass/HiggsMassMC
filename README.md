#Relevant analysis

- [CADI SMP-13-001: Diphoton differential cross sections at 7 TeV](http://cms.cern.ch/iCMS/analysisadmin/cadilines?id=1071&ancode=SMP-13-001&tp=an&line=SMP-13-001)

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
