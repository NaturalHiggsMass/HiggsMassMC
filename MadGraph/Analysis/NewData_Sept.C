#define NewData_Sept_cxx
#include "NewData_Sept.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <stdbool.h>
#include <fstream>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"





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


	TLorentzVector test(0.,0.,0.,0.);
	TLorentzVector p1(0.,0.,0.,0.);
	TLorentzVector p2(0.,0.,0.,0.);
	TLorentzVector sp1(0.,0.,0.,0.);
	TLorentzVector sp2(0.,0.,0.,0.);
	TLorentzVector s(0.,0.,0.,0.);	
	
// initialize histograms

	TH1D * bothBarrel[5];
	TH1D * barrelAndEndCaps[5];
//	TH1D * bothEndCaps[5];
	
	std::string title[5] = {"pT(0,50 GeV)", "pT(50,100 GeV)", "pT(100,150 GeV)", "pT(150,200 GeV)", "pT(200+ GeV)"};
	std::string work;	
		
	for (int i = 0; i < 5; i++)
	{		
//		work = "Histogram of Invariant Mass, both Photons Eta < 1.44, Higgs " + title[i];
	 	bothBarrel[i] = new TH1D(work.c_str(),work.c_str(), 100,110, 140);
		bothBarrel[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		bothBarrel[i]-> GetYaxis()->SetTitle("Number of Events");
		
//		work = "Histogram of Invariant Mass, one Photon Eta < 1.44, other Photon > 1.57, Higgs " + title[i];
//		work = "Histogram of Invariant Mass, at least one Photon Eta > 1.57, Higgs " + title[i];
		work = "Historgam of Photon" + title[i];
		barrelAndEndCaps[i] = new TH1D(work.c_str(),work.c_str(), 100,110,140);
		barrelAndEndCaps[i]-> GetXaxis()->SetTitle("Invariant Mass (GeV)");
		barrelAndEndCaps[i]-> GetYaxis()->SetTitle("Number of Events");			
	}
	
//	TH1D * HiggsPT;
//	TH1D * HiggsEta;
//	TH1D * HiggsGenEta;
	TH1D * GammaEta;
	TH1D * DoubleGamma1;
	TH1D * DoubleGamma2;
	TH1D * DoubleGamma3;
	TH1D * DoubleGamma4;

	TH1D * DoubleGamma_f2;
	TH1D * DoubleGamma_f3;
	TH1D * DoubleGamma_f4;
	TH1D * DoubleGamma_f5;
	TH1D * DoubleGamma_f6;
	TH1D * DoubleGamma_f7;
	TH1D * DoubleGamma_f8;
	TH1D * DoubleGamma_f9;

	TH1D * DoubleGamma_r2_pt1;
	TH1D * DoubleGamma_r2_pt3;
	TH1D * DoubleGamma_r2_eta1;
	

	TH1D * Background_pt;

	TH1D * Background_eta;


	DoubleGamma1 = new TH1D("DoubleGamma_BG","DiGammas pT", 550,0,1100);
	DoubleGamma1-> GetXaxis()->SetTitle("DiGammas pT (GeV)");
	DoubleGamma1-> GetYaxis()->SetTitle("Number of Events");

	
	DoubleGamma2 = new TH1D("SingleGamma_BG","Gamma pT", 150,0,150);
	DoubleGamma2-> GetXaxis()->SetTitle("Gammas pT (GeV)");
	DoubleGamma2-> GetYaxis()->SetTitle("Number of Events");

	DoubleGamma3 = new TH1D("DoubleGamma_mass_BG","DiGammas mass", 100,100,150);
	DoubleGamma3-> GetXaxis()->SetTitle("DiGammas mass (GeV)");
	DoubleGamma3-> GetYaxis()->SetTitle("Number of Events");

	
	GammaEta = new TH1D("Background_eta","Background Gamma eta", 100,-5,5);
	GammaEta-> GetXaxis()->SetTitle("Gamma eta  (GeV)");
	GammaEta-> GetYaxis()->SetTitle("Number of Events");



//	DoubleGamma_for_eta2 = new TH1D("DoubleGamma_s6","DiGammas pT", 225,200,650);
//	DoubleGamma_for_eta2-> GetXaxis()->SetTitle("DiGammas pT (GeV)");
//	DoubleGamma_for_eta2-> GetYaxis()->SetTitle("Number of Events");


//	HiggsPT = new TH1D("HiggsPT","Higgs pT", 100,0,500);
//	HiggsPT-> GetXaxis()->SetTitle("Higgs pT (GeV)");
//	HiggsPT-> GetYaxis()->SetTitle("Number of Events");	
		
//	HiggsEta = new TH1D("HiggsEta","Higgs eta", 100,-8,8);
//	HiggsEta-> GetXaxis()->SetTitle("Higgs eta");
//	HiggsEta-> GetYaxis()->SetTitle("Number of Events");	
	
//	HiggsGenEta = new TH1D("HiggsGenEta","Higgs reconstructed Eta", 100,-8,8);
//	HiggsGenEta-> GetXaxis()->SetTitle("Higgs reconstructed Eta");
//	HiggsGenEta-> GetYaxis()->SetTitle("Number of Events");	
		

//	TH2D *h2d = new TH2D("h2d","Jet vs. Higgs pT",400,0, 400,400,0,400);
//	h2d-> GetXaxis()->SetTitle("Higgs pT (GeV)");
//	h2d-> GetYaxis()->SetTitle("Jet pT (GeV)");	

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
	double gamma = 0;
	double Digamma = 0;
	double gammas_Pt = 0;
	double gamma1_Pt = 0;
	double gamma2_Pt = 0;
	double gammas_mass = 0;

//____________________
	double BG_gamma1_Pt = 0;
	double BG_gamma2_Pt = 0;
	double BG_gamma1_eta = 0;
	double BG_gamma2_eta = 0;


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

//____________fill_background_________________			
		



// checking higgs situation
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
		
	//	BG_gamma1_eta = p1.Eta();
	//	BG_gamma2_eta = p2.Eta();
		
	//	BG_gamma1_Pt = p1.Pt();
	//	BG_gamma2_Pt = p2.Pt();

	//	Background_pt-> Fill (BG_gamma1_Pt);
	//	Background_pt-> Fill (BG_gamma2_Pt);

	//	Background_eta ->Fill(BG_gamma1_eta);
	//	Background_eta ->Fill(BG_gamma2_eta);


		if (p2 != test)			// if both photons filled 
		{
			s = p1+p2;
			mass = s.M();
			if (mass > 100)
			reconstructedEta = s.Eta();
			gamma1_eta = p1.Eta();
			gamma2_eta = p2.Eta();
			gammas_Pt = s.Pt();
			gamma1_Pt = p1.Pt();
			gamma2_Pt = p2.Pt();
			gammas_mass = s.M();

			//Fill_histo
			//_______________________________
			DoubleGamma1 -> Fill(gammas_Pt);
			DoubleGamma2 -> Fill(gamma1_Pt);
			DoubleGamma2 -> Fill(gamma2_Pt);
			DoubleGamma3 -> Fill(gammas_mass);
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
		TCanvas *DoubleGamma_pt;
		TCanvas *Gammas_pt;
		TCanvas *Digamma_mass;
//		TCanvas *Diga;

	//	TCanvas *DG2;
	//	TCanvas *DG3;
	//	TCanvas *DG4;
	//	TCanvas *DG5;
	//	TCanvas *DG6;
	//	TCanvas *DG7;
	//	TCanvas *DG8;
	//	TCanvas *DG9;
		TCanvas *DG_for_pt1; //for preselection
		TCanvas *DG_for_pt3;
		TCanvas *DG_for_eta1;
		//__________________
		TCanvas *BG_for_pt;    //for background
		TCanvas *BG_for_eta;







//		TString outfileName = "NewData300_500.root";
//		TFile *outFile = TFile::Open( "NewData300_500.root", "RECREATE" );
		TFile *outFile = new TFile ( "BackGround_run09_test.root", "RECREATE");
		


//___________________________________________________________________________________		
		DoubleGamma_pt = new TCanvas();
		DoubleGamma1->Draw();
		DoubleGamma1->Write();

		Gammas_pt = new TCanvas();
		DoubleGamma2->Draw();
		DoubleGamma2->Write();

		Digamma_mass = new TCanvas();
		DoubleGamma3->Draw();
		DoubleGamma3->Write();

		gammaEta = new TCanvas();
		GammaEta->Draw();
		GammaEta->Write();

	//	DoubleGamma = new TCanvas();
	//	DoubleGamma1->SetLineColor(2);
	//	DoubleGamma1->Draw("same");
	//	DoubleGamma1->Write();
	//	DG2 = new TCanvas();
	//	DoubleGamma_f2 -> Draw();
	//	DoubleGamma_f2 -> Write ();

	//	DG3 = new TCanvas();
	//	DoubleGamma_f3 -> Draw();
	//	DoubleGamma_f3 -> Write ();
	//	DG4 = new TCanvas();
	//	DoubleGamma_f4 -> Draw();
	//	DoubleGamma_f4 -> Write ();
	//	DG5 = new TCanvas();
	//	DoubleGamma_f5 -> Draw();
	//	DoubleGamma_f5 -> Write ();
	//	DG6 = new TCanvas();
	//	DoubleGamma_f6 -> Draw();
	//	DoubleGamma_f6 -> Write ();
	//	DG7 = new TCanvas();
	//	DoubleGamma_f7 -> Draw();
	//	DoubleGamma_f7 -> Write ();
	//	DG8 = new TCanvas();
	//	DoubleGamma_f8 -> Draw();
	//	DoubleGamma_f8 -> Write ();
	//	DG9 = new TCanvas();
	//	DoubleGamma_f9 -> Draw();
	//	DoubleGamma_f9 -> Write ();



//		outputFile <<DoubleGamma1->Draw()<< std::end1;
		outFile->Close();
//___________________________________________________________________________________


//		pT = new TCanvas();
//		HiggsPT->Draw();

//		eta = new TCanvas();
//		HiggsEta->Draw();
		
//		ReEta = new TCanvas();
//		HiggsGenEta->Draw();

//		gammaEta = new TCanvas();
//		GammaEta->Draw();
		
/*		d = new TCanvas();
		h2d->Draw();*/
   
   
   
}
