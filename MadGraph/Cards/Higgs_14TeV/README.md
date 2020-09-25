# Info for 14 TeV generation

## Madgraph version:
```
version = 2.7.0
date = 2020-01-20
```
## Data cards for Signal generation:

in `Signal` directory

Default Higgs mass is 125 GeV.
To generate 123 and 127 GeV Higgs mass, change it at `param_card.dat`

To generate Higgs pT bins, change `pt_min_pdg` and `pt_max_pdg` at `run_card.dat`

## Data cards for Background generation:

in `Background` directory

To generate gamma-gamma pT bins, change `htjmin` and `htjmax` at `run_card.dat`

## Data cards for Delphes reconstruction:

`Delphes-3.4.2` was installing seperate from Madgraph

* to process Signal `run_Delphes.sh` is used
* to process Background `run_Delphes.sh` is used

Modified `delphes_card_HLLHC.tcl` for ECAL resolution is used in this study.



