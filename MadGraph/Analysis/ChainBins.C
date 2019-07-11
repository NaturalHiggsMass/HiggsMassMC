{

TFile *outFile = new TFile ("HiggsMass_CombineTest.root", "UPDATE");
outFile->mkdir("PT_0_120");
outFile->mkdir("PT_550_INF");
//TString Name[] = {"original_background","background","data_obs","original_signal_MassDown", "signal_sigma1p0_MassDown", "signal_sigma1p2_MassDown", "signal_sigma1p4_MassDown", "original_signal","signal_sigma1p0","signal_sigma1p2","signal_sigma1p4","original_signal_MassUp","signal_sigma1p0_MassUp", "signal_sigma1p2_MassUp", "signal_sigma1p4_MassUp"};
TString Name[] = {"original_background","background","data_obs","original_signal_MassDown", "signal_sigma1p0_MassDown", "signal_sigma1p2_MassDown", "signal_sigma1p4_MassDown", "original_signal","signal_sigma1p0","signal_sigma1p2", "signal_sigma1p2_MggScaleUp","signal_sigma1p2_MggScaleDown","signal_sigma1p4","original_signal_MassUp","signal_sigma1p0_MassUp", "signal_sigma1p2_MassUp", "signal_sigma1p4_MassUp"};
int Nstring = int(sizeof(Name)/sizeof(Name[0]));
TH1D * h1;
TH1D * h2;
for (int i = 0; i < Nstring; i++)
{
  h1 = (TH1D*)outFile->Get("PT_0_80/"+Name[i]);
  h2 = (TH1D*)outFile->Get("PT_80_120/"+Name[i]);
  h2->Add(h1);
  outFile->cd("PT_0_120");
  h2->Write (Name[i]);

  h1 = (TH1D*)outFile->Get("PT_550_750/"+Name[i]);
  h2 = (TH1D*)outFile->Get("PT_750_INF/"+Name[i]);
  h2->Add(h1);
  outFile->cd("PT_550_INF");
  h2->Write (Name[i]);

}

outFile->Close();


}
