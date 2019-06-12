#define HiggsAnalysis_cxx
#include "HiggsAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void HiggsAnalysis::Loop()
{
//   In a ROOT session, you can do:
//      root> .L HiggsAnalysis.C
//      root> HiggsAnalysis t
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
        TH1D * hHiggsPt[NpTbins];
        TH1D * hHiggsPtCut[NpTbins];
        TH1D * hHiggsMass;
        hHiggsMass = new TH1D("hHiggsMass","hHiggsMass", 400, 0., 150.);

        std::string work;
        std::string workName;

        for (int i = 0; i < NpTbins; i++)
        {
                TString TitleX = "M(#gamma#gamma) (GeV)";
                TString TitleY = "# events";

                work = "M(#gamma#gamma), both barrel (|#eta_{#gamma}| < 1.44)" + title[i];
                workName = "hMgg_bothBarrel " + title[i];
                bothBarrel[i] = new TH1D(workName.c_str(),work.c_str(), 100,110, 140);
                bothBarrel[i]-> GetXaxis()->SetTitle(TitleX);
                bothBarrel[i]-> GetYaxis()->SetTitle(TitleY);
                bothBarrel[i]-> Sumw2();

                work = "M(#gamma#gamma), barrel (|#eta_{#gamma}| < 1.44) - endcap (1.57 < |#eta_{#gamma}| > 2.5) " + title[i];
                workName = "hMgg_BarrelEndcap " + title[i];
                barrelAndEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), 100,110,140);
                barrelAndEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                barrelAndEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                barrelAndEndCaps[i]-> Sumw2(); 

                work = "M(#gamma#gamma), both endcap (1.57 < |#eta_{#gamma}| > 2.5)" + title[i];
                workName = "hMgg_bothEndcap " + title[i];
                bothEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), 100,110,140);
                bothEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                bothEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                bothEndCaps[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma)" + title[i];
                workName = "hpTgg " + title[i];
                hHiggsPt[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 1000.);
                hHiggsPt[i]-> GetXaxis()->SetTitle("p_{T}(#gamma#gamma) (GeV)");
                hHiggsPt[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPt[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma) Cut" + title[i];
                workName = "hpTggCut " + title[i];
                hHiggsPtCut[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 1000.);
                hHiggsPtCut[i]-> GetXaxis()->SetTitle("p_{T}(#gamma#gamma) (GeV)");
                hHiggsPtCut[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPtCut[i]-> Sumw2();
        }


        string FileName = "";
        int FileNumber = 1;
        //for reco pT (GeV):  0-120   120-200  200-270  270-350  350-450   450-550   550-750    750-inf 
        //double Lumi[] =      {1477.,  12760.,  73820.,  222100., 530400.,  1526000., 2882000.,  11703000.}; // Luminosity pb-1 
        double Lumi[] =      {1.477,  12.760,  73.820,  222.100, 530.400,  1526.000, 2882.000,  11703.000}; // Luminosity fb-1 
        double Xsec[] =      {40.63,  4.700,   0.8124,  0.2670,  0.11302,  0.03927,  0.02079,   0.005114}; // xsec pb 
        double dXsec[] =     { 0.03,  0.005,   0.0008,  0.0003,  0.00014,  0.00004,  0.00002,   0.000006}; // xsec pb 
        int NLumi = int(sizeof(Lumi)/sizeof(Lumi[0]));

        Long64_t nbytes = 0, nb = 0;
        for (Long64_t jentry=0; jentry<nentries;jentry++) {
                Long64_t ientry = LoadTree(jentry);
                if (ientry < 0) break;
                nb = fChain->GetEntry(jentry);   nbytes += nb;
                // if (Cut(ientry) < 0) continue;
                string FileNameCheck = fChain -> GetCurrentFile()->GetName();
                size_t posStart = FileNameCheck.find("forPtReco");      // position of "forPtReco" in str
                size_t posEnd = FileNameCheck.find("GeV.root");      // position of "GeV.root" in str
                string FileNameCut = FileNameCheck.substr (posStart+9,posEnd-posStart-9);
                string FileNameCut_pTbin = "";
                if (FileNumber == 1)FileNameCut_pTbin = Form("%d_%d_%d", pTbin[0],pTbin[1],pTbin[2]);
                if (FileNumber > 1 && FileNumber < (NpTbins-1))FileNameCut_pTbin = Form("%d_%d", pTbin[FileNumber],pTbin[FileNumber+1]);
		if (FileNumber == (NpTbins-1))FileNameCut_pTbin = Form("%d_inf", pTbin[FileNumber]);
		if (FileNumber > (NpTbins-1))
		{
			cout <<"Error: more Files then pTbins, please check and correct code. Task is terminated: no more Files connecting." << endl;
			continue;
		}
                if (FileNameCut != FileNameCut_pTbin)
		{
			FileNumber++;
                	if (FileNumber > 1 && FileNumber < (NpTbins-1))FileNameCut_pTbin = Form("%d_%d", pTbin[FileNumber],pTbin[FileNumber+1]);
			if (FileNumber == (NpTbins-1))FileNameCut_pTbin = Form("%d_inf", pTbin[FileNumber]);
                	if (FileNumber > (NpTbins-1))
        	        {
      	                  cout <<"Error: more Files then pTbins, please check and correct code. Task is terminated: no more Files connecting." << endl;
                        	continue;
                	}
                }
                if (FileName != FileNameCheck)
		{
			cout << "File name = " << FileNameCheck << endl;
                	cout << "File name Cut = " << FileNameCut << endl;
                	cout << "File name Cut pTbin = " << FileNameCut_pTbin << endl;
                }
		FileName = FileNameCheck;
                 

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
                                if (p1 == test)         // if this is the first photon in the event
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
                                else if ( (p1 != test) && (p2 != test) )        // maybe another photon, trying to get max pTs
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
                }       // End of looping through photons


                if(Particle_size > 0){
                for ( int i = 0; i < Particle_size; i++) // now loop through all particles to find Higgs
                {
                        if ( Particle_PID[i] == 25)     //set indicator depending on Pt
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
                //if (indicator < 0 )std::cout <<"ERROR: no gen Higgs found" << std::endl;
                //if (indicator < 0)continue; // reject events without good generated Higgs

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

                if (FileNumber > NLumi) cout << "Error: FileNumber > NLumi check code: FileNumber = " << FileNumber << " NLumi = " << NLumi << endl;
                if (FileNumber > NLumi) continue;
                if (check == 1)         //fill the right histograms if the mass was recorded
                {
                        double weight_In = 1./Lumi[0];
                        if (indicatorReco > 1) weight_In = 1./Lumi[indicatorReco-1];
                        double weight_F = 1./Lumi[FileNumber-1];
                        //cout << " indicatorReco = " << indicatorReco << " FileNumber = " << FileNumber << "weight_In = " << weight_In << " weight_F = " << weight_F << endl;
                        if (flag == 1)
                        {
                                //bothBarrel[indicator] -> Fill(mass);
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))bothBarrel[indicatorReco] -> Fill(mass,weight_In);
                                if(FileNumber > 1 && FileNumber == indicatorReco)bothBarrel[indicatorReco] -> Fill(mass,weight_In);
                        }
                        else if (flag == 2)
                        {
                                //barrelAndEndCaps[indicator] -> Fill(mass);
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_In);
                                if(FileNumber > 1 && FileNumber == indicatorReco)barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_In);
                        }
                        else if (flag == 3)
                        {
                                //bothEndCaps[indicator] -> Fill(mass);                 
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))bothEndCaps[indicatorReco] -> Fill(mass,weight_In);
                                if(FileNumber > 1 && FileNumber == indicatorReco)bothEndCaps[indicatorReco] -> Fill(mass,weight_In);
                        }
                        if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))
			{
				if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_In);
				else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_In);
			}
                        if(FileNumber > 1 && FileNumber == indicatorReco)
			{
				if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_In);
                                else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_In);
			}
                        if (pTHiggs < 1000.)hHiggsPt[FileNumber] -> Fill (pTHiggs,weight_F);
			else hHiggsPt[FileNumber] -> Fill (1000.,weight_F);
                }

        } // end cyclbe by nentries

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
                hHiggsPt[i]->Write();
                hHiggsPtCut[i]->Write();
        }
        outFile->Close();
}//end Loop()
