#define Research_1_cxx
#include "Research_1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Research_1::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Research_1.C
//      root> Research_1 t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	TLorentzVector test(0.,0.,0.,0.);
	TLorentzVector p1(0.,0.,0.,0.);
	TLorentzVector p2(0.,0.,0.,0.);
	TLorentzVector s(0.,0.,0.,0.);	
	
// initialize histograms

	TH1D * bothBarrel[5];
	TH1D * barrelAndEndCaps[5];
	TH1D * bothEndCaps[5];
	
	std::string title[5] = {"pT(0,50 GeV)", "pT(50,100 GeV)", "pT(100,150 GeV)", "pT(150,200 GeV)", "pT(200+ GeV)"};
	std::string work;	
		
	for (int i = 0; i < 5; i++)
	{		
		work = "Histogram of Invariant Mass, both Photons Eta < 1.44, Higgs " + title[i];
	 	bothBarrel[i] = new TH1D(work.c_str(),work.c_str(), 100,110, 140);
		bothBarrel[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		bothBarrel[i]-> GetYaxis()->SetTitle("Number of Events");
		
		work = "Histogram of Invariant Mass, one Photon Eta < 1.44, other Photon > 1.57, Higgs " + title[i];
		barrelAndEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		barrelAndEndCaps[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		barrelAndEndCaps[i]-> GetYaxis()->SetTitle("Number of Events");		
		
		work = "Histogram of Invariant Mass, both Photons Eta > 1.57, Higgs " + title[i];
		bothEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		bothEndCaps[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		bothEndCaps[i]-> GetYaxis()->SetTitle("Number of Events");		
	}

	int indicator = 0;
	int flag = 0;
	bool check = 0;
	double mass = 0;

   	Long64_t nbytes = 0, nb = 0;
   	for (Long64_t jentry=0; jentry<nentries;jentry++) 
  	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;

// if (Cut(ientry) < 0) continue;
		
		p1 = test;
		p2 = test;
		check = 0;

		for (int i = 0; i < Photon_size; i++)
		{
			if ( abs(Photon_Eta[i]) <= 2.5 && (abs(Photon_Eta[i]) > 1.57 || abs(Photon_Eta[i]) < 1.44))
			{
				if (p1 == test) 	// if this is the first photon in the event
				{
					p1.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
				}
				else if ( (p1 != test) && (p2 == test) ) // if this is the second photon in the event
				{
					p2.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
				}
				else if ( (p1 != test) && (p2 != test) )	// maybe another photon, trying to get max pTs
				{
					if ( Photon_PT[i] > p1.Pt() )
					{
						if (p1.Pt() >= p2.Pt())
						{
							p2.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
						}
						else
						{
							p1.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
						}
					}
					else if ( Photon_PT[i] > p2.Pt() )
					{
						p2.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
					}
				}
			}
		}	// End of looping through photons
		
		for ( int i = 0; i < Particle_size; i++) // now loop through all particles to find Higgs
		{
			if ( Particle_PID[i] == 25)	//set indicator depending on Pt
			{
				if (abs(Particle_PT[i]) <= 50)
				{
					indicator = 1;
				}				
				else if (abs(Particle_PT[i]) <= 100)
				{
					indicator = 2;
				}
				else if (abs(Particle_PT[i]) <= 150)
				{
					indicator = 3;
				}
				else if (abs(Particle_PT[i]) <= 200)
				{
					indicator = 4;
				}				
				else if (abs(Particle_PT[i]) > 250)
				{
					indicator = 5;
				}
			}
		}
		
		if (p2 != test)			// if both photons filled
		{
			s = p1+p2;
			mass = s.M();
			
			if ( (p1.Pt() > mass/3) && (p2.Pt() > mass/4) )
			{
				check = 1;
				if ( abs(p1.Eta()) < 1.44 && abs(p2.Eta()) < 1.44 )
				{
					flag = 1;
				}
				else if (abs(p1.Eta()) > 1.57 && abs(p2.Eta()) > 1.57)
				{
					flag = 3;
				}
				else
				{
					flag = 2;
				}
			}
		}

		if (check == 1)		//fill the right histograms if the mass was recorded
		{
			if (flag == 1)
			{
				bothBarrel[indicator - 1] -> Fill(mass);
			}
			else if (flag == 2)
			{
				barrelAndEndCaps[indicator - 1] -> Fill(mass);
			}
			else if (flag == 3)
			{
				bothEndCaps[indicator - 1] -> Fill(mass);			
			}	
		}
	}

	TCanvas *first[5];
	TCanvas *second[5];	
	TCanvas *third[5];

	for (int i = 0; i < 5; i++) // draw and fit histograms
	{
		first[i] = new TCanvas();
		bothBarrel[i]->Draw();
		bothBarrel[i]->Fit("gaus","","",118,132);
	    gStyle->SetOptFit(1); 
	
		second[i] = new TCanvas();
		barrelAndEndCaps[i]->Draw();
		barrelAndEndCaps[i]->Fit("gaus","","",118,132);	
	    gStyle->SetOptFit(1); 
	    
	    third[i] = new TCanvas();
		bothEndCaps[i]->Draw();
		bothEndCaps[i]->Fit("gaus","","",118,132);
	    gStyle->SetOptFit(1); 	    	
	}
}