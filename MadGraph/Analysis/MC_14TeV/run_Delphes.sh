for i in 6 7 8 9 
do
  source /cvmfs/sft.cern.ch/lcg/views/LCG_92/x86_64-slc6-gcc62-opt/setup.sh
  
  echo "Copy hepmc file from run_0${i}"
  cp ../HiggsMass/Higgs_0or1Jet_PtCut/Events/run_0${i}/tag_1_pythia8_events.hepmc.gz ./
  echo "Upzip hepmc file"
  gunzip tag_1_pythia8_events.hepmc.gz
  
  echo "Run Delphes:"
  ./DelphesHepMC cards/delphes_card_HLLHC.tcl run0${i}.root tag_1_pythia8_events.hepmc > info_run0${i}_Delphes.txt
  
  echo "mv root file to original directory:"
  mv run0${i}.root ../HiggsMass/Higgs_0or1Jet_PtCut/Events/run_0${i}/
  mv info_run0${i}_Delphes.txt ../HiggsMass/Higgs_0or1Jet_PtCut/Events/run_0${i}/
  echo "rm hepmc file:"
  rm tag_1_pythia8_events.hepmc

done
