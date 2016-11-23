//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Nov 22 09:30:10 2016 by ROOT version 6.06/09
// from TTree tree/Hcal SiPM ntuple
// found on file: step2hpdprec10K_HcalHPD2ntuple.root
//////////////////////////////////////////////////////////

#ifndef HPD2Class_h
#define HPD2Class_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

class HPD2Class {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   UInt_t          id;
   Int_t           subdet;
   Int_t           ieta;
   Int_t           iphi;
   Int_t           depth;
   vector<double>  *energy;
   vector<int>     *photons;
   vector<double>  *time;
   vector<double>  *tof;
   vector<double>  *tzero;
   vector<double>  *signalTot;
   vector<double>  *signalTotPrecise;

   // List of branches
   TBranch        *b_nevent;   //!
   TBranch        *b_id;   //!
   TBranch        *b_subdet;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_depth;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_photons;   //!
   TBranch        *b_time;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_tzero;   //!
   TBranch        *b_signalTot;   //!
   TBranch        *b_signalTotPrecise;   //!

   HPD2Class(TTree *tree=0);
   virtual ~HPD2Class();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef HPD2Class_cxx
HPD2Class::HPD2Class(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("step2hpdprec10K_HcalHPD2ntuple.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("step2hpdprec10K_HcalHPD2ntuple.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

HPD2Class::~HPD2Class()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HPD2Class::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HPD2Class::LoadTree(Long64_t entry)
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

void HPD2Class::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   energy = 0;
   photons = 0;
   time = 0;
   tof = 0;
   tzero = 0;
   signalTot = 0;
   signalTotPrecise = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_nevent);
   fChain->SetBranchAddress("id", &id, &b_id);
   fChain->SetBranchAddress("subdet", &subdet, &b_subdet);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("depth", &depth, &b_depth);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("photons", &photons, &b_photons);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("tof", &tof, &b_tof);
   fChain->SetBranchAddress("tzero", &tzero, &b_tzero);
   fChain->SetBranchAddress("signalTot", &signalTot, &b_signalTot);
   fChain->SetBranchAddress("signalTotPrecise", &signalTotPrecise, &b_signalTotPrecise);
   Notify();
}

Bool_t HPD2Class::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HPD2Class::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HPD2Class::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef HPD2Class_cxx
