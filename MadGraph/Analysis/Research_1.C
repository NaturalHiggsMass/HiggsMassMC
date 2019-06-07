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
        int pTbin[] = {0, 80, 120, 200, 270, 350, 450, 550, 750};	
        int NpTbins = int(sizeof(pTbin)/sizeof(pTbin[0]));
        cout << "# of pT bins = " << NpTbins << endl; 
	//std::string title[5] = {"pT(0,50 GeV)", "pT(50,100 GeV)", "pT(100,150 GeV)", "pT(150,200 GeV)", "pT(200+ GeV)"};
	std::string title[NpTbins];
        for (int i = 0; i < NpTbins; i++)
        {
           if (i < (NpTbins-1) )title[i] = Form("p_{T}: %d-%d GeV", pTbin[i], pTbin[i+1]);
           else title[i] = Form("p_{T}: %d-inf GeV", pTbin[i]);
           cout << "title " << i << " = " << title[i] << endl;   
        }

	TH1D * bothBarrel[NpTbins];
	TH1D * barrelAndEndCaps[NpTbins];
	TH1D * bothEndCaps[NpTbins];
	TH1D * hHiggsPt;
        hHiggsPt = new TH1D("hHiggsPt","hHiggsPt", 100, 0., 500.);
	TH1D * hHiggsMass;
        hHiggsMass = new TH1D("hHiggsMass","hHiggsMass", 400, 0., 150.);

	std::string work;	
		
	for (int i = 0; i < NpTbins; i++)
	{	
                TString TitleX = "M(#gamma#gamma) (GeV)";
                TString TitleY = "# events";
                	
		work = "M(#gamma#gamma), both barrel (|#eta_{#gamma}| < 1.44)" + title[i];     
	 	bothBarrel[i] = new TH1D(work.c_str(),work.c_str(), 100,110, 140);
		bothBarrel[i]-> GetXaxis()->SetTitle(TitleX);
		bothBarrel[i]-> GetYaxis()->SetTitle(TitleY);
		
		work = "M(#gamma#gamma), barrel (|#eta_{#gamma}| < 1.44) - endcap (1.57 < |#eta_{#gamma}| > 2.5) " + title[i];
		barrelAndEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		barrelAndEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
		barrelAndEndCaps[i]-> GetYaxis()->SetTitle(TitleY);		
		
		work = "M(#gamma#gamma), both endcap (1.57 < |#eta_{#gamma}| > 2.5)" + title[i];
		bothEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		bothEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
		bothEndCaps[i]-> GetYaxis()->SetTitle(TitleY);		
	}


   	Long64_t nbytes = 0, nb = 0;
   	for (Long64_t jentry=0; jentry<nentries;jentry++) 
  	{
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
                cout << "File name = " << fChain -> GetCurrentFile()->GetName() << endl;

// if (Cut(ientry) < 0) continue;
	   	int indicator = -1;
	   	int indicatorReco = -1;
		int flag = 0;
		bool check = 0;
		double mass = 0;
		
		p1 = test;
		p2 = test;

                if (Photon_size < 2) continue; //select at least 2 photons:
                // p1 - leading (max pT) photon, p2 - subleading
		for (int i = 0; i < Photon_size; i++)
		{
                        if (Photon_PT[i] < 0) cout << "Error: Photon_PT[i] = " << Photon_PT[i] << endl;
			if ( abs(Photon_Eta[i]) <= 2.5 && (abs(Photon_Eta[i]) > 1.57 || abs(Photon_Eta[i]) < 1.44))
			{
				if (p1 == test) 	// if this is the first photon in the event
				{
					p1.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
				}
				else if ( (p1 != test) && (p2 == test) ) // if this is the second photon in the event p1 is with max pT
				{
                                        if(p1.Pt() >= Photon_PT[i])p2.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
                                        else
                                        { 
                                             p2 = p1;
                                             p1.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
					}
				}
				else if ( (p1 != test) && (p2 != test) )	// maybe another photon, trying to get max pTs
				{
					if ( Photon_PT[i] > p1.Pt() )
					{
							p2 = p1;
							p1.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
					}
					else if ( Photon_PT[i] > p2.Pt() )
					{
						p2.SetPtEtaPhiE(Photon_PT[i], Photon_Eta[i], Photon_Phi[i], Photon_E[i]);
					}
				}
			}
		}	// End of looping through photons


                if(Particle_size > 0){		
		for ( int i = 0; i < Particle_size; i++) // now loop through all particles to find Higgs
		{
			if ( Particle_PID[i] == 25)	//set indicator depending on Pt
			{
                                if (Particle_PT[i] < 0) cout << "Error: Particle_PT[" << i << "] = " << Particle_PT[i] << " Particle_Status = " << Particle_Status[i] << endl;
                                for ( int j = 0; j < NpTbins; j++) 
			        {
              				if (j < (NpTbins -1)){if (Particle_PT[i] >= pTbin[j] && Particle_PT[i] < pTbin[j+1]) indicator = j;}
              				else {if (Particle_PT[i] >= pTbin[j]) indicator = j;}
               
				}
			}
		}
                }
                //cout << "indicator = " << indicator << endl;
	        if (indicator < 0 )std::cout <<"ERROR: no gen Higgs found" << std::endl;	
	        if (indicator < 0)continue; // reject events without good generated Higgs

			// set indicatorReco:
        	        //if (p1 == test || p2 == test) cout << "Error: one both photons are not exist -> check the code: pT1 = " << p1.Pt() << " pT2 = " << p2.Pt() << endl;   // if both photons filled
                	if (p1 == test || p2 == test) continue;
			s = p1+p2;
        	        for ( int j = 0; j < NpTbins; j++)
			{
				if (j < (NpTbins -1)){if (s.Pt()>= pTbin[j] && s.Pt() < pTbin[j+1]) indicatorReco = j;}
          	              else {if (s.Pt() >= pTbin[j]) indicatorReco = j;}
                	}	
			mass = s.M();
                        float pTHiggs = s.Pt();
                        hHiggsPt -> Fill (pTHiggs);
                        hHiggsMass -> Fill (mass);
			
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

                //TCanvas* cQuality;
                //cQuality = new TCanvas("");
                //cQuality->cd();
                //hHiggsPt->Draw("e");
                //cQuality->SaveAs("pic_HiggsPt.png");

		if (check == 1)		//fill the right histograms if the mass was recorded
		{
			if (flag == 1)
			{
				//bothBarrel[indicator] -> Fill(mass);
				bothBarrel[indicatorReco] -> Fill(mass);
			}
			else if (flag == 2)
			{
				//barrelAndEndCaps[indicator] -> Fill(mass);
				barrelAndEndCaps[indicatorReco] -> Fill(mass);
			}
			else if (flag == 3)
			{
				//bothEndCaps[indicator] -> Fill(mass);			
				bothEndCaps[indicatorReco] -> Fill(mass);			
			}	
		}
	} // end cycle by root-file

        TFile *outFile = new TFile ( "SignalSummary.root", "RECREATE");
	TCanvas *first[NpTbins];

	for (int i = 0; i < NpTbins; i++) // draw and fit histograms
	{
		first[i] = new TCanvas();
                first[i] -> Divide(2,2);
	        gStyle->SetOptFit(1); 
	        //gStyle->SetOptStat(1); 

                first[i] -> cd(1);
		bothBarrel[i]->Draw();
		bothBarrel[i]->Fit("gaus","","",118,132);
	
                first[i] -> cd(2);
		barrelAndEndCaps[i]->Draw();
		barrelAndEndCaps[i]->Fit("gaus","","",118,132);	
	    
                first[i] -> cd(3);
		bothEndCaps[i]->Draw();
		bothEndCaps[i]->Fit("gaus","","",118,132);

   
		bothBarrel[i]->Write();
		barrelAndEndCaps[i]->Write();
		bothEndCaps[i]->Write();
	}
        outFile->Close();
}
