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


        //TString DataSample = "Signal";
        TString DataSample = "BG";
        // initialize histograms
        int pTbin[]      = {0, 80, 120, 200, 270, 350, 450, 550, 750};// keep only for file definition
        int pTbinCor[]   = {0, 80, 120, 200, 300, 400, 500, 600, 750};//Corrected values due to generation
	int pTbinCorBG[] = {0, 80, 120, 200, 270, 350, 450, 550, 750};//Corrected values due to generation
        int NpTbins = int(sizeof(pTbin)/sizeof(pTbin[0]));
        if(DataSample == "BG"){
	        for (int i = 0; i < NpTbins; i++)
        	{
			pTbinCor[i] = pTbinCorBG[i];
		}
	} 
        cout << "# of pT bins = " << NpTbins << endl;
        std::string title[NpTbins];
        for (int i = 0; i < NpTbins; i++)
        {
           if (i < (NpTbins-1) )title[i] = Form("p_{T}: %d-%d GeV", pTbinCor[i], pTbinCor[i+1]);
           else title[i] = Form("p_{T}: %d-inf GeV", pTbinCor[i]);
           cout << "title " << i << " = " << title[i] << endl;
        }

        TH1D * bothBarrel[NpTbins];
        TH1D * barrelAndEndCaps[NpTbins];
        TH1D * bothEndCaps[NpTbins];
        TH1D * hHiggsPt[NpTbins];
        TH1D * hHiggsPtCut[NpTbins];
        TH1D * hLeadingJet_pT[NpTbins];
        TH1D * hLeadingJet_eta[NpTbins];
        TH1D * hHiggsMass;
        hHiggsMass = new TH1D("hHiggsMass","hHiggsMass", 400, 0., 150.);

        std::string work;
        std::string workName;

        for (int i = 0; i < NpTbins; i++)
        {

                Int_t nbin_mass = 60;
		Double_t xmin_mass = 110.;
		Double_t xmax_mass = 140.;
		if(DataSample == "BG"){
			nbin_mass = 300; xmin_mass = 50.; xmax_mass = 200.;
		}
                work = "M(#gamma#gamma), both barrel (|#eta_{#gamma}| < 1.44)" + title[i];
                workName = "hMgg_bothBarrel " + title[i];
                bothBarrel[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                TString TitleX = "M(#gamma#gamma) (GeV)";
                TString TitleY = Form("Events/(%1.1f GeV)", bothBarrel[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                bothBarrel[i]-> GetXaxis()->SetTitle(TitleX);
                bothBarrel[i]-> GetYaxis()->SetTitle(TitleY);
                bothBarrel[i]-> Sumw2();

                work = "M(#gamma#gamma), barrel (|#eta_{#gamma}| < 1.44) - endcap (1.57 < |#eta_{#gamma}| > 2.5) " + title[i];
                workName = "hMgg_BarrelEndcap " + title[i];
                barrelAndEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                barrelAndEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                barrelAndEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                barrelAndEndCaps[i]-> Sumw2(); 

                work = "M(#gamma#gamma), both endcap (1.57 < |#eta_{#gamma}| > 2.5)" + title[i];
                workName = "hMgg_bothEndcap " + title[i];
                bothEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                bothEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                bothEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                bothEndCaps[i]-> Sumw2();


                work = "p_{T}(#gamma#gamma)" + title[i];
                workName = "hpTgg " + title[i];
                hHiggsPt[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 1000.);
                TitleX = "p_{T}(#gamma#gamma) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hHiggsPt[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hHiggsPt[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsPt[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPt[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma) Cut" + title[i];
                workName = "hpTggCut " + title[i];
                hHiggsPtCut[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 1000.);
                hHiggsPtCut[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsPtCut[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPtCut[i]-> Sumw2();

                work = "p_{T}(leading jet)" + title[i];
                workName = "hLeadingJet_pT " + title[i];
                hLeadingJet_pT[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 1000.);
                TitleX = "p_{T}(leading jet) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hLeadingJet_pT[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hLeadingJet_pT[i]-> GetXaxis()->SetTitle(TitleX);
                hLeadingJet_pT[i]-> GetYaxis()->SetTitle(TitleY);
                hLeadingJet_pT[i]-> Sumw2();

                work = "#eta(leading jet)" + title[i];
                workName = "hLeadingJet_eta " + title[i];
                hLeadingJet_eta[i] = new TH1D(workName.c_str(),work.c_str(), 200, -2.5, 2.5);
                TitleX = "#eta(leading jet) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hLeadingJet_eta[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hLeadingJet_eta[i]-> GetXaxis()->SetTitle(TitleX);
                hLeadingJet_eta[i]-> GetYaxis()->SetTitle(TitleY);
                hLeadingJet_eta[i]-> Sumw2();

        }


        string FileName = "";
        int FileNumber = 1;
        //for reco pT (GeV):  0-120   120-200  200-270  270-350  350-450   450-550   550-750    750-inf 
        double XsecD[] =     {14.307, 3.9088,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 15 
        double Xsec[]  =     {14.557, 3.9086,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 22.5 
        double XsecU[] =     {14.582, 3.9047,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 30
        double dXsec[] =     {0.0068, 0.0012,  0.00022, 7.4e-05, 3.1e-05, 1.1e-05,   5.8e-06,   1.4e-06}; // xsec pb: merge scale 22.5 

        double XsecD_BG[] =  {15.562, 2.8415,  0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
	double Xsec_BG[]  =  {15.715, 2.8415,  0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
	double XsecU_BG[] =  {15.637, 2.841,   0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
        double dXsec_BG[] =  {0.005,  0.0008,  7.1e-05, 1.3e-05,  3.2e-06, 6.5e-07,  2.4e-07,    2.8e-08};
        int NLumi = int(sizeof(Xsec)/sizeof(Xsec[0]));
        if(DataSample == "BG"){
                for (int i = 0; i < NLumi; i++)
                {
                        XsecD[i] = XsecD_BG[i];
                        Xsec[i] = Xsec_BG[i];
                        XsecU[i] = XsecU_BG[i];
                        dXsec[i] = dXsec_BG[i];
                }
        }
        int Ncheck = 0;

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
                 
		if( jentry%10000 == 0 )
			std::cout << "Loop over entry " << jentry << " events: File number = " << FileNumber << std::endl;
                if (FileNumber > NLumi) cout << "Error: FileNumber > NLumi check code: FileNumber = " << FileNumber << " NLumi = " << NLumi << endl;
                if (FileNumber > NLumi) continue;
        	// make weight for 300 fb-1 and Br(H->gg) = 2.27e-03
                //                              pb -> fb   #fb  #events BR(H->gg)                 
        	double weight_F = Xsec[FileNumber-1]*1000.*300./50000.*2.27e-03;

                // find leading jet in |eta| < 2.4:
                double JetLeading_pT = 0.;
                double JetLeading_eta = -5.;
                if (Jet_size > 0)
                {
                for (int i = 0; i < Jet_size; i++)
                {
			if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > JetLeading_pT){JetLeading_pT = Jet_PT[i]; JetLeading_eta = Jet_Eta[i];} 

                }
                }
                if (JetLeading_pT > 0.)
		{
			hLeadingJet_pT[FileNumber] -> Fill(JetLeading_pT, weight_F);
			hLeadingJet_eta[FileNumber] -> Fill(JetLeading_eta, weight_F);
		}
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
                                        if (j < (NpTbins -1)){if (Particle_PT[i] >= pTbinCor[j] && Particle_PT[i] < pTbinCor[j+1]) indicator = j;}
                                        else {if (Particle_PT[i] >= pTbinCor[j]) indicator = j;}

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
                        if (j < (NpTbins -1)){if (s.Pt()>= pTbinCor[j] && s.Pt() < pTbinCor[j+1]) indicatorReco = j;}
                      else {if (s.Pt() >= pTbinCor[j]) indicatorReco = j;}
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

                if (check == 1)         //fill the right histograms if the mass was recorded
                {
                        if (flag == 1)
                        {
                                //bothBarrel[indicator] -> Fill(mass);
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))bothBarrel[indicatorReco] -> Fill(mass,weight_F);
                                if(FileNumber > 1 && FileNumber == indicatorReco)bothBarrel[indicatorReco] -> Fill(mass,weight_F);
                        }
                        else if (flag == 2)
                        {
                                //barrelAndEndCaps[indicator] -> Fill(mass);
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
                                if(FileNumber > 1 && FileNumber == indicatorReco)barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
                        }
                        else if (flag == 3)
                        {
                                //bothEndCaps[indicator] -> Fill(mass);                 
                                if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
                                if(FileNumber > 1 && FileNumber == indicatorReco)bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
                        }
			// fill pT gamma-gamma bins:
                        Ncheck ++;
                        if(FileNumber == 1 && (indicatorReco == 0 || indicatorReco==1))
			{
				if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
				else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
			}
                        if(FileNumber > 1 && FileNumber == indicatorReco)
			{
				if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
                                else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
			}
                        if (FileNumber < (NpTbins-3) && pTHiggs < pTbinCor[FileNumber+3])
			{
                        	if (pTHiggs < 1000.)hHiggsPt[FileNumber] -> Fill (pTHiggs,weight_F);
				else hHiggsPt[FileNumber] -> Fill (1000.,weight_F);
                        }
                        if (FileNumber >= (NpTbins-3))
			{
                        	if (pTHiggs < 1000.)hHiggsPt[FileNumber] -> Fill (pTHiggs,weight_F);
				else hHiggsPt[FileNumber] -> Fill (1000.,weight_F);
                        }
			if( Ncheck%1000 == 0 ){
				std::cout << "Selected events = " << Ncheck << " #File = " << FileNumber << " weight_F = " << weight_F << " indicatorReco = " << indicatorReco << " pT(gg) = " << pTHiggs << std::endl;
			}
                } //end check

        } // end cyclbe by nentries

        TFile *outFile = new TFile ( "SignalSummary.root", "RECREATE");
        if(DataSample == "BG")outFile = new TFile ( "BackgroundSummary.root", "RECREATE");
	gStyle->SetCanvasDefW(2000);
	gStyle->SetCanvasDefH(2000);
        TCanvas *first[NpTbins];

        for (int i = 0; i < NpTbins; i++) // draw and fit histograms
        {
                first[i] = new TCanvas();
                first[i] -> Divide(2,2);
                if(DataSample == "Signal")gStyle->SetOptFit(1);
                //gStyle->SetOptStat(1); 

                first[i] -> cd(1);
                bothBarrel[i]->Draw();
                if(DataSample == "Signal")bothBarrel[i]->Fit("gaus","","",120,130);

                first[i] -> cd(2);
                barrelAndEndCaps[i]->Draw();
                if(DataSample == "Signal")barrelAndEndCaps[i]->Fit("gaus","","",120,130);

                first[i] -> cd(3);
                bothEndCaps[i]->Draw();
                if(DataSample == "Signal")bothEndCaps[i]->Fit("gaus","","",120,130);


                bothBarrel[i]->Write();
                barrelAndEndCaps[i]->Write();
                bothEndCaps[i]->Write();
                hHiggsPt[i]->Write();
                hHiggsPtCut[i]->Write();
                hLeadingJet_pT[i]->Write();
                hLeadingJet_eta[i]->Write();
                first[i] -> Print(Form("Plots/pTbin_%d.png",i));
        }
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0); 
        TCanvas *Dist_pTgg;
        Dist_pTgg = new TCanvas();
        Dist_pTgg -> Divide(1,1);
        Dist_pTgg -> cd(1);
        gPad -> SetLogx(1);
        gPad -> SetLogy(1);
	TH2F *hEmpty = new TH2F("hEmpty","",40,80,1000.,40,0.001,200.);//x vs y
	if(DataSample == "BG")hEmpty = new TH2F("hEmpty","",40,50,1000.,40,0.00001,200.);//x vs y
        hEmpty -> GetXaxis()->SetTitle("p_{T}(#gamma#gamma) (GeV)");
        hEmpty -> GetYaxis()->SetTitle(Form("Events/(%3.0f GeV)", hHiggsPt[0]->GetXaxis()->GetBinWidth(1)));
        hEmpty ->Draw();
        for (int i = 0; i < NLumi; i++) // draw and fit histograms
        {
           hHiggsPt[i+1] -> SetLineColor(i+1);   
           hHiggsPt[i+1] -> SetLineStyle(i+1);   
           hHiggsPt[i+1] -> SetLineWidth(3);   
           hHiggsPt[i+1] -> Draw("same");   
           hHiggsPtCut[i+1] -> SetLineColor(i+1);   
           hHiggsPtCut[i+1] -> SetLineStyle(1);   
           hHiggsPtCut[i+1] -> SetLineWidth(3);   
           hHiggsPtCut[i+1] -> Draw("samehisto");   
	}
        Dist_pTgg -> Print("Plots/Dist_pTbinMerge.png");

        //TCanvas* cQuality;
        //cQuality = new TCanvas("");
        //cQuality->cd();
        //hHiggsPt->Draw("e");
        //cQuality->SaveAs("pic_HiggsPt.png");

        outFile->Close();
}//end Loop()
