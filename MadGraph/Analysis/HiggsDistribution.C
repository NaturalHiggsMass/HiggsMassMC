#define HiggsDistribution_cxx
#include "HiggsDistribution.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void HiggsDistribution::Loop()
{
//   In a ROOT session, you can do:
//      root> .L HiggsDistribution.C
//      root> HiggsDistribution t
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

	TH1D * hHiggsPt;
        hHiggsPt = new TH1D("hHiggsPt","hHiggsPt", 100, 0., 500.);
	TH1D * hHiggsMass;
        hHiggsMass = new TH1D("hHiggsMass","hHiggsMass", 400, 0., 150.);
	

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

                if(Photon_size < 2) continue; // select 2 photons 
                // select 2 photons with max pT
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

		
		if (p2 == test) continue;			// if both photons filled

			s = p1+p2;
			mass = s.M();
                        float pTHiggs = s.Pt();
                        hHiggsPt -> Fill (pTHiggs);
                        hHiggsMass -> Fill (mass);
			

                //TCanvas* cQuality;
                //cQuality = new TCanvas("");
                //cQuality->cd();
                //hHiggsPt->Draw("e");
                //cQuality->SaveAs("pic_HiggsPt.png");

	}

}
