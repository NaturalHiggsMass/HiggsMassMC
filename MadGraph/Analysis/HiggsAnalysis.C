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

        Double_t massHiggs = 125.;
        //Double_t massHiggs = 123.;
        //Double_t massHiggs = 127.;

        // initialize histograms

        // for Signal
        int pTbin[]      = {0, 80, 120, 200, 270, 350, 450, 550, 750};// keep only for file definition
        int pTbinCor[]   = {0, 80, 120, 200, 270, 350, 450, 550, 750};// keep the same like definition
        // for Background 
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
        std::string titleCard[NpTbins];
        std::string titleCard_SigmaUp[NpTbins];
        std::string titleCard_SigmaDo[NpTbins];
        for (int i = 0; i < NpTbins; i++)
        {
           if (i < (NpTbins-1) ){
		title[i] = Form("p_{T}: %d-%d GeV", pTbinCor[i], pTbinCor[i+1]);
		if(DataSample == "BG")titleCard[i] = Form("BACKGROUND_PT_%d_%d",pTbinCor[i], pTbinCor[i+1]);
		if(DataSample == "Signal"){
			titleCard[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p2_Mass",pTbinCor[i], pTbinCor[i+1]);
			titleCard_SigmaUp[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p4_Mass",pTbinCor[i], pTbinCor[i+1]);
			titleCard_SigmaDo[i] = Form("SIGNAL_PT_%d_%d_SIGMA1p0_Mass",pTbinCor[i], pTbinCor[i+1]);
		}
	   }	
           else {
		title[i] = Form("p_{T}: %d-inf GeV", pTbinCor[i]);
		if(DataSample == "BG")titleCard[i] = Form("BACKGROUND_PT_%d_INF",pTbinCor[i]);
		if(DataSample == "Signal"){
			titleCard[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p2_MASS",pTbinCor[i]);
			titleCard_SigmaUp[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p4_MASS",pTbinCor[i]);
			titleCard_SigmaDo[i] = Form("SIGNAL_PT_%d_INF_SIGMA1p0_MASS",pTbinCor[i]);
		}
	   }
	   if(DataSample == "Signal" && (massHiggs >122.9 && massHiggs < 123.1)){
		titleCard[i] = titleCard[i]+"Do";
		titleCard_SigmaUp[i] = titleCard_SigmaUp[i]+"Do";
		titleCard_SigmaDo[i] = titleCard_SigmaDo[i]+"Do";
	   }
	   if(DataSample == "Signal" && (massHiggs >126.9 && massHiggs < 127.1)){
		titleCard[i] = titleCard[i]+"Up";
		titleCard_SigmaUp[i] = titleCard_SigmaUp[i]+"Up";
		titleCard_SigmaDo[i] = titleCard_SigmaDo[i]+"Up";
	   }
           cout << "title " << i << " = " << title[i] << endl;
        }

        TH1D * bothBarrel[NpTbins];
        TH1D * barrelAndEndCaps[NpTbins];
        TH1D * bothEndCaps[NpTbins];
        TH1D * massBBandBE[NpTbins];
        TH1D * massSigma1p2[NpTbins];
        TH1D * massSigma1p0[NpTbins];
        TH1D * massSigma1p4[NpTbins];
        TH1D * hHiggsPt[NpTbins];
        TH1D * hHiggsPtCut[NpTbins];
        TH1D * hLeadingJet_pT[NpTbins];
        TH1D * hLeadingJet_eta[NpTbins];
        TH1D * hHiggsMass;
        hHiggsMass = new TH1D("hHiggsMass","hHiggsMass", 400, 0., 150.);

        std::string work;
        std::string workName;

        Int_t nbin_mass = 60;
	Double_t xmin_mass = 110.;
	Double_t xmax_mass = 140.;
	//if(DataSample == "BG"){
	//	nbin_mass = 300; xmin_mass = 50.; xmax_mass = 200.;
	//}
        for (int i = 0; i < NpTbins; i++)
        {

                work = "M(#gamma#gamma), both barrel (|#eta_{#gamma}| < 1.44)" + title[i];
                workName = "hMgg_bothBarrel " + title[i];
                bothBarrel[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                TString TitleX = "M(#gamma#gamma) (GeV)";
                TString TitleY = Form("Events/(%1.1f GeV)", bothBarrel[0]->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
                bothBarrel[i]-> GetXaxis()->SetTitle(TitleX);
                bothBarrel[i]-> GetYaxis()->SetTitle(TitleY);
                bothBarrel[i]-> Sumw2();

                work = "M(#gamma#gamma), barrel (|#eta_{#gamma}| < 1.44) - endcap (1.57 < |#eta_{#gamma}| < 2.5) " + title[i];
                workName = "hMgg_BarrelEndcap " + title[i];
                barrelAndEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                barrelAndEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                barrelAndEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                barrelAndEndCaps[i]-> Sumw2(); 

                work = "M(#gamma#gamma), both endcap (1.57 < |#eta_{#gamma}| < 2.5)" + title[i];
                workName = "hMgg_bothEndcap " + title[i];
                bothEndCaps[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                bothEndCaps[i]-> GetXaxis()->SetTitle(TitleX);
                bothEndCaps[i]-> GetYaxis()->SetTitle(TitleY);
                bothEndCaps[i]-> Sumw2();

                work = "M(#gamma#gamma), BB and BE " + title[i];
		//workName = "hMgg_BBandBE " + title[i];
                workName = "ORIGINAL_"+titleCard[i];
                massBBandBE[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                massBBandBE[i]-> GetXaxis()->SetTitle(TitleX);
                massBBandBE[i]-> GetYaxis()->SetTitle(TitleY);
                massBBandBE[i]-> Sumw2();

                work = "M(#gamma#gamma), SIGMA1p2 " + title[i];
                workName = titleCard[i];
                massSigma1p2[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                massSigma1p2[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p2[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p2[i]-> Sumw2();

                work = "M(#gamma#gamma), SIGMA1p4 " + title[i];
                workName = titleCard_SigmaUp[i];
                massSigma1p4[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                massSigma1p4[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p4[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p4[i]-> Sumw2();

                work = "M(#gamma#gamma), SIGMA1p0 " + title[i];
                workName = titleCard_SigmaDo[i];
                massSigma1p0[i] = new TH1D(workName.c_str(),work.c_str(), nbin_mass, xmin_mass, xmax_mass);
                massSigma1p0[i]-> GetXaxis()->SetTitle(TitleX);
                massSigma1p0[i]-> GetYaxis()->SetTitle(TitleY);
                massSigma1p0[i]-> Sumw2();

                work = "p_{T}(#gamma#gamma), " + title[i];
                workName = "hpTgg " + title[i];
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

                work = "p_{T}(#gamma#gamma) Cut" + title[i];
                workName = "hpTggCut " + title[i];
                hHiggsPtCut[i] = new TH1D(workName.c_str(),work.c_str(), nbin_pT, 0., 1000.);
                hHiggsPtCut[i]-> GetXaxis()->SetTitle(TitleX);
                hHiggsPtCut[i]-> GetYaxis()->SetTitle(TitleY);
                hHiggsPtCut[i]-> Sumw2();

                work = "p_{T}(leading jet)" + title[i];
                workName = "hLeadingJet_pT " + title[i];
                hLeadingJet_pT[i] = new TH1D(workName.c_str(),work.c_str(), nbin_pT, 0., 1000.);
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
			hLeadingJet_pT[ID_pT] -> Fill(JetLeading_pT, weight_F);
			hLeadingJet_eta[ID_pT] -> Fill(JetLeading_eta, weight_F);
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
                                if(DataSample == "Signal"){
                    	                if(ID_pT == 1 && (indicatorReco == 0 || indicatorReco==1))bothBarrel[indicatorReco] -> Fill(mass,weight_F);
                        	        if(ID_pT > 1 && ID_pT == indicatorReco)bothBarrel[indicatorReco] -> Fill(mass,weight_F);

				}
				else if(ID_pT == indicatorReco)bothBarrel[indicatorReco] -> Fill(mass,weight_F);
                        }
                        else if (flag == 2)
                        {
                                if(DataSample == "Signal"){
                           	        if(ID_pT == 1 && (indicatorReco == 0 || indicatorReco==1))barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
                                	if(ID_pT > 1 && ID_pT == indicatorReco)barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
				}
				else if(ID_pT == indicatorReco)barrelAndEndCaps[indicatorReco] -> Fill(mass,weight_F);
                        }
                        else if (flag == 3)
                        {
                                if(DataSample == "Signal"){
                         		  if(ID_pT == 1 && (indicatorReco == 0 || indicatorReco==1))bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
                               		 if(ID_pT > 1 && ID_pT == indicatorReco)bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
				}
				else if(ID_pT == indicatorReco)bothEndCaps[indicatorReco] -> Fill(mass,weight_F);
                        }
			// fill pT gamma-gamma bins:
                        Ncheck ++;
			if(DataSample == "Signal"){
	                        if(ID_pT == 1 && (indicatorReco == 0 || indicatorReco==1))
				{
					if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
					else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
				}
        	                if(ID_pT > 1 && ID_pT == indicatorReco)
				{
					if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
                                	else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
				}
			}
                        else if (ID_pT == indicatorReco){
					if (pTHiggs < 1000.)hHiggsPtCut[indicatorReco] -> Fill (pTHiggs,weight_F);
                                	else hHiggsPtCut[indicatorReco] -> Fill (1000.,weight_F);
			
			}
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
				std::cout << "Selected events = " << Ncheck << " #File = " << FileNumber << " weight_F = " << weight_F << " indicatorReco = " << indicatorReco << " pT(gg) = " << pTHiggs << std::endl;
			}
                } //end check

        } // end cyclbe by nentries

        TFile *outFile = new TFile ( "SignalSummary.root", "RECREATE");
        if(DataSample == "BG")outFile = new TFile ( "BackgroundSummary.root", "RECREATE");
	gStyle->SetCanvasDefW(2000);
	gStyle->SetCanvasDefH(2000);
        gStyle->SetPadLeftMargin(0.13);
        TCanvas *first[NpTbins];

        for (int i = 0; i < NpTbins; i++) // draw and fit histograms
        {
                Double_t func_xmin = massHiggs - 7.; 
                Double_t func_xmax = massHiggs + 7.; 
                TF1 *FitFunc = new TF1("FitFunc","[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])",func_xmin,func_xmax); 
                FitFunc->SetParameter(0, 120./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(2, 1.5); //n
                FitFunc->SetParameter(3, 1.5); //sigma
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
                FitFuncBG->SetParName(0, "p0");
                FitFuncBG->SetParName(1, "p1");

                first[i] = new TCanvas();
                first[i] -> Divide(2,2);
                gStyle->SetOptFit(1);
                //gStyle->SetOptStat(1); 

                first[i] -> cd(1);
                //first[i] -> SetLeftMargin(0.15);
                bothBarrel[i]->Draw();
                //if(DataSample == "Signal")bothBarrel[i]->Fit("gaus","","",120,130);
                if(DataSample == "Signal")bothBarrel[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")bothBarrel[i]->Fit("FitFuncBG","","",110,140);

                first[i] -> cd(2);
                barrelAndEndCaps[i]->Draw();
                //if(DataSample == "Signal")barrelAndEndCaps[i]->Fit("gaus","","",120,130);
                FitFunc->SetParameter(0, 60./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(3, 1.5); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                if(DataSample == "Signal")barrelAndEndCaps[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")barrelAndEndCaps[i]->Fit("FitFuncBG","","",110,140);

                //first[i] -> cd(3);
                //bothEndCaps[i]->Draw();
                ////if(DataSample == "Signal")bothEndCaps[i]->Fit("gaus","","",120,130);
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
                FitFunc->SetParameter(3, 1.5); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                if(DataSample == "Signal")massBBandBE[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")massBBandBE[i]->Fit("FitFuncBG","","",110,140);

        	Int_t nbin_mass = 60;
        	Double_t xmin_mass = 110.;              
        	Double_t xmax_mass = 140.; 
        	for (int j = 1; j <= nbin_mass; j++){
                        Double_t binWidth = (xmax_mass-xmin_mass)/nbin_mass;
			Double_t binValue = FitFunc->Integral(xmin_mass+(j-1)*binWidth, xmin_mass+j*binWidth);
			if(DataSample == "BG")binValue = FitFuncBG->Integral(xmin_mass+(j-1)*binWidth, xmin_mass+j*binWidth);
                        massSigma1p2[i]->SetBinContent(j,binValue/binWidth);
                        massSigma1p2[i]->SetBinError(j,0.01*binValue/binWidth);
		}
                first[i] -> cd(4);
                massSigma1p2[i]->Draw();
                FitFunc->SetParameter(0, 200./pow(3.,i)); //A
                FitFunc->SetParameter(1, -3.); //alpha
                FitFunc->SetParameter(3, 1.5); //sigma
                FitFunc->SetParameter(4, massHiggs); //Higgs mass
                if(DataSample == "Signal")massSigma1p2[i]->Fit("FitFunc","RB");
                if(DataSample == "BG")massSigma1p2[i]->Fit("FitFuncBG","","",110,140);

                //bothBarrel[i]->Write();
                massBBandBE[i]->Write();
                massSigma1p2[i]->Write();
                //barrelAndEndCaps[i]->Write();
                //bothEndCaps[i]->Write();
                //hHiggsPt[i]->Write();
                //hHiggsPtCut[i]->Write();
                //hLeadingJet_pT[i]->Write();
                //hLeadingJet_eta[i]->Write();
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
}//end Loop()
