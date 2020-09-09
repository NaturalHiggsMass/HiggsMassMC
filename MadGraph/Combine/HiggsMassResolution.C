#define HiggsMassResolution_cxx
#include "HiggsMassResolution.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TGraph.h"
#include "TLatex.h"
#include "TGraphErrors.h"

void HiggsMassResolution::Loop()
{
//   In a ROOT session, you can do:
//      root> .L HiggsMassResolution.C
//      root> HiggsMassResolution t
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

   //int pTbin[]      = {0, 80, 120, 200, 270, 350, 450, 550, 750};
   int pTbin[]      = {0, 120, 200, 270, 350, 450, 550, 750};
   //int pTbin[]      = {0, 120, 200, 270, 350, 450, 550};
   int NpTbins = int(sizeof(pTbin)/sizeof(pTbin[0]));
   Double_t pTHiggs[NpTbins];
   Double_t dpTHiggs[NpTbins];
   Double_t MassRes[NpTbins];
   Double_t MassResNeg[NpTbins];
   Double_t dMassRes[NpTbins];
   for (int i = 0; i < NpTbins; i++)
   {
	if (i < (NpTbins-1)){
		pTHiggs[i] = (pTbin[i]+pTbin[i+1])/2;
		dpTHiggs[i] = 10;
	}
	if (i == (NpTbins-1)){
		pTHiggs[i] = pTbin[i]+50;
		dpTHiggs[i] = 10;
	}
   }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);   nbytes += nb;
	// if (Cut(ientry) < 0) continue;
        string FileNameCheck = fChain -> GetCurrentFile()->GetName();
        size_t posStart = FileNameCheck.find("BINPT_");      // position of "forPtReco" in str
        //size_t posEnd = FileNameCheck.find(".root");      // position of "GeV.root" in str
        size_t posEnd = FileNameCheck.find(".RES1p2.root");      // position of "GeV.root" in str
        string FileNameCut = FileNameCheck.substr (posStart+6,posEnd-posStart-6);
        cout << "FileNameCut = " << FileNameCut << endl;
        cout << "Mass 68% in GeV = " << Mass*2 << endl;// sigma 2 GeV: 123-125-127

	//find ID_pT <- pT bin
        int ID_pT = -1;
        for (int i = 0; i < NpTbins; i++)
        {
		string pTbinCheck = "";
            if (i < (NpTbins-1)) pTbinCheck = Form("%d_%d", pTbin[i], pTbin[i+1]);
            if (i == (NpTbins-1)) pTbinCheck = Form("%d_INF", pTbin[i]);
            if (pTbinCheck == FileNameCut) ID_pT = i;
	}
        if (ID_pT < 0) cout << "Error: ID_pT < 0 check code" << endl;
        cout << "ID_pT = " << ID_pT << endl;
        if (fabs(Mass) < 0.002) continue;
        if (Mass > 0) MassRes[ID_pT] = Mass*2.; // sigma 2 GeV: 123-125-127 GeV
        if (Mass < 0) MassResNeg[ID_pT] = fabs(Mass)*2.; // sigma 2 GeV: 123-125-127 GeV

   } // end cycle by jentries

   for (int i = 0; i < NpTbins; i++)
   {
	dMassRes[i] = fabs(MassRes[i]-MassResNeg[i]);
   }
   TGraphErrors* grMassRes = new TGraphErrors( NpTbins, pTHiggs, MassRes, dpTHiggs, dMassRes );
   grMassRes->SetMarkerStyle(33);
   grMassRes->SetMarkerSize(2.);
   grMassRes->SetMarkerColor(kBlue);
   //grMassRes->SetMarkerColor(kBlack);

   TCanvas *first = new TCanvas();
   first -> cd(1);
   //gPad -> SetLogx(1);

   //grMassRes -> SetTitle("Higgs mass precision as a function of p_{T}");
   grMassRes -> SetTitle("HL-LHC: 3000 fb^{-1} (14 TeV)");
   grMassRes -> GetXaxis()->SetRangeUser(30.,850.);
   grMassRes -> GetYaxis()->SetRangeUser(0.,1.8);
   grMassRes -> GetXaxis()->SetTitle("Higgs p_{T} [GeV/c]");
   grMassRes -> GetYaxis()->SetTitle("Higgs mass precision [GeV/c^{2}]");
   grMassRes -> Draw("ALP");
   first->Print("MassResolution.png");  
}
