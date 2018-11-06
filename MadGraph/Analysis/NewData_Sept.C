#define NewData_Sept_cxx
#include "NewData_Sept.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void NewData_Sept::Loop()
{
//   In a ROOT session, you can do:
//      root> .L NewData_Sept.C
//      root> NewData_Sept t
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

/*   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
*/   

	TLorentzVector test(0.,0.,0.,0.);
	TLorentzVector p1(0.,0.,0.,0.);
	TLorentzVector p2(0.,0.,0.,0.);
	TLorentzVector s(0.,0.,0.,0.);	
	
// initialize histograms

	TH1D * bothBarrel[5];
	TH1D * barrelAndEndCaps[5];
//	TH1D * bothEndCaps[5];
	
	std::string title[5] = {"pT(0,50 GeV)", "pT(50,100 GeV)", "pT(100,150 GeV)", "pT(150,200 GeV)", "pT(200+ GeV)"};
	std::string work;	
		
	for (int i = 0; i < 5; i++)
	{		
		work = "Histogram of Invariant Mass, both Photons Eta < 1.44, Higgs " + title[i];
	 	bothBarrel[i] = new TH1D(work.c_str(),work.c_str(), 100,110, 140);
		bothBarrel[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		bothBarrel[i]-> GetYaxis()->SetTitle("Number of Events");
		
//		work = "Histogram of Invariant Mass, one Photon Eta < 1.44, other Photon > 1.57, Higgs " + title[i];
		work = "Histogram of Invariant Mass, at least one Photon Eta > 1.57, Higgs " + title[i];
		barrelAndEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		barrelAndEndCaps[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		barrelAndEndCaps[i]-> GetYaxis()->SetTitle("Number of Events");			
	}
	
	TH1D * HiggsPT;
	TH1D * HiggsEta;
	TH1D * HiggsGenEta;
	TH1D * GammaEta;
	
	HiggsPT = new TH1D("HiggsPT","Higgs pT", 100,0,500);
	HiggsPT-> GetXaxis()->SetTitle("Higgs pT (GeV)");
	HiggsPT-> GetYaxis()->SetTitle("Number of Events");	
		
	HiggsEta = new TH1D("HiggsEta","Higgs eta", 100,-8,8);
	HiggsEta-> GetXaxis()->SetTitle("Higgs eta");
	HiggsEta-> GetYaxis()->SetTitle("Number of Events");	
	
	HiggsGenEta = new TH1D("HiggsGenEta","Higgs reconstructed Eta", 100,-8,8);
	HiggsGenEta-> GetXaxis()->SetTitle("Higgs reconstructed Eta");
	HiggsGenEta-> GetYaxis()->SetTitle("Number of Events");	
		
	GammaEta = new TH1D("GammaEta","Gamma eta", 100,-8,8);
	GammaEta-> GetXaxis()->SetTitle("Gamma eta");
	GammaEta-> GetYaxis()->SetTitle("Number of Events");
	
	TH2D *h2d = new TH2D("h2d","Jet vs. Higgs pT",400,0, 400,400,0,400);
	h2d-> GetXaxis()->SetTitle("Higgs pT (GeV)");
	h2d-> GetYaxis()->SetTitle("Jet pT (GeV)");	

	int indicator = 0;
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int count4 = 0;
	int count5 = 0;
	int count6 = 0;
	int y =0;
	int flag = 0;
	bool check = 0;
	double mass = 0;
	double reconstructedEta = 0;
	double gamma1_eta = 0;
	double gamma2_eta = 0;
	double dEta = 0;
	double jEta = 0;
	double hEta = 0;

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
		
		std::vector<double> v_hpt;
		std::vector<double> v_jpt;
// checking higgs situation
		if (Particle_size > 0)
		{
		for ( int i = 0; i < Particle_size; i++) // loop through all particles to find Higgs
		{
			if ( Particle_PID[i] == 25 )	//set indicator depending on Pt
			{
				count++;
				if (y==0)
				{
					count1++;
					y=1;
				}
				else if (y == 1) 
				{
					count2++;
					y=2;
				}
				else if (y==2)
				{
					count3++;
					y=3;
				}
				else if (y == 3) 
				{
					count4++;
					y=4;
				}
				else if (y==4)
				{
					count5++;
					y=5;
				}
				else if (y==5)
				{
					count6++;
				}
		
				HiggsPT-> Fill(abs(Particle_PT[i]));
				HiggsEta-> Fill(Particle_Eta[i]);
				hEta = Particle_Eta[i];
//				v_hpt.push_back(abs(Particle_PT[i]));
			}
		}
		}
		y = 0;
		
		if (Jet_size > 0)
		{
		for (int i = 0; i < Jet_size; i++)
		{
//			v_jpt.push_back(abs(Jet_PT[i]));
			
		}
		}
/*		
		
		for (int i = 0; i < v_hpt.size(); i++)
		{
			h2d->Fill(v_hpt[i],v_jpt[i]);
		}
*/		
		if (Photon_size > 0)
		{
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
		}
		
		if (Particle_size >0)
		{
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
		}
		
		if (p2 != test)			// if both photons filled
		{
			s = p1+p2;
			mass = s.M();
			reconstructedEta = s.Eta();
			gamma1_eta = p1.Eta();
			gamma2_eta = p2.Eta();
			
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
			else if (flag == 2 || flag == 3)
			{
				barrelAndEndCaps[indicator - 1] -> Fill(mass);
			}

			HiggsGenEta -> Fill(reconstructedEta);
			GammaEta -> Fill(gamma1_eta);
			GammaEta -> Fill(gamma2_eta);
		}
	}
	
	std:: cout << "total number of higgs " << count << std::endl;
	std:: cout << "Events with one higgs: " << count1 << std::endl;
	std:: cout << "Events with two higgs: " << count2 << std::endl;
	std:: cout << "Events with three higgs: " << count3 << std::endl;
	std:: cout << "Events with four higgs: " << count4 << std::endl;
	std:: cout << "Events with five higgs: " << count5 << std::endl;
	std:: cout << "Events with six higgs: " << count6 << std::endl;
	std:: cout << "Events: " << nentries << std::endl;

	TCanvas *first[5];
	TCanvas *second[5];		

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
	}
		TCanvas *pT;
		TCanvas *eta;
		TCanvas *d;
		TCanvas *ReEta;
		TCanvas *gammaEta;
		
		pT = new TCanvas();
		HiggsPT->Draw();

		eta = new TCanvas();
		HiggsEta->Draw();
		
		ReEta = new TCanvas();
		HiggsGenEta->Draw();

		gammaEta = new TCanvas();
		GammaEta->Draw();
		
/*		d = new TCanvas();
		h2d->Draw();*/
   
   
   
}
