for i in 1 2 3 4 5 6 7 8 9 
do
  source /cvmfs/sft.cern.ch/lcg/views/LCG_92/x86_64-slc6-gcc62-opt/setup.sh
  
  echo "Copy hepmc file from run_0${i}"
  cp ../HiggsMass/pp2aa_01j/Events/run_0${i}/tag_1_pythia8_events.hepmc.gz ./
  ##cp ../HiggsMass/pp2aa_01j/Events/run_0${i}/tag_2_pythia8_events.hepmc.gz ./
  echo "Upzip hepmc file"
  gunzip tag_1_pythia8_events.hepmc.gz
  
  echo "Run Delphes:"
  ./DelphesHepMC cards/delphes_card_HLLHC.tcl run0${i}.root tag_1_pythia8_events.hepmc > info_run0${i}_Delphes.txt
  #./DelphesHepMC cards/delphes_card_HLLHC.tcl run0${i}.root tag_2_pythia8_events.hepmc > info_run0${i}_Delphes.txt
  
  echo "mv root file to original directory:"
  rm -rf ../HiggsMass/pp2aa_01j/Events/run_0${i}/run0${i}.root
  rm -rf ../HiggsMass/pp2aa_01j/Events/run_0${i}/info_run0${i}_Delphes.txt
  mv run0${i}.root ../HiggsMass/pp2aa_01j/Events/run_0${i}/
  mv info_run0${i}_Delphes.txt ../HiggsMass/pp2aa_01j/Events/run_0${i}/
  echo "rm hepmc file:"
  rm -rf tag_1_pythia8_events.hepmc

done
