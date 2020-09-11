{
#include <TCanvas.h>

TFile *outFile = new TFile ("HiggsMass_14TeV_Combine_ChainBins.root", "READ");
TString NamePt[] = {"PT_0_120", "PT_120_200","PT_200_270","PT_270_350","PT_350_450","PT_450_550","PT_550_750", "PT_750_INF"};
TString PtBin[]  = {" < 120",   ": 120-200", ": 200-270", ": 270-350", ": 350-450", ": 450-550", ": 550-750",  " > 750"};
Double_t ScaleSig[] = {1.,       1.,           1.,          1.,         1.,          1.,          1.,          1.};
//int Nstring = int(sizeof(Name)/sizeof(Name[0]));
int Nstring = int(sizeof(NamePt)/sizeof(NamePt[0]));
TH1D * h1;
TH1D * h2;
TH1D * h3;
TCanvas *first[8];
gStyle->SetOptFit(0);
gStyle->SetOptStat(0);
for (int i = 0; i < Nstring; i++)
//for (int i = 1; i < Nstring; i++)
{
  //h1 = (TH1D*)outFile->Get(NamePt[i]+"/signal_sigma1p2");
  h1 = (TH1D*)outFile->Get(NamePt[i]+"/signal_sigma1p0");
  Double_t ScalePar = 10.;
  h1 ->Scale( ScalePar );
  Double_t Integral_sig = h1->Integral()*ScaleSig[i];
  h2 = (TH1D*)outFile->Get(NamePt[i]+"/background");
  h2 ->Scale( ScalePar );
  Double_t Integral_bg = h2->Integral(201,300);
  h3 = (TH1D*)outFile->Get(NamePt[i]+"/data_obs");
  h3 ->Scale( ScalePar );

  cout << "pT bin = " << NamePt[i] << " For 3000 1/fb: #Signal = " << Integral_sig << " #Background = " << Integral_bg << " All bkg = " << h2->Integral() << endl;

  first[i] = new TCanvas();
  first[i] -> Divide(1,1);
  first[i] -> cd(1);
  //h3-> GetYaxis()->SetTitle("Events/(0.1 GeV)");
  TString TitleX = "M(#gamma#gamma) (GeV/c^{2})";
  TString TitleY = Form("Events/(%1.1f GeV/c^{2})", h3->GetXaxis()->GetBinWidth(1)); // could make it only after histo creating
  h3-> GetXaxis()->SetTitle(TitleX);
  h3-> GetYaxis()->SetTitle(TitleY);

  //TString Title = "HL-LHC: p_{T}(#gamma#gamma)"+ PtBin[i] +" GeV/c, 3000 fb^{-1} (14 TeV)";
  TString Title = Form("HL-LHC: p_{T}(#gamma#gamma)%s GeV/c, 3000 fb^{-1} (14 TeV)",PtBin[i].Data());
  h3-> SetTitle(Title);


  //h3->GetFunction("FitFuncBG")->SetBit(TF1::kNotDraw);
  h3->GetFunction("FitFuncGaus")->SetBit(TF1::kNotDraw);
  h3->Draw("e");
  first[i] -> Print("pic_"+NamePt[i]+".png");  
}

outFile->Close();


}
