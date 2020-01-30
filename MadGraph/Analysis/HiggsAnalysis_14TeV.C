#define HiggsAnalysis_14TeV_cxx
#include "HiggsAnalysis_14TeV.h"

void HiggsAnalysis_14TeV::Loop()
{
//   In a ROOT session, you can do:
//      root> .L HiggsAnalysis_14TeV.C
//      root> HiggsAnalysis_14TeV t
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
        TLorentzVector p_temp1(0.,0.,0.,0.);
        TLorentzVector p_temp2(0.,0.,0.,0.);


        TString DataSample = "Signal";
        //TString DataSample = "BG";

        Double_t massHiggs = 125.;
        //Double_t massHiggs = 123.;
        //Double_t massHiggs = 127.;

        // initialize histograms

        // for Signal
        int pTbin[]      = {0, 80, 120, 200, 270, 350, 450, 550, 750};// keep only for file definition
        int pTbinCor[]   = {0, 80, 120, 200, 270, 350, 450, 550, 750};// keep the same like definition
        // for Background 
	int pTbinCorBG[] = {0, 80, 120, 200, 270, 350, 450, 550, 750};//Corrected values due to generation
        Int_t NpTbins = int(sizeof(pTbin)/sizeof(pTbin[0]));
        if(DataSample == "BG"){
	        for (int i = 0; i < NpTbins; i++)
        	{
			pTbinCor[i] = pTbinCorBG[i];
		}
	}
        Double_t pTbinCorCenter[NpTbins];
	for (int i = 0; i < (NpTbins-1); i++){
		pTbinCorCenter[i] = Double_t(pTbinCor[i] + pTbinCor[i+1])/2.; 
	}
	pTbinCorCenter[NpTbins-1] = Double_t(pTbinCor[NpTbins-1])+50.;
        cout << "# of pT bins = " << NpTbins << endl;
        Double_t NHiggsGen[NpTbins];
        Double_t NHiggsGen_2g[NpTbins];
        Double_t NHiggsGen_2gCut[NpTbins];
        Double_t NHiggsReco[NpTbins];
        Double_t NHiggsReco_Jet[NpTbins];
        Double_t NHiggsReco_FatJet[NpTbins];
        Double_t Eff_HiggsGen_2gCut[NpTbins];
        Double_t Eff_HiggsReco[NpTbins];
        Double_t Eff_HiggsReco_Jet[NpTbins];
        Double_t Eff_HiggsReco_FatJet[NpTbins];
        Double_t Eff_HiggsReco_Total[NpTbins];
        for (int i = 0; i < NpTbins; i++)
       	{
		NHiggsGen[i] = 0;
		NHiggsGen_2g[i] = 0;
		NHiggsGen_2gCut[i] = 0;
		NHiggsReco[i] = 0;
		NHiggsReco_Jet[i] = 0;
		NHiggsReco_FatJet[i] = 0;
		Eff_HiggsGen_2gCut[i] = 0;
		Eff_HiggsReco[i] = 0;
		Eff_HiggsReco_Jet[i] = 0;
		Eff_HiggsReco_FatJet[i] = 0;
		Eff_HiggsReco_Total[i] = 0;
	}
        std::string title[NpTbins];
        std::string titleDir[NpTbins];
        std::string titleCard[NpTbins];
        std::string titleCard_SigmaUp[NpTbins];
        std::string titleCard_SigmaDown[NpTbins];
        for (int i = 0; i < NpTbins; i++)
        {
           if (i < (NpTbins-1) ){
		title[i] = Form("p_{T}: %d-%d GeV", pTbinCor[i], pTbinCor[i+1]);
		titleDir[i] = Form("PT_%d_%d",pTbinCor[i], pTbinCor[i+1]);
		titleCard[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p2_Mass",pTbinCor[i], pTbinCor[i+1]);
		titleCard_SigmaUp[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p4_Mass",pTbinCor[i], pTbinCor[i+1]);
		titleCard_SigmaDown[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p0_Mass",pTbinCor[i], pTbinCor[i+1]);
		if(DataSample == "BG")titleCard_SigmaDown[i] = Form("BACKGROUND_PT_%d_%d",pTbinCor[i], pTbinCor[i+1]);
	   }	
           else {
		title[i] = Form("p_{T}: %d-inf GeV", pTbinCor[i]);
		titleDir[i] = Form("PT_%d_INF",pTbinCor[i]);
		titleCard[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p2_Mass",pTbinCor[i]);
		titleCard_SigmaUp[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p4_Mass",pTbinCor[i]);
		titleCard_SigmaDown[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p0_Mass",pTbinCor[i]);
		if(DataSample == "BG")titleCard_SigmaDown[i] = Form("BACKGROUND_PT_%d_INF",pTbinCor[i]);
	   }
	   if(DataSample == "Signal" && (massHiggs >122.9 && massHiggs < 123.1)){
		titleCard[i] = titleCard[i]+"Down";
		titleCard_SigmaUp[i] = titleCard_SigmaUp[i]+"Down";
		titleCard_SigmaDown[i] = titleCard_SigmaDown[i]+"Down";
	   }
	   if(DataSample == "Signal" && (massHiggs >126.9 && massHiggs < 127.1)){
		titleCard[i] = titleCard[i]+"Up";
		titleCard_SigmaUp[i] = titleCard_SigmaUp[i]+"Up";
		titleCard_SigmaDown[i] = titleCard_SigmaDown[i]+"Up";
	   }
           cout << "title " << i << " = " << title[i] << endl;
        }

        TH1D * bothBarrel[NpTbins];
        TH1D * barrelAndEndCaps[NpTbins];
        TH1D * bothEndCaps[NpTbins];
        TH1D * massBBandBE[NpTbins];
        TH1D * massSigma1p2[NpTbins];
        TH1D * massSigma1p2_MggScaleUp[NpTbins];
        TH1D * massSigma1p2_MggScaleDown[NpTbins];
        TH1D * massSigma1p0[NpTbins];
        TH1D * massSigma1p4[NpTbins];
        TH1D * hHiggsMass[NpTbins];
        TH1D * hHiggsMass_Cut[NpTbins];
        TH1D * hHiggsPt[NpTbins];
        TH1D * hHiggsPtCut[NpTbins];
        TH1D * hLeadingJet_pT[NpTbins];
        TH1D * hLeadingJet_eta[NpTbins];
        TH1D * hNFatJet_cut[NpTbins];
        TH1D * hFatJet_Mass2Jet[NpTbins];
        TH1D * hFatJet_MassLeading[NpTbins];
        TH1D * hFatJet_MassSubLeading[NpTbins];
        TH1D * hFatJet_MassRest[NpTbins];
        TH1D * hFatJet_Higgs_dR_S[NpTbins];
        TH1D * hFatJet_Higgs_dR_BS[NpTbins];
        TH1D * hFatJet_Higgs_dR_ggGen[NpTbins];
        TH1D * hHiggs_dR_ggReco[NpTbins];
        TH1D * hNHiggsReco[NpTbins];

        TH1D * hNJet_cut[NpTbins];
        TH1D * hJet_Mass2Jet[NpTbins];
        TH1D * hJet_MassLeading[NpTbins];
        TH1D * hJet_MassSubLeading[NpTbins];
        TH1D * hJet_MassRest[NpTbins];
        TH1D * hJet_Higgs_dR_S[NpTbins];
        TH1D * hJet_Higgs_dR_BS[NpTbins];
        TH1D * hJet_Higgs_dR_ggGen[NpTbins];

        std::string work;
        std::string workName;

        Int_t nbin_mass = 60; // Madgraph sample
        Int_t nbin_massGen = 300;// gen Random sample in 100 MeV
	Double_t xmin_mass = 110.;
	Double_t xmax_mass = 140.;
	//if(DataSample == "BG"){
	//	nbin_mass = 300; xmin_mass = 50.; xmax_mass = 200.;
	//}

        for (int i = 0; i < NpTbins; i++)
        {


                work = "BB (|#eta_{#gamma}| < 1.44), " + title[i];
                workName = "hMgg_bothBarrel " + title[i];
                bothBarrel[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                TString TitleX = "M(#gamma#gamma) (GeV)";
                TString TitleY = Form("Events/(%1.1f GeV)", bothBarrel[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                bothBarrel[i]-> GetXaxis()->SetTitle(TitleX);
                bothBarrel[i]-> GetYaxis()->SetTitle(TitleY);
                bothBarrel[i]-> Sumw2();

                work = "B (|#eta_{#gamma}| < 1.44) - E (1.57 < |#eta_{#gamma}| < 2.5), " + title[i];
                workName = "hMgg_BarrelEndcap " + title[i];
                barrelAndEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                barrelAndEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                barrelAndEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                barrelAndEndCaps[i]-> Sumw2(); 

                work = "EE (1.57 < |#eta_{#gamma}| < 2.5), " + title[i];
                workName = "hMgg_bothEndcap " + title[i];
                bothEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                bothEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                bothEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                bothEndCaps[i]-> Sumw2();

                work = "BB & BE, " + title[i];
		//workName = "hMgg_BBandBE " + title[i];
                workName = "ORIGINAL_"+titleCard[i];
		if(DataSample == "BG")workName = "ORIGINAL_"+titleCard_SigmaDown[i];
                massBBandBE[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                massBBandBE[i]-> GetXaxis()->SetTitle(TitleX);
                massBBandBE[i]-> GetYaxis()->SetTitle(TitleY);
                massBBandBE[i]-> Sumw2();

                work = "#sigma 1p2 for 300 fb^{-1}, " + title[i];
                workName = titleCard[i];
                massSigma1p2[i] = new TH1D(workName.c_str(),work.c_str(), nbin_massGen, xmin_mass, xmax_mass);
                massSigma1p2[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p2[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p2[i]-> Sumw2();

                work = "#sigma 1p2 MggScaleUp for 300 fb^{-1}, " + title[i];
                workName = titleCard[i]+"_MggScaleUp";
                massSigma1p2_MggScaleUp[i] = new TH1D(workName.c_str(),work.c_str(), nbin_massGen, xmin_mass, xmax_mass);
                massSigma1p2_MggScaleUp[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p2_MggScaleUp[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p2_MggScaleUp[i]-> Sumw2();

                work = "#sigma 1p2 MggScaleDown for 300 fb^{-1}, " + title[i];
                workName = titleCard[i]+"_MggScaleDown";
                massSigma1p2_MggScaleDown[i] = new TH1D(workName.c_str(),work.c_str(), nbin_massGen, xmin_mass, xmax_mass);
                massSigma1p2_MggScaleDown[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p2_MggScaleDown[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p2_MggScaleDown[i]-> Sumw2();

                work = "#sigma 1p4 for 300 fb^{-1}, " + title[i];
                workName = titleCard_SigmaUp[i];
                massSigma1p4[i] = new TH1D(workName.c_str(),work.c_str(), nbin_massGen, xmin_mass, xmax_mass);
                massSigma1p4[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p4[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p4[i]-> Sumw2();

                work = "#sigma 1p0 for 300 fb^{-1}, " + title[i];
                workName = titleCard_SigmaDown[i];
                massSigma1p0[i] = new TH1D(workName.c_str(),work.c_str(), nbin_massGen, xmin_mass, xmax_mass);
                massSigma1p0[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p0[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p0[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma), " + title[i];
                workName = "hpTgg_" + titleDir[i];
                //Int_t nbin_pT = 200;
                Int_t nbin_pT = 100;
                Double_t xmin_pT = -0.5;
                Double_t xmax_pT = 999.5;
                hHiggsPt[i] = new TH1D(workName.c_str(),work.c_str(), nbin_pT, 0., 1000.);
                TitleX = "p_{T}(#gamma#gamma) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hHiggsPt[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hHiggsPt[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsPt[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPt[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma) Cut " + title[i];
                workName = "hpTggCut_" + titleDir[i];
                hHiggsPtCut[i] = new TH1D(workName.c_str(),work.c_str(), nbin_pT, 0., 1000.);
                hHiggsPtCut[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsPtCut[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPtCut[i]-> Sumw2();

                work = "p_{T}(leading jet) " + title[i];
                workName = "hLeadingJet_pT_" + titleDir[i];
                hLeadingJet_pT[i] = new TH1D(workName.c_str(),work.c_str(), nbin_pT, 0., 1000.);
                TitleX = "p_{T}(leading jet) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hLeadingJet_pT[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hLeadingJet_pT[i]-> GetXaxis()->SetTitle(TitleX);
                hLeadingJet_pT[i]-> GetYaxis()->SetTitle(TitleY);
                hLeadingJet_pT[i]-> Sumw2();

                work = "#eta(leading jet) " + title[i];
                workName = "hLeadingJet_eta_" + titleDir[i];
                hLeadingJet_eta[i] = new TH1D(workName.c_str(),work.c_str(), 200, -2.5, 2.5);
                TitleX = "#eta(leading jet) (GeV)";
                TitleY = Form("Events/(%3.0f GeV)", hLeadingJet_eta[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hLeadingJet_eta[i]-> GetXaxis()->SetTitle(TitleX);
                hLeadingJet_eta[i]-> GetYaxis()->SetTitle(TitleY);
                hLeadingJet_eta[i]-> Sumw2();

                work = "# Fat jets " + title[i];
                workName = "hNFatJet_cut_" + titleDir[i];
                hNFatJet_cut[i] = new TH1D(workName.c_str(),work.c_str(), 10, -0.5, 9.5);
                TitleX = "# Fat jets";
                TitleY = "Events"; // could make it only after histo creating
                hNFatJet_cut[i]-> GetXaxis()->SetTitle(TitleX);
                hNFatJet_cut[i]-> GetYaxis()->SetTitle(TitleY);
                hNFatJet_cut[i]-> Sumw2();

                work = "Mass of 2 FatJets, " + title[i];
                workName = "hFatJet_Mass2Jet_" + titleDir[i];
                hFatJet_Mass2Jet[i] = new TH1D(workName.c_str(),work.c_str(), 1000, 0., 1000.);
                TitleX = "M of 2 FatJets (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hFatJet_Mass2Jet[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_Mass2Jet[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_Mass2Jet[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_Mass2Jet[i]-> Sumw2();

                work = "Mass of Leading FatJet, " + title[i];
                workName = "hFatJet_MassLeading_" + titleDir[i];
                hFatJet_MassLeading[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of Leading FatJet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hFatJet_MassLeading[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_MassLeading[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_MassLeading[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_MassLeading[i]-> Sumw2();

                work = "Mass of SubLeading FatJet, " + title[i];
                workName = "hFatJet_MassSubLeading_" + titleDir[i];
                hFatJet_MassSubLeading[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of SubLeading FatJet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hFatJet_MassSubLeading[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_MassSubLeading[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_MassSubLeading[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_MassSubLeading[i]-> Sumw2();

                work = "Mass of Rest FatJet, " + title[i];
                workName = "hFatJet_MassRest_" + titleDir[i];
                hFatJet_MassRest[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of Rest FatJet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hFatJet_MassRest[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_MassRest[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_MassRest[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_MassRest[i]-> Sumw2();

                work = "#DeltaR(Fat Jet, Gen Higgs) at S, " + title[i];
                workName = "hFatJet_Higgs_dR_S_" + titleDir[i];
                hFatJet_Higgs_dR_S[i] = new TH1D(workName.c_str(),work.c_str(), 500, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.3f)", hFatJet_Higgs_dR_S[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_Higgs_dR_S[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_Higgs_dR_S[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_Higgs_dR_S[i]-> Sumw2();

                work = "#DeltaR(Fat Jet, Gen Higgs) at BS, " + title[i];
                workName = "hFatJet_Higgs_dR_BS_" + titleDir[i];
                hFatJet_Higgs_dR_BS[i] = new TH1D(workName.c_str(),work.c_str(), 100, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.2f)", hFatJet_Higgs_dR_BS[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_Higgs_dR_BS[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_Higgs_dR_BS[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_Higgs_dR_BS[i]-> Sumw2();

                work = "#DeltaR(#gamma, #gamma) for Gen Higgs, " + title[i];
                workName = "hFatJet_Higgs_dR_ggGen_" + titleDir[i];
                hFatJet_Higgs_dR_ggGen[i] = new TH1D(workName.c_str(),work.c_str(), 500, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.3f)", hFatJet_Higgs_dR_ggGen[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hFatJet_Higgs_dR_ggGen[i]-> GetXaxis()->SetTitle(TitleX);
                hFatJet_Higgs_dR_ggGen[i]-> GetYaxis()->SetTitle(TitleY);
                hFatJet_Higgs_dR_ggGen[i]-> Sumw2();

                work = "# Higgs: Reco (1), Fat Jet (10), Jet (100) " + title[i];
                workName = "hNHiggsReco_" + titleDir[i];
                hNHiggsReco[i] = new TH1D(workName.c_str(),work.c_str(), 250, -0.5, 249.5);
                TitleX = "# Reco Higgs per event";
                TitleY = "Events"; // could make it only after histo creating
                hNHiggsReco[i]-> GetXaxis()->SetTitle(TitleX);
                hNHiggsReco[i]-> GetYaxis()->SetTitle(TitleY);
                hNHiggsReco[i]-> Sumw2();

                work = "# jets " + title[i];
                workName = "hNJet_cut_" + titleDir[i];
                hNJet_cut[i] = new TH1D(workName.c_str(),work.c_str(), 10, -0.5, 9.5);
                TitleX = "# jets";
                TitleY = "Events"; // could make it only after histo creating
                hNJet_cut[i]-> GetXaxis()->SetTitle(TitleX);
                hNJet_cut[i]-> GetYaxis()->SetTitle(TitleY);
                hNJet_cut[i]-> Sumw2();

                work = "Mass of 2 Jets, " + title[i];
                workName = "hJet_Mass2Jet_" + titleDir[i];
                hJet_Mass2Jet[i] = new TH1D(workName.c_str(),work.c_str(), 1000, 0., 1000.);
                TitleX = "M of 2 Jets (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hJet_Mass2Jet[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_Mass2Jet[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_Mass2Jet[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_Mass2Jet[i]-> Sumw2();

                work = "Mass of Leading Jet, " + title[i];
                workName = "hJet_MassLeading_" + titleDir[i];
                hJet_MassLeading[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of Leading Jet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hJet_MassLeading[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_MassLeading[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_MassLeading[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_MassLeading[i]-> Sumw2();

                work = "Mass of SubLeading Jet, " + title[i];
                workName = "hJet_MassSubLeading_" + titleDir[i];
                hJet_MassSubLeading[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of SubLeading Jet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hJet_MassSubLeading[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_MassSubLeading[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_MassSubLeading[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_MassSubLeading[i]-> Sumw2();

                work = "Mass of Rest Jet, " + title[i];
                workName = "hJet_MassRest_" + titleDir[i];
                hJet_MassRest[i] = new TH1D(workName.c_str(),work.c_str(), 200, 0., 200.);
                TitleX = "M of Rest Jet (GeV)";
                TitleY = Form("Events/(%1.1f GeV)", hJet_MassRest[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_MassRest[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_MassRest[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_MassRest[i]-> Sumw2();

                work = "#DeltaR(Jet, Gen Higgs) at S, " + title[i];
                workName = "hJet_Higgs_dR_S_" + titleDir[i];
                hJet_Higgs_dR_S[i] = new TH1D(workName.c_str(),work.c_str(), 500, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.3f)", hJet_Higgs_dR_S[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_Higgs_dR_S[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_Higgs_dR_S[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_Higgs_dR_S[i]-> Sumw2();

                work = "#DeltaR(Jet, Gen Higgs) at BS, " + title[i];
                workName = "hJet_Higgs_dR_BS_" + titleDir[i];
                hJet_Higgs_dR_BS[i] = new TH1D(workName.c_str(),work.c_str(), 100, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.2f)", hJet_Higgs_dR_BS[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_Higgs_dR_BS[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_Higgs_dR_BS[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_Higgs_dR_BS[i]-> Sumw2();

                work = "#DeltaR(#gamma, #gamma) for Gen Higgs, " + title[i];
                workName = "hJet_Higgs_dR_ggGen_" + titleDir[i];
                hJet_Higgs_dR_ggGen[i] = new TH1D(workName.c_str(),work.c_str(), 500, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)";
                TitleY = Form("Events/(%1.3f)", hJet_Higgs_dR_ggGen[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hJet_Higgs_dR_ggGen[i]-> GetXaxis()->SetTitle(TitleX);
                hJet_Higgs_dR_ggGen[i]-> GetYaxis()->SetTitle(TitleY);
                hJet_Higgs_dR_ggGen[i]-> Sumw2();

                work = "Higgs Mass Gen " + title[i];
                workName = "hHiggsMass_"+titleDir[i];
                hHiggsMass[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                hHiggsMass[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsMass[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsMass[i]-> Sumw2();

                work = "Higgs Mass Gen Cut " + title[i];
                workName = "hHiggsMass_Cut_"+titleDir[i];
                hHiggsMass_Cut[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                hHiggsMass_Cut[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsMass_Cut[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsMass_Cut[i]-> Sumw2();

                work = "#DeltaR(#gamma, #gamma) for Reco Higgs, " + title[i];
                workName = "hHiggs_dR_ggReco_" + titleDir[i];
                hHiggs_dR_ggReco[i] = new TH1D(workName.c_str(),work.c_str(), 500, 0., 5.);
                TitleX = "#DeltaR(#eta, #phi)(#gamma, #gamma)";
                TitleY = Form("Events/(%1.3f)", hHiggs_dR_ggReco[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                hHiggs_dR_ggReco[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggs_dR_ggReco[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggs_dR_ggReco[i]-> Sumw2();

        }


        string FileName = "";
        Int_t FileNumber = 1;
	//CMS 2015 config
        //for reco pT (GeV):  0-120           120-200  200-270  270-350  350-450   450-550   550-750    750-inf 
        double XsecD[] =     {14.443, 14.443, 4.9921,  1.1349,  0.58424, 0.28451, 0.084658,  0.039874,  0.0082928}; // xsec pb: merge scale 15 
        double Xsec[]  =     {14.706, 14.706, 4.989,  1.1349,  0.58424, 0.28451, 0.084658,  0.039874,  0.0082928}; // xsec pb: merge scale 22.5 
        double XsecU[] =     {14.852, 14.852, 4.9621,  1.1349,  0.58424, 0.2845,  0.084658,  0.039874,  0.0082928}; // xsec pb: merge scale 30
        double dXsec[] =     {0.0068, 0.0068, 0.0016,  0.00033, 0.00017, 8.1e-05, 2.4e-05,   1.1e-05,   2.3e-06}; // xsec pb: merge scale 22.5 
	//CMS 2015 config Mass 123 GeV
        //for reco pT (GeV):     0-120           120-200  200-270  270-350  350-450   450-550   550-750    750-inf 
        double XsecD123[] =     {14.732, 14.732, 5.0991,  0.94999,  0.50227, 0.16721, 0.085069,  0.033143,  0.0071448}; // xsec pb: merge scale 15 
        double Xsec123[]  =     {15.086, 15.086, 5.0973,  0.94999,  0.50227, 0.16721, 0.085069,  0.033143,  0.0071448}; // xsec pb: merge scale 22.5 
        double XsecU123[] =     {15.045, 15.045, 5.0715,  0.94999,  0.50227, 0.16721, 0.085069,  0.033143,  0.0071448}; // xsec pb: merge scale 30
        double dXsec123[] =     {0.0069, 0.0069, 0.0016,  0.00027,  0.00014, 4.7e-05, 2.4e-05,   9.3e-06,   2.e-06}; // xsec pb: merge scale 22.5 
	//CMS 2015 config Mass 127 GeV
        //for reco pT (GeV):     0-120           120-200  200-270  270-350  350-450   450-550   550-750    750-inf 
        double XsecD127[] =     {13.942, 13.942, 4.9134,  0.93174,  0.49542, 0.16645, 0.084656,  0.033187,  0.0072161}; // xsec pb: merge scale 15 
        double Xsec127[]  =     {14.213, 14.213, 4.91,    0.93174,  0.49542, 0.16645, 0.084656,  0.033187,  0.0072161}; // xsec pb: merge scale 22.5 
        double XsecU127[] =     {14.263, 14.263, 4.8809,  0.93174,  0.49542, 0.16645, 0.084656,  0.033187,  0.0072161}; // xsec pb: merge scale 30
        double dXsec127[] =     {0.0067, 0.0067, 0.0015,  0.00027,  0.00014, 4.7e-05, 2.4e-05,   9.3e-06,   2.e-06}; // xsec pb: merge scale 22.5 
	// CMS old config: 
        //double XsecD[] =     {14.307, 3.9088,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 15 
        //double Xsec[]  =     {14.557, 3.9086,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 22.5 
        //double XsecU[] =     {14.582, 3.9047,  0.77316, 0.26312, 0.11097, 0.038815,  0.020594,  0.0050862}; // xsec pb: merge scale 30
        //double dXsec[] =     {0.0068, 0.0012,  0.00022, 7.4e-05, 3.1e-05, 1.1e-05,   5.8e-06,   1.4e-06}; // xsec pb: merge scale 22.5 

        double XsecD_BG[] =  {15.569, 3.9041,  1.4883,  0.19437, 0.073139, 0.016648, 0.0058665, 0.0021491, 0.00020585};
	double Xsec_BG[]  =  {15.73,  3.9037,  1.4883,  0.19437, 0.073137, 0.016647, 0.0058665, 0.0021491, 0.00020585};
	double XsecU_BG[] =  {15.603, 3.8987,  1.4883,  0.19437, 0.073137, 0.016647, 0.0058665, 0.0021491, 0.00020585};
        double dXsec_BG[] =  {0.005,  0.0011,  0.00042, 5.4e-05, 2.1e-05,  4.7e-06,  1.6e-06,   6e-07,     5.8e-08};
        //CMS old config:
        //double XsecD_BG[] =  {15.562, 2.8415,  0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
	//double Xsec_BG[]  =  {15.715, 2.8415,  0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
	//double XsecU_BG[] =  {15.637, 2.841,   0.25233, 0.045449, 0.0113, 0.0023344, 0.00086844, 0.00010066};
        //double dXsec_BG[] =  {0.005,  0.0008,  7.1e-05, 1.3e-05,  3.2e-06, 6.5e-07,  2.4e-07,    2.8e-08};

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
        if(DataSample == "Signal" && (massHiggs >122.9 && massHiggs < 123.1)){
                for (int i = 0; i < NLumi; i++)
                {
                        XsecD[i] = XsecD123[i];
                        Xsec[i] = Xsec123[i];
                        XsecU[i] = XsecU123[i];
                        dXsec[i] = dXsec123[i];
                }
        }
        if(DataSample == "Signal" && (massHiggs >126.9 && massHiggs < 127.1)){
                for (int i = 0; i < NLumi; i++)
                {
                        XsecD[i] = XsecD127[i];
                        Xsec[i] = Xsec127[i];
                        XsecU[i] = XsecU127[i];
                        dXsec[i] = dXsec127[i];
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
                if (FileNumber == 1 && DataSample == "BG")FileNameCut_pTbin = Form("%d_%d", pTbin[0],pTbin[1]);
                Int_t ID_pT = FileNumber;
                if(DataSample == "BG") ID_pT = FileNumber-1;
                if (FileNumber > 1 && ID_pT < (NpTbins-1))FileNameCut_pTbin = Form("%d_%d", pTbin[ID_pT],pTbin[ID_pT+1]);
		if (ID_pT == (NpTbins-1))FileNameCut_pTbin = Form("%d_inf", pTbin[ID_pT]);
		if (ID_pT > (NpTbins-1))
		{
			cout <<"Error: more Files then pTbins, please check and correct code. Task is terminated: no more Files connecting." << endl;
			continue;
		}
                if (FileNameCut != FileNameCut_pTbin)
		{
                        //cout << "Test: FileNameCut= X" << FileNameCut << "X FileNameCut_pTbin = X" << FileNameCut_pTbin << "X "<< endl;
			FileNumber++; 
			ID_pT = FileNumber;
			if(DataSample == "BG") ID_pT = FileNumber-1;
                	if (FileNumber > 1 && ID_pT < (NpTbins-1))FileNameCut_pTbin = Form("%d_%d", pTbin[ID_pT],pTbin[ID_pT+1]);
			if (ID_pT == (NpTbins-1))FileNameCut_pTbin = Form("%d_inf", pTbin[ID_pT]);
                	if (ID_pT > (NpTbins-1))
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
                	cout << "File number = " << FileNumber << endl;
                }
		FileName = FileNameCheck;
                 
		if( jentry%10000 == 0 )
			std::cout << "Loop over entry " << jentry << " events: File number = " << FileNumber << std::endl;
                if (FileNumber > NLumi) cout << "Error: FileNumber > NLumi check code: FileNumber = " << FileNumber << " NLumi = " << NLumi << endl;
                if (FileNumber > NLumi) continue;
        	// make weight for 300 fb-1 and Br(H->gg) = 2.27e-03
                //                              pb -> fb   #fb  #events BR(H->gg)                 
        	double weight_F = Xsec[ID_pT]*1000.*300./50000.*2.27e-03;
        	if(DataSample == "BG") weight_F = Xsec[ID_pT]*1000.*300./50000.; // no Br correction
	        Int_t NHiggs_RecoPerEvent = 0;
                int indicator = -1;
                Double_t pTHiggsGen = -10;
		Int_t i_HiggsGen = -1;
                if(Particle_size > 0){
                for ( int i = 0; i < Particle_size; i++) // now loop through all particles to find Higgs
                {
			//set indicator depending on Pt
                        if ( Particle_PID[i] == 25 && Particle_Status[i] == 62) // check for Higgs and that it is final state before decay   
                        {
                                if (Particle_PT[i] < 0) cout << "Error: Particle_PT[" << i << "] = " << Particle_PT[i] << " Particle_Status = " << Particle_Status[i] << endl;
                                //for ( int j = 0; j < NpTbins; j++)
                                //{
                                //        if (j < (NpTbins -1)){if (Particle_PT[i] >= pTbinCor[j] && Particle_PT[i] < pTbinCor[j+1]) indicator = j;}
                                //        else {if (Particle_PT[i] >= pTbinCor[j]) indicator = j;}
                                //}
				int indicator_test = HiggsAnalysis_14TeV::BinNumber(ID_pT, pTbinCor, NpTbins, Particle_PT[i], DataSample);
                                //cout << "TEST: indicator_test = " << indicator_test << " ID_pT = " << ID_pT << " Particle_PT[i] = " << Particle_PT[i] << endl;
				if (indicator != -1 && indicator_test != -1) cout << "Warning: 2 gen stable Higgs per event! -> check code " << endl;
				if (indicator == -1){
					indicator = indicator_test;
					pTHiggsGen = Particle_PT[i];
					i_HiggsGen = i;
				}
				if (indicator_test > -1){
					hHiggsMass[indicator_test] -> Fill(Particle_Mass[i], weight_F);
					NHiggsGen_2g[indicator_test] = NHiggsGen_2g[indicator_test] + weight_F;
				}
				// find daugthers of Higgs:
				Int_t i1 = Particle_D1[i];
				Int_t i2 = Particle_D2[i];
				// cout << " Particle_PID[i1] = " << Particle_PID[i1] << " Particle_PID[i1] = " << Particle_PID[i1] << " Higgs Particle_Status[i] = " << Particle_Status[i] << endl;
				//if(Particle_Status[i] == 62) cout << " Particle_PID[i1] = " << Particle_PID[i1] << " Particle_PID[i1] = " << Particle_PID[i1] << endl;
				//if (Particle_PID[i1] == 22 )cout << " Particle_PID[i1] = " << Particle_PID[i1] << " Higgs Particle_Status[i] = " << Particle_Status[i] << endl;
                                if (abs(i2-i1) != 1) cout <<"ERROR: not 2 daughters, it is: " << abs(i2-i1)+1 << endl;
                                if (Particle_PID[i1] !=22 || Particle_PID[i2] !=22) cout <<"ERROR: decay to not 2 photons" << endl;
				// fine leading (i1) and subleading (i2) photons in pT
                                if (Particle_PT[i1] < Particle_PT[i2]) { Int_t i_sub = i1; i1=i2; i2 = i_sub;}
                                // Check after cuts:
				if (indicator_test > -1){
				if (fabs(Particle_Eta[i1]) < 2.5 && (fabs(Particle_Eta[i1]) > 1.57 || fabs(Particle_Eta[i1]) < 1.44) && fabs(Particle_Eta[i2]) < 2.5 && (fabs(Particle_Eta[i2]) > 1.57 || fabs(Particle_Eta[i2]) < 1.44) && Particle_PT[i1] > Particle_Mass[i]/3. && Particle_PT[i1] > Particle_Mass[i]/4.  )
					{ hHiggsMass_Cut[indicator_test] -> Fill(Particle_Mass[i],weight_F);
					  NHiggsGen_2gCut[indicator_test] = NHiggsGen_2gCut[indicator_test] + weight_F;}
				}
                        }
                }
                }
                //cout << "TEST indicator = " << indicator << endl;
                //if (indicator < 0 )std::cout <<"ERROR: no gen Higgs found" << std::endl;

		// Could change # of Reco Higgs, because come Reco could come from another bins
                if (indicator < 0)continue; // reject events without good generated Higgs 

                // find leading jet in |eta| < 2.4:
                double JetLeading_pT = 0.;
                double JetLeading_eta = -5.;
		Int_t N_jet = 0;
                if (Jet_size > 0)
                {
                for (int i = 0; i < Jet_size; i++)
                {
			if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > JetLeading_pT){JetLeading_pT = Jet_PT[i]; JetLeading_eta = Jet_Eta[i];} 
			if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > 30.)N_jet++;

                }
                }
                if (JetLeading_pT > 0.)
		{
			hLeadingJet_pT[indicator] -> Fill(JetLeading_pT, weight_F);
			hLeadingJet_eta[indicator] -> Fill(JetLeading_eta, weight_F);
		}

                // *********   analyze Fat jets:
                p1 = test;
                p2 = test;
                Int_t i_Leading = -1;
                Int_t i_SubLeading = -1;
                Int_t N_FatJet = 0;
                //Int_t FJet_size = FatJet_size; 
                Int_t FJet_size = ParticleFlowJet08_size; 

                if (FJet_size > 0)
                {
                for (int i = 0; i < FJet_size; i++)
                {
		//Float_t FJet_PT   = FatJet_PT[i];
		//Float_t FJet_Eta  = FatJet_Eta[i];
		//Float_t FJet_Phi  = FatJet_Phi[i];
		//Float_t FJet_Mass = FatJet_Mass[i];
		Float_t FJet_PT   = ParticleFlowJet08_PT[i];
		Float_t FJet_Eta  = ParticleFlowJet08_Eta[i];
		Float_t FJet_Phi  = ParticleFlowJet08_Phi[i];
		Float_t FJet_Mass = ParticleFlowJet08_Mass[i];
                if(fabs(FJet_Eta) < 2.4 && FJet_PT > 30.){
			N_FatJet++;

                                if (p1 == test)         // if this is the first Fat Jet in the event
                                {
                                        p1.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
					i_Leading = i;
                                }
                                else if ( (p1 != test) && (p2 == test) ) // if this is the second Fat Jet in the event p1 is with max pT
                                {
                                        if(p1.Pt() >= FJet_PT){p2.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass); i_SubLeading = i;}
                                        else
                                        {
                                             p2 = p1;
                                             p1.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
					     i_SubLeading = i_Leading; i_Leading = i;
                                        }
                                }
                                else if ( (p1 != test) && (p2 != test) )        // maybe another photon, trying to get max pTs
                                {
                                        if ( FJet_PT > p1.Pt() )
                                        {
                                                        p2 = p1;
                                                        p1.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
					     		i_SubLeading = i_Leading; i_Leading = i;
                                        }
                                        else if ( FJet_PT > p2.Pt() )
                                        {
                                                p2.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
						i_SubLeading = i;
                                        }
                                }
		
                }}}
                
                hNFatJet_cut[indicator] -> Fill(N_FatJet, weight_F);
                if((p1 != test) ){
			hFatJet_MassLeading[indicator] -> Fill(p1.M(), weight_F);
			if(p1.M() > 120. && p1.M() < 140.){
				NHiggsReco_FatJet[indicator] = NHiggsReco_FatJet[indicator] + weight_F;
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +10;
				hFatJet_Higgs_dR_S[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p1)); // Fill dR at Signal region
				Int_t i1 = Particle_D1[i_HiggsGen];
                                Int_t i2 = Particle_D2[i_HiggsGen];
				hFatJet_Higgs_dR_ggGen[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(Particle_Eta[i1], Particle_Phi[i1], Particle_Eta[i2], Particle_Phi[i2])); // Fill dR for Higgs gg Gen
			}
			//subtract background:
			if( (p1.M() > 110. && p1.M() < 120.) || (p1.M() > 140. && p1.M() < 150.)){
				NHiggsReco_FatJet[indicator] = NHiggsReco_FatJet[indicator] - weight_F;
				hFatJet_Higgs_dR_BS[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p1)); // Fill dR at Side Bend region
			}
		}
                if((p2 != test) ){
			hFatJet_MassSubLeading[indicator] -> Fill(p2.M(), weight_F);
			if(p2.M() > 120. && p2.M() < 140.){
				NHiggsReco_FatJet[indicator] = NHiggsReco_FatJet[indicator] + weight_F;
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +10;
				hFatJet_Higgs_dR_S[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p2)); // Fill dR at Signal region
				Int_t i1 = Particle_D1[i_HiggsGen];
                                Int_t i2 = Particle_D2[i_HiggsGen];
				hFatJet_Higgs_dR_ggGen[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(Particle_Eta[i1], Particle_Phi[i1], Particle_Eta[i2], Particle_Phi[i2])); // Fill dR for Higgs gg Gen
			}
			//subtract background:
			if( (p2.M() > 110. && p2.M() < 120.) || (p2.M() > 140. && p2.M() < 150.)){
				NHiggsReco_FatJet[indicator] = NHiggsReco_FatJet[indicator] - weight_F;
				hFatJet_Higgs_dR_BS[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p2)); // Fill dR at Side Bend region
			}
		}
		// fill Fat jet mass without leading and subleading jets:
                if (FJet_size > 0)
                {
                for (int i = 0; i < FJet_size; i++)
                {
                Float_t FJet_PT   = ParticleFlowJet08_PT[i];
                Float_t FJet_Eta  = ParticleFlowJet08_Eta[i];
                Float_t FJet_Phi  = ParticleFlowJet08_Phi[i];
                Float_t FJet_Mass = ParticleFlowJet08_Mass[i];
                if(fabs(FJet_Eta) < 2.4 && FJet_PT > 30.){

			if (i != i_Leading && i != i_SubLeading){p_temp1.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass); hFatJet_MassRest[indicator] -> Fill(p_temp1.M(), weight_F);}
                }}}
                //if((p1 != test) && (p2 != test))hFatJet_Mass2Jet[indicator] -> Fill((p1+p2).M(), weight_F);
		//fill mass of any 2 FatJet:
                if (FJet_size > 1)
                {
                for (int i = 0; i < FJet_size-1; i++)
               	{
                Float_t FJet_PT   = ParticleFlowJet08_PT[i];
                Float_t FJet_Eta  = ParticleFlowJet08_Eta[i];
                Float_t FJet_Phi  = ParticleFlowJet08_Phi[i];
                Float_t FJet_Mass = ParticleFlowJet08_Mass[i];
               		if(fabs(FJet_Eta) < 2.4 && FJet_PT > 30.){
				p_temp1.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
				for (int j = i+1; j < FJet_size; j++)
				{
					if(fabs(FJet_Eta) < 2.4 && FJet_PT > 30.){
						p_temp2.SetPtEtaPhiM(FJet_PT, FJet_Eta, FJet_Phi, FJet_Mass);
						hFatJet_Mass2Jet[indicator] -> Fill((p_temp1+p_temp2).M(), weight_F);
					}
				}
			}
		}}
                // *********   end: analyze Fat jets
                // *********   analyze normal jets:
                p1 = test;
                p2 = test;
                i_Leading = -1;
                i_SubLeading = -1;
                Int_t N_Jet = 0;

                if (Jet_size > 0)
                {
                for (int i = 0; i < Jet_size; i++)
                {
                if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > 30.){
			N_Jet++;

                                if (p1 == test)         // if this is the first  Jet in the event
                                {
                                        p1.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]);
					i_Leading = i;
                                }
                                else if ( (p1 != test) && (p2 == test) ) // if this is the second  Jet in the event p1 is with max pT
                                {
                                        if(p1.Pt() >= Jet_PT[i]){p2.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]); i_SubLeading = i;}
                                        else
                                        {
                                             p2 = p1;
                                             p1.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]);
					     i_SubLeading = i_Leading; i_Leading = i;
                                        }
                                }
                                else if ( (p1 != test) && (p2 != test) )        // maybe another photon, trying to get max pTs
                                {
                                        if ( Jet_PT[i] > p1.Pt() )
                                        {
                                                        p2 = p1;
                                                        p1.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]);
					     		i_SubLeading = i_Leading; i_Leading = i;
                                        }
                                        else if ( Jet_PT[i] > p2.Pt() )
                                        {
                                                p2.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]);
						i_SubLeading = i;
                                        }
                                }
		
                }}}
                
                hNJet_cut[indicator] -> Fill(N_Jet, weight_F);
                if((p1 != test) ){
			hJet_MassLeading[indicator] -> Fill(p1.M(), weight_F);
			if(p1.M() > 120. && p1.M() < 140.){
				NHiggsReco_Jet[indicator] = NHiggsReco_Jet[indicator] + weight_F;
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +100;
                                hJet_Higgs_dR_S[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p1)); // Fill dR at Signal region
                                Int_t i1 = Particle_D1[i_HiggsGen];
                                Int_t i2 = Particle_D2[i_HiggsGen];
                                hJet_Higgs_dR_ggGen[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(Particle_Eta[i1], Particle_Phi[i1], Particle_Eta[i2], Particle_Phi[i2])); // Fill dR for Higgs gg Gen
			}
			//subtract background:
			if( (p1.M() > 110. && p1.M() < 120.) || (p1.M() > 140. && p1.M() < 150.)){
				NHiggsReco_Jet[indicator] = NHiggsReco_Jet[indicator] - weight_F;
				hJet_Higgs_dR_BS[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p1)); // Fill dR at Side Bend region
			}

		}
                if((p2 != test) ){
			hJet_MassSubLeading[indicator] -> Fill(p2.M(), weight_F);
			if(p2.M() > 120. && p2.M() < 140.) {
				NHiggsReco_Jet[indicator] = NHiggsReco_Jet[indicator] + weight_F;
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +100;
                                hJet_Higgs_dR_S[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p2)); // Fill dR at Signal region
                                Int_t i1 = Particle_D1[i_HiggsGen];
                                Int_t i2 = Particle_D2[i_HiggsGen];
                                hJet_Higgs_dR_ggGen[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(Particle_Eta[i1], Particle_Phi[i1], Particle_Eta[i2], Particle_Phi[i2])); // Fill dR for Higgs gg Gen
			}
			//subtract background:
			if( (p2.M() > 110. && p2.M() < 120.) || (p2.M() > 140. && p2.M() < 150.)){
				NHiggsReco_Jet[indicator] = NHiggsReco_Jet[indicator] - weight_F;
				hJet_Higgs_dR_BS[indicator] -> Fill(HiggsAnalysis_14TeV::DeltaR(Particle_Eta[i_HiggsGen], Particle_Phi[i_HiggsGen], p2)); // Fill dR at Side Bend region
			}
		}
		// fill  jet mass without leading and subleading jets:
                if (Jet_size > 0)
                {
                for (int i = 0; i < Jet_size; i++)
                {
                if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > 30.){

			if (i != i_Leading && i != i_SubLeading){p_temp1.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]); hJet_MassRest[indicator] -> Fill(p_temp1.M(), weight_F);}
                }}}
                //if((p1 != test) && (p2 != test))hJet_Mass2Jet[indicator] -> Fill((p1+p2).M(), weight_F);
		//fill mass of any 2 Jet:
                if (Jet_size > 1)
                {
                for (int i = 0; i < Jet_size-1; i++)
               	{
               		if(fabs(Jet_Eta[i]) < 2.4 && Jet_PT[i] > 30.){
				p_temp1.SetPtEtaPhiM(Jet_PT[i], Jet_Eta[i], Jet_Phi[i], Jet_Mass[i]);
				for (int j = i+1; j < Jet_size; j++)
				{
					if(fabs(Jet_Eta[j]) < 2.4 && Jet_PT[j] > 30.){
						p_temp2.SetPtEtaPhiM(Jet_PT[j], Jet_Eta[j], Jet_Phi[j], Jet_Mass[j]);
						hJet_Mass2Jet[indicator] -> Fill((p_temp1+p_temp2).M(), weight_F);
					}
				}
			}
		}}
                // *********   end: analyze normal  jets



                // move to Reco part:
                int indicatorReco = -1;
                p1 = test;
                p2 = test;
                int flag = 0;
                bool check = 0;
                double mass = 0;


                if (Photon_size < 2) continue; //select at least 2 reco photons:
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



                        // set indicatorReco:
                        //if (p1 == test || p2 == test) cout << "Error: one both photons are not exist -> check the code: pT1 = " << p1.Pt() << " pT2 = " << p2.Pt() << endl;   // if both photons filled
                if (p1 == test || p2 == test) continue;
                s = p1+p2;

                //for ( int j = 0; j < NpTbins; j++)
                //{
                //        if (j < (NpTbins -1)){if (s.Pt()>= pTbinCor[j] && s.Pt() < pTbinCor[j+1]) indicatorReco = j;}
                //      else {if (s.Pt() >= pTbinCor[j]) indicatorReco = j;}
                //}

                indicatorReco = HiggsAnalysis_14TeV::BinNumber(ID_pT, pTbinCor, NpTbins, s.Pt(), DataSample);
                if (indicatorReco < 0) continue; //reject events without reco matching

                mass = s.M();
                float pTHiggs = s.Pt();
                //hHiggsMass -> Fill (mass);
                if ( (p1.Pt() > mass/3) && (p2.Pt() > mass/4) && JetLeading_pT > 30. ) // make cut of leaning jet and |eta_jet| < 2.4
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
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +1; // BB + BE
				NHiggsReco[indicatorReco] = NHiggsReco[indicatorReco] + weight_F; // BB + BE
				bothBarrel[indicatorReco] -> Fill(mass,weight_F);
				hHiggs_dR_ggReco[indicatorReco] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(p1.Eta(), p1.Phi(), p2.Eta(), p2.Phi() )); // Fill dR for Higgs gg Reco
                        }
                        else if (flag == 2)
                        {
				NHiggs_RecoPerEvent = NHiggs_RecoPerEvent +1; // BB + BE
				NHiggsReco[indicatorReco] = NHiggsReco[indicatorReco] + weight_F; // BB + BE
				barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
				hHiggs_dR_ggReco[indicatorReco] -> Fill(HiggsAnalysis_14TeV::DeltaR_F(p1.Eta(), p1.Phi(), p2.Eta(), p2.Phi() )); // Fill dR for Higgs gg Reco
                        }
                        else if (flag == 3)
                        {
				bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
                        }
			// fill pT gamma-gamma bins:
                        Ncheck ++;
			if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
                        else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
			
                        if (ID_pT < (NpTbins-2) && pTHiggs < pTbinCor[ID_pT+2])
			{
                        	if (pTHiggs < 1000.)hHiggsPt[ID_pT] -> Fill (pTHiggs,weight_F);
				else hHiggsPt[ID_pT] -> Fill (1000.,weight_F);
                        }
                        if (ID_pT >= (NpTbins-2))
			{
                        	if (pTHiggs < 1000.)hHiggsPt[ID_pT] -> Fill (pTHiggs,weight_F);
				else hHiggsPt[ID_pT] -> Fill (1000.,weight_F);
                        }
			if( Ncheck%1000 == 0 ){
				std::cout << "Selected events = " << Ncheck << " #File = " << FileNumber << " weight_F = " << weight_F << " indicatorReco = " << indicatorReco << " indicator = " << indicator << " pT(gg) = " << pTHiggs << " pTHiggsGen = " << pTHiggsGen << std::endl;
			}
                } //end check

		hNHiggsReco[indicator] -> Fill(NHiggs_RecoPerEvent);

        } // end cyclbe by nentries

        TFile *outFile = new TFile ("Test.root", "RECREATE");
        if(DataSample == "Signal") outFile = new TFile (Form("Signal_mH%3.0fGeV.root",massHiggs), "RECREATE");
        if(DataSample == "BG") outFile = new TFile ( "Background.root", "RECREATE");
	gStyle->SetCanvasDefW(2000);
	gStyle->SetCanvasDefH(2000);
        //gStyle->SetPadLeftMargin(0.25);
        gStyle->SetPadLeftMargin(0.13);
        TCanvas *first[NpTbins];

        //TDirectory *DirName[NpTbins];

        for (int i = 0; i < NpTbins; i++) // draw and fit histograms
        {
                Double_t func_xmin = massHiggs - 7.; 
                Double_t func_xmax = massHiggs + 7.; 
                TF1 *FitFunc = new TF1("FitFunc","[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])",func_xmin,func_xmax); 
                FitFunc->SetNpx(1000);
                FitFunc->SetParameter(0, 120./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(2, 1.5); //n
                FitFunc->SetParameter(3, 1.2+i*0.1); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                FitFunc->SetParName(0, "A");
                FitFunc->SetParName(1, "alpha");
                FitFunc->SetParName(2, "n");
                FitFunc->SetParName(3, "sigma");
                FitFunc->SetParName(4, "mass");
                //FitFunc->SetParLimits(1, -10., 10.); //alpha
                //FitFunc->SetParLimits(3, 0.8, 2.); //sigma
                //FitFunc->SetParLimits(4, 121.,129.); //Higgs mass
                FitFunc->FixParameter(2, 1.5); //n
                //FitFunc->FixParameter(3, 1.3); //sigma
                //FitFunc->FixParameter(4, 125.); //Higgs mass

                TF1 *FitFuncBG = new TF1("FitFuncBG","[0]+[1]*x",110,120); 
                FitFuncBG->SetNpx(1000);
                FitFuncBG->SetParName(0, "p0");
                FitFuncBG->SetParName(1, "p1");

                TF1 *FitFuncGaus = new TF1("FitFuncGaus","[0]*exp(-0.5*((x-[1])/[2])**2)",func_xmin,func_xmax); 
                FitFuncGaus->SetNpx(1000);
                FitFuncGaus->SetParameter(0, 120./pow(3.,i)); //A
                FitFuncGaus->SetParameter(1, massHiggs); //mass
                FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                FitFuncGaus->SetParName(0, "A");
                FitFuncGaus->SetParName(1, "mass");
                FitFuncGaus->SetParName(2, "sigma");

                first[i] = new TCanvas();
                if(DataSample == "BG")first[i] -> Divide(2,2);
                if(DataSample == "Signal")first[i] -> Divide(3,2);
                gStyle->SetOptFit(1);
                //gStyle->SetOptStat(1); 

                first[i] -> cd(1);
                //first[i] -> SetLeftMargin(0.15);
                bothBarrel[i]->Draw();
                if(DataSample == "Signal")bothBarrel[i]->Fit("FitFuncGaus");
                //if(DataSample == "Signal")bothBarrel[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")bothBarrel[i]->Fit("FitFuncBG","","",110,140);

                first[i] -> cd(2);
                barrelAndEndCaps[i]->Draw();
                FitFunc->SetParameter(0, 60./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(3, 1.2+i*0.1); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                FitFuncGaus->SetParameter(0, 60./pow(3.,i)); //A
                FitFuncGaus->SetParameter(1, massHiggs); //mass
                FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                if(DataSample == "Signal")barrelAndEndCaps[i]->Fit("FitFuncGaus");
                //if(DataSample == "Signal")barrelAndEndCaps[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")barrelAndEndCaps[i]->Fit("FitFuncBG","","",110,140);

                //first[i] -> cd(3);
                //bothEndCaps[i]->Draw();
                //FitFunc->SetParameter(0, 12./pow(3.,i)); //A
                //FitFunc->SetParameter(1, -3.); //alpha
                //FitFunc->SetParameter(3, 1.5); //sigma
                //FitFunc->SetParameter(4, massHiggs); //Higgs mass
                //if(DataSample == "Signal")bothEndCaps[i]->Fit("FitFunc","RB");
                //if(DataSample == "BG")bothEndCaps[i]->Fit("FitFuncBG","","",110,140);

                first[i] -> cd(3);
                massBBandBE[i]->Add(bothBarrel[i],barrelAndEndCaps[i]);
                massBBandBE[i]->Draw();
                FitFunc->SetParameter(0, 200./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(3, 1.2+i*0.1); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                FitFuncGaus->SetParameter(0, 200./pow(3.,i)); //A
                FitFuncGaus->SetParameter(1, massHiggs); //mass
                FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                if(DataSample == "Signal")massBBandBE[i]->Fit("FitFuncGaus");
                //if(DataSample == "Signal")massBBandBE[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")massBBandBE[i]->Fit("FitFuncBG","","",110,140);

        	////Int_t nbin_mass = 60;
        	////Double_t xmin_mass = 110.;              
        	////Double_t xmax_mass = 140.; 
        	//for (int j = 1; j <= nbin_mass; j++){
                //        Double_t binWidth = (xmax_mass-xmin_mass)/nbin_mass;
		//	Double_t binValue = FitFuncGaus->Integral(xmin_mass+(j-1)*binWidth, xmin_mass+j*binWidth);
		//	if(DataSample == "BG")binValue = FitFuncBG->Integral(xmin_mass+(j-1)*binWidth, xmin_mass+j*binWidth);
                //        massSigma1p0[i]->SetBinContent(j,binValue/binWidth);
                //        massSigma1p0[i]->SetBinError(j,0.01*binValue/binWidth);
		//}
                if(DataSample == "Signal"){
                        Double_t GausWidth = FitFuncGaus->GetParameter(2); // get width form fit
                        cout << "**************** i = " << i << " gaus width = " << GausWidth << endl;
			massSigma1p0[i]->FillRandom("FitFuncGaus",50000);

		}
                if(DataSample == "BG")massSigma1p0[i]->FillRandom("FitFuncBG",200000);
                Double_t Integral_GEN = massSigma1p0[i]->Integral();
                if(Integral_GEN > 0.)massSigma1p0[i]->Scale(massBBandBE[i]->Integral()/Integral_GEN);
                first[i] -> cd(4);
                massSigma1p0[i]->Draw();
                FitFunc->SetParameter(0, 200./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(3, 1.5); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                FitFuncGaus->SetParameter(0, 200./pow(3.,i)); //A
                FitFuncGaus->SetParameter(1, massHiggs); //mass
                FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                if(DataSample == "Signal")massSigma1p0[i]->Fit("FitFuncGaus");
                //if(DataSample == "Signal")massSigma1p0[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")massSigma1p0[i]->Fit("FitFuncBG","","",110,140);


		if(DataSample == "Signal"){

                        first[i] -> cd(5);
                        massBBandBE[i]->Fit("FitFuncGaus"); // get fit partameter
                        Double_t GausWidth = FitFuncGaus->GetParameter(2); // get width form fit
                        cout << "**************** i = " << i << " gaus width = " << GausWidth << " gaus width*1.2 = " << 1.2*GausWidth << endl;
                        FitFuncGaus->SetParameter(2,1.2*GausWidth);// Set corrected width
                        massSigma1p2[i]->FillRandom("FitFuncGaus",50000);
                        Double_t Integral_GEN = massSigma1p2[i]->Integral();
                        if(Integral_GEN > 0.)massSigma1p2[i]->Scale(massBBandBE[i]->Integral()/Integral_GEN);
			Double_t GausMass = FitFuncGaus->GetParameter(1); // get mass form fit
			//start Mgg Scale variation
                        FitFuncGaus->SetParameter(1,(GausMass+0.05));// Set corrected for mass by +50 Mev
                        massSigma1p2_MggScaleUp[i]->FillRandom("FitFuncGaus",50000);
                        if(Integral_GEN > 0.)massSigma1p2_MggScaleUp[i]->Scale(massBBandBE[i]->Integral()/Integral_GEN);
                        FitFuncGaus->SetParameter(1,(GausMass-0.05));// Set corrected for mass by -50 Mev
                        massSigma1p2_MggScaleDown[i]->FillRandom("FitFuncGaus",50000);
                        if(Integral_GEN > 0.)massSigma1p2_MggScaleDown[i]->Scale(massBBandBE[i]->Integral()/Integral_GEN);
			//end Mgg Scale variation
                        massSigma1p2[i]->Draw();
                        FitFuncGaus->SetParameter(0, 200./pow(3.,i)); //A
                        FitFuncGaus->SetParameter(1, massHiggs); //mass
                        FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                        massSigma1p2[i]->Fit("FitFuncGaus");

                	first[i] -> cd(6);
			massBBandBE[i]->Fit("FitFuncGaus"); // get fit partameter
			GausWidth = FitFuncGaus->GetParameter(2); // get width form fit
			cout << "**************** i = " << i << " gaus width = " << GausWidth << " gaus width*1.4 = " << 1.4*GausWidth << endl;
			FitFuncGaus->SetParameter(2,1.4*GausWidth);// Set corrected width
			massSigma1p4[i]->FillRandom("FitFuncGaus",50000);
                	Integral_GEN = massSigma1p4[i]->Integral();
                	if(Integral_GEN > 0.)massSigma1p4[i]->Scale(massBBandBE[i]->Integral()/Integral_GEN);
                	massSigma1p4[i]->Draw();
                	FitFuncGaus->SetParameter(0, 200./pow(3.,i)); //A
                	FitFuncGaus->SetParameter(1, massHiggs); //mass
                	FitFuncGaus->SetParameter(2, 1.2+i*0.1); //sigma
                	massSigma1p4[i]->Fit("FitFuncGaus");


		}

        	//DirName[i]= outFile->mkdir(titleDir[i]);
        	//DirName[i]= outFile->mkdir("Test");
		TString DirRef = titleDir[i]; 
        	outFile->mkdir(Form("%s",DirRef.Data() ));
                outFile->   cd(Form("%s",DirRef.Data() ));
		//DirName[i]->Write();
                //bothBarrel[i]->Write();
                if(DataSample == "Signal" && (massHiggs >124.9 && massHiggs < 125.1)){
			massBBandBE[i]->Write("original_signal");
                	massSigma1p0[i]->Write("signal_sigma1p0");
                	massSigma1p2[i]->Write("signal_sigma1p2");
                	massSigma1p2_MggScaleUp[i]->Write("signal_sigma1p2_MggScaleUp");
                	massSigma1p2_MggScaleDown[i]->Write("signal_sigma1p2_MggScaleDown");
                	massSigma1p2[i]->Write("data_obs");// only 1p2 and only for mH = 125.
                	massSigma1p4[i]->Write("signal_sigma1p4");
		}
		if(DataSample == "Signal" && (massHiggs >122.9 && massHiggs < 123.1)){
			massBBandBE[i]->Write("original_signal_MassDown");
                	massSigma1p0[i]->Write("signal_sigma1p0_MassDown");
                	massSigma1p2[i]->Write("signal_sigma1p2_MassDown");
                	massSigma1p4[i]->Write("signal_sigma1p4_MassDown");
		}
		if(DataSample == "Signal" && (massHiggs >126.9 && massHiggs < 127.1)){
			massBBandBE[i]->Write("original_signal_MassUp");
                	massSigma1p0[i]->Write("signal_sigma1p0_MassUp");
                	massSigma1p2[i]->Write("signal_sigma1p2_MassUp");
                	massSigma1p4[i]->Write("signal_sigma1p4_MassUp");
		}

                if(DataSample == "BG"){
			massBBandBE[i]->Write("original_background");
                	massSigma1p0[i]->Write("background");
                	massSigma1p0[i]->Write("data_obs");// for BG 
		}
                //barrelAndEndCaps[i]->Write();
                //bothEndCaps[i]->Write();
                //hHiggsPt[i]->Write();
                //hHiggsPtCut[i]->Write();
		// for efficiency caclulation only
	                outFile->mkdir(Form("Eff_%s%3.0f_%s",DataSample.Data(),massHiggs,DirRef.Data() ));
        	        outFile->   cd(Form("Eff_%s%3.0f_%s",DataSample.Data(),massHiggs,DirRef.Data() ));

                	hNFatJet_cut[i]->Write();
                	hFatJet_Mass2Jet[i]->Write();
                	hFatJet_MassLeading[i]->Write();
                	hFatJet_MassSubLeading[i]->Write();
                	hFatJet_MassRest[i]->Write();
			hFatJet_Higgs_dR_S[i]->Write();
			hFatJet_Higgs_dR_BS[i]->Write();
			hFatJet_Higgs_dR_ggGen[i]->Write();

                	hNJet_cut[i]->Write();
                	hLeadingJet_pT[i]->Write();
             	        hLeadingJet_eta[i]->Write();
                	hJet_Mass2Jet[i]->Write();
                	hJet_MassLeading[i]->Write();
                	hJet_MassSubLeading[i]->Write();
                	hJet_MassRest[i]->Write();
			hJet_Higgs_dR_S[i]->Write();
			hJet_Higgs_dR_BS[i]->Write();
			hJet_Higgs_dR_ggGen[i]->Write();

                	hNHiggsReco[i]->Write();
			hHiggs_dR_ggReco[i]->Write();
                	hHiggsMass_Cut[i]->Write();
                	hHiggsMass[i]->Write();
                // end for efficiency calculation
                if(DataSample == "Signal")first[i] -> Print(Form("Plots/mH%3.0f_pTbin_%d.png",massHiggs,i));
                if(DataSample == "BG")first[i] -> Print(Form("Plots/BG_pTbin_%d.png",i));
        }
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0); 
        TCanvas *Dist_pTgg;
        Dist_pTgg = new TCanvas();
        Dist_pTgg -> Divide(1,1);
        Dist_pTgg -> cd(1);
        //Dist_pTgg -> SetLeftMargin(0.15);
        gPad -> SetLogx(1);
        gPad -> SetLogy(1);
        Double_t xmin_Empty = 80.; 
        Double_t xmax_Empty = 1000.; 
        Double_t ymin_Empty = 0.001; 
        Double_t ymax_Empty = 200.; 
        if(DataSample == "BG"){
		xmin_Empty = 50.;
		ymin_Empty = 0.01;
		ymax_Empty = 100000.;
	}
        int i_min = 2;
        if(DataSample == "BG"){
           i_min = 1;
           hHiggsPt[0] -> SetLineColor(9);    
           hHiggsPt[0] -> SetLineStyle(1);    
           hHiggsPt[0] -> SetLineWidth(3);  
	   hHiggsPt[0] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPt[0] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPt[0] -> SetTitle("p_{T}(#gamma#gamma) for Background");  
           hHiggsPt[0] -> GetXaxis() -> SetTitleOffset(1.25);  
           hHiggsPt[0] -> Draw();  
           hHiggsPtCut[0] -> SetLineColor(9);    
           hHiggsPtCut[0] -> SetLineStyle(1);  
           hHiggsPtCut[0] -> SetLineWidth(3);  
	   hHiggsPtCut[0] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPtCut[0] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPtCut[0] -> Draw("samehisto");
	}
        else{
           hHiggsPt[1] -> SetLineColor(1);
           hHiggsPt[1] -> SetLineStyle(1);
           hHiggsPt[1] -> SetLineWidth(3);
	   hHiggsPt[1] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPt[1] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPt[1] -> SetTitle(Form("p_{T}(#gamma#gamma) for Higgs m =%3.1f", massHiggs));  
           hHiggsPt[1] -> GetXaxis() -> SetTitleOffset(1.25);  
           hHiggsPt[1] -> Draw();  
           hHiggsPtCut[1] -> SetLineColor(1);    
           hHiggsPtCut[1] -> SetLineStyle(1);  
           hHiggsPtCut[1] -> SetLineWidth(3);  
	   hHiggsPtCut[1] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPtCut[1] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPtCut[1] -> Draw("samehisto");
	}
        for (int i = i_min; i < NLumi; i++) // draw and fit histograms
        {
           hHiggsPt[i] -> SetLineColor(i);   
           hHiggsPt[i] -> SetLineStyle(i);   
           hHiggsPt[i] -> SetLineWidth(3);   
	   hHiggsPt[i] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPt[i] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPt[i] -> Draw("same");   
           hHiggsPtCut[i] -> SetLineColor(i);   
           hHiggsPtCut[i] -> SetLineStyle(1);   
           hHiggsPtCut[i] -> SetLineWidth(3);   
	   hHiggsPtCut[i] -> GetXaxis()->SetRangeUser(xmin_Empty,xmax_Empty);
	   hHiggsPtCut[i] -> GetYaxis()->SetRangeUser(ymin_Empty,ymax_Empty);
           hHiggsPtCut[i] -> Draw("samehisto");   
	}
        if(DataSample == "Signal")Dist_pTgg -> Print(Form("Plots/mH%3.0f_Dist_pTbinMerge.png",massHiggs));
        if(DataSample == "BG")Dist_pTgg -> Print("Plots/BG_Dist_pTbinMerge.png");

        outFile->Close();

        // Estimate Efficiency:
        for (int i = 0; i < NpTbins; i++) // draw and fit histograms
        {
	        if (NHiggsGen_2g[i]< 0.00000001) {
			Eff_HiggsReco[i] = 0; Eff_HiggsReco_FatJet[i] = 0; Eff_HiggsReco_Jet[i] = 0; Eff_HiggsReco_Total[i] = 0; Eff_HiggsGen_2gCut[i] = 0;}
		else {
			Eff_HiggsReco[i] = NHiggsReco[i]/NHiggsGen_2g[i];
			if(NHiggsReco_FatJet[i] < 0) NHiggsReco_FatJet[i] = 0; //reset to 0 is subtracted background was too big
			Eff_HiggsReco_FatJet[i] = NHiggsReco_FatJet[i]/NHiggsGen_2g[i];
			if(NHiggsReco_Jet[i] < 0) NHiggsReco_Jet[i] = 0; //reset to 0 is subtracted background was too big
			Eff_HiggsReco_Jet[i] = NHiggsReco_Jet[i]/NHiggsGen_2g[i];
			Eff_HiggsReco_Total[i] = Eff_HiggsReco[i] + Eff_HiggsReco_FatJet[i] + Eff_HiggsReco_Jet[i];
			Eff_HiggsGen_2gCut[i] = NHiggsGen_2gCut[i]/NHiggsGen_2g[i];
		}
		cout << "i = " << i << " Eff_HiggsGen_2gCut[i] = " << Eff_HiggsGen_2gCut[i] << " Eff_HiggsReco[i] = " << Eff_HiggsReco[i] << endl;
		cout << "i = " << i << " Eff_HiggsReco_FatJet[i] = " << Eff_HiggsReco_FatJet[i] << " Eff_HiggsReco_Jet[i] = " << Eff_HiggsReco_Jet[i] << endl;
		cout << "i = " << i << " Eff_HiggsReco_Total[i] = " << Eff_HiggsReco_Total[i] << endl;  
		cout << "i = " << i << " NHiggsGen_2g[i] = " << NHiggsGen_2g[i] << " NHiggsGen_2gCut[i] = " << NHiggsGen_2gCut[i] << " NHiggsReco[i] = " << NHiggsReco[i] << endl;
		cout << "i = " << i << " NHiggsReco_FatJet[i] = " << NHiggsReco_FatJet[i] << " NHiggsReco_Jet[i] = " << NHiggsReco_Jet[i] << endl;
		cout << "i = " << i << " NHiggsReco_Total[i] = " << (Eff_HiggsReco[i] + Eff_HiggsReco_FatJet[i] + Eff_HiggsReco_Jet[i]) << endl;  
	}

	TGraph* gr_Eff_HiggsGen_2gCut = new TGraph(NpTbins,pTbinCorCenter,Eff_HiggsGen_2gCut);
	gr_Eff_HiggsGen_2gCut->SetMarkerStyle(22);
	gr_Eff_HiggsGen_2gCut->SetMarkerSize(3);
	gr_Eff_HiggsGen_2gCut->SetMarkerColor(kRed);

	TGraph* gr_Eff_HiggsReco = new TGraph(NpTbins,pTbinCorCenter,Eff_HiggsReco);
	gr_Eff_HiggsReco->SetMarkerStyle(23);
	gr_Eff_HiggsReco->SetMarkerSize(3);
	gr_Eff_HiggsReco->SetMarkerColor(kBlue);

        TGraph* gr_Eff_HiggsReco_FatJet = new TGraph(NpTbins,pTbinCorCenter,Eff_HiggsReco_FatJet);
        gr_Eff_HiggsReco_FatJet->SetMarkerStyle(20);
        gr_Eff_HiggsReco_FatJet->SetMarkerSize(3);
        gr_Eff_HiggsReco_FatJet->SetMarkerColor(kGreen+2);

        TGraph* gr_Eff_HiggsReco_Jet = new TGraph(NpTbins,pTbinCorCenter,Eff_HiggsReco_Jet);
        gr_Eff_HiggsReco_Jet->SetMarkerStyle(21);
        gr_Eff_HiggsReco_Jet->SetMarkerSize(3);
        gr_Eff_HiggsReco_Jet->SetMarkerColor(kMagenta+2);

        TGraph* gr_Eff_HiggsReco_Total = new TGraph(NpTbins,pTbinCorCenter,Eff_HiggsReco_Total);
        gr_Eff_HiggsReco_Total->SetMarkerStyle(22);
        gr_Eff_HiggsReco_Total->SetMarkerSize(3);
        gr_Eff_HiggsReco_Total->SetMarkerColor(kBlack);


	TH2F *hEmpty = new TH2F("hEmpty","",170,0.,850.,100,0.,1.8);
	hEmpty->GetXaxis()->SetTitle(" Higgs p_{T} (GeV)");
	hEmpty->GetYaxis()->SetTitle("Eff");
	hEmpty->SetStats(0);

	TCanvas *Can_Eff;
        Can_Eff = new TCanvas();
        Can_Eff -> Divide(1,1);
        Can_Eff -> cd(1);
	hEmpty->Draw();
	gr_Eff_HiggsGen_2gCut->Draw("P");
	gr_Eff_HiggsReco->Draw("P");
	gr_Eff_HiggsReco_Jet->Draw("P");
	gr_Eff_HiggsReco_FatJet->Draw("P");
	gr_Eff_HiggsReco_Total->Draw("P");

        TLegend* legData = new TLegend(0.15, 0.70, 0.45, 0.9, "");
        //TLegend* legData = new TLegend(0.25, 0.70, 0.55, 0.9, "");
        legData->SetTextFont(42);
        legData->SetTextSize(0.03);
        //legData->SetFillColor(kWhite);
        //legData->SetTextColor(kBlack);
        legData->AddEntry(gr_Eff_HiggsGen_2gCut,"Gen kin. cuts","P");
        legData->AddEntry(gr_Eff_HiggsReco,"Reco #gamma#gamma","P");
        legData->AddEntry(gr_Eff_HiggsReco_FatJet,"Reco Fat Jet","P");
        legData->AddEntry(gr_Eff_HiggsReco_Jet,"Reco Jet","P");
        legData->AddEntry(gr_Eff_HiggsReco_Total,"Reco Combined","P");
	legData->Draw("same");

	Can_Eff->Print(Form("Plots/mH%3.0f_Eff.png",massHiggs));

}//end Loop()
int HiggsAnalysis_14TeV::BinNumber(Int_t ID_pT, int *pTbinCor, Int_t NpTbins, Float_t pT, TString DataSample)
{
        int indicator_test = -1;
        int indicator = -1;
        //cout << "TEST at BinNumber: NpTbins == " << NpTbins << endl;

	for ( int j = 0; j < NpTbins; j++)
        {
        	if (j < (NpTbins -1)){if (pT >= pTbinCor[j] && pT < pTbinCor[j+1]) indicator = j;}
        	else {if (pT >= pTbinCor[j]) indicator = j;}
	}
        if( (ID_pT == 1 && (indicator == 0 || indicator == 1)) || (ID_pT > 1 && ID_pT == indicator) )indicator_test = indicator;
        else indicator_test = -1;

	if(DataSample == "BG"){
		if(ID_pT == indicator) indicator_test = indicator;
		else indicator_test = -1;
	}

        return indicator_test;
}
Double_t HiggsAnalysis_14TeV::DeltaR(Float_t eta, Float_t phi, TLorentzVector p1)
{
	Double_t dR = -10;
	Double_t deta = eta - p1.Eta();
	Double_t dphi = phi - p1.Phi();
        if (dphi > TMath::Pi()) dphi = dphi - 2*TMath::Pi();
        if (dphi < -TMath::Pi()) dphi = dphi + 2*TMath::Pi();
	dR = sqrt(deta*deta + dphi*dphi);
	return dR;
}
Double_t HiggsAnalysis_14TeV::DeltaR_F(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2)
{
        Double_t dR = -10;
        Double_t deta = eta1 - eta2;
        Double_t dphi = phi1 - phi2;
        if (dphi > TMath::Pi()) dphi = dphi - 2*TMath::Pi();
        if (dphi < -TMath::Pi()) dphi = dphi + 2*TMath::Pi();
        dR = sqrt(deta*deta + dphi*dphi);
        //dR = sqrt(deta*deta);
        //dR = sqrt(dphi*dphi);
        return dR;
}
