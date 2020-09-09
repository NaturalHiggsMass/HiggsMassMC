{
#include <TCanvas.h>

TFile *outFile = new TFile ("HiggsMass_Combine_ChainBins.root", "READ");
TString NamePt[] = {"PT_0_120", "PT_120_200","PT_200_270","PT_270_350","PT_350_450","PT_450_550","PT_550_INF"};
Double_t ScaleSig[] = {1.14,     1.17,         1.9,        1.20,        1.22,        1.25,        1.29};
//int Nstring = int(sizeof(Name)/sizeof(Name[0]));
int Nstring = int(sizeof(NamePt)/sizeof(NamePt[0]));
TH1D * h1;
TH1D * h2;
TH1D * h3;
TCanvas *first[7];
gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
for (int i = 0; i < Nstring; i++)
{
  h1 = (TH1D*)outFile->Get(NamePt[i]+"/signal_sigma1p2");
  Double_t Integral_sig = h1->Integral()*ScaleSig[i]*10.;
  h2 = (TH1D*)outFile->Get(NamePt[i]+"/background");
  Double_t Integral_bg = h2->Integral(201,300)*10.;
  h3 = (TH1D*)outFile->Get(NamePt[i]+"/data_obs");

  cout << "pT bin = " << NamePt[i] << " For 3000 1/fb: #Signal = " << Integral_sig << " #Background = " << Integral_bg << " All bkg = " << h2->Integral()*10. << endl;

  first[i] = new TCanvas();
  first[i] -> Divide(1,1);
  first[i] -> cd(1);
  h3-> GetYaxis()->SetTitle("Events/(0.1 GeV)");
  h3->GetFunction("FitFuncBG")->SetBit(TF1::kNotDraw);
  h3->Draw("e");
  first[i] -> Print("pic_"+NamePt[i]+".png");  
}

outFile->Close();


}
