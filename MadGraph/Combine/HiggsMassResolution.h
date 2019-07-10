//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 10 12:02:56 2019 by ROOT version 6.12/07
// from TChain limit/
//////////////////////////////////////////////////////////

#ifndef HiggsMassResolution_h
#define HiggsMassResolution_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "string"

class HiggsMassResolution {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        limit;
   Double_t        limitErr;
   Double_t        mh;
   Int_t           syst;
   Int_t           iToy;
   Int_t           iSeed;
   Int_t           iChannel;
   Float_t         t_cpu;
   Float_t         t_real;
   Float_t         quantileExpected;
   string          *BIN;
   Float_t         Mass;
   Float_t         deltaNLL;

   // List of branches
   TBranch        *b_limit;   //!
   TBranch        *b_limitErr;   //!
   TBranch        *b_mh;   //!
   TBranch        *b_syst;   //!
   TBranch        *b_iToy;   //!
   TBranch        *b_iSeed;   //!
   TBranch        *b_iChannel;   //!
   TBranch        *b_t_cpu;   //!
   TBranch        *b_t_real;   //!
   TBranch        *b_quantileExpected;   //!
   TBranch        *b_BIN;   //!
   TBranch        *b_Mass;   //!
   TBranch        *b_deltaNLL;   //!

   HiggsMassResolution(TTree *tree=0);
   virtual ~HiggsMassResolution();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef HiggsMassResolution_cxx
HiggsMassResolution::HiggsMassResolution(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("limit",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("limit","");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_0_80.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_80_120.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_120_200.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_200_270.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_270_350.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_350_450.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_450_550.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_550_750.root/limit");
      chain->Add("/afs/cern.ch/work/k/kropiv/HiggsMass/HiggsMassMC/MadGraph/Combine/higgsCombineMassRange.MultiDimFit.mH125.BINPT_750_INF.root/limit");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

HiggsMassResolution::~HiggsMassResolution()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HiggsMassResolution::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HiggsMassResolution::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void HiggsMassResolution::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   BIN = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("limit", &limit, &b_limit);
   fChain->SetBranchAddress("limitErr", &limitErr, &b_limitErr);
   fChain->SetBranchAddress("mh", &mh, &b_mh);
   fChain->SetBranchAddress("syst", &syst, &b_syst);
   fChain->SetBranchAddress("iToy", &iToy, &b_iToy);
   fChain->SetBranchAddress("iSeed", &iSeed, &b_iSeed);
   fChain->SetBranchAddress("iChannel", &iChannel, &b_iChannel);
   fChain->SetBranchAddress("t_cpu", &t_cpu, &b_t_cpu);
   fChain->SetBranchAddress("t_real", &t_real, &b_t_real);
   fChain->SetBranchAddress("quantileExpected", &quantileExpected, &b_quantileExpected);
   fChain->SetBranchAddress("BIN", &BIN, &b_BIN);
   fChain->SetBranchAddress("Mass", &Mass, &b_Mass);
   fChain->SetBranchAddress("deltaNLL", &deltaNLL, &b_deltaNLL);
   Notify();
}

Bool_t HiggsMassResolution::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HiggsMassResolution::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HiggsMassResolution::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef HiggsMassResolution_cxx
