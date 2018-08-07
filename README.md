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

