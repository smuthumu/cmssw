//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct  6 14:42:36 2016 by ROOT version 6.06/09
// from TTree tree/Hcal SiPM ntuple
// found on file: step2_HcalSiPMntuple.root
//////////////////////////////////////////////////////////

#ifndef DigiClass_h
#define DigiClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

class DigiClass {
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
   Double_t        fCtoGeV;
   Double_t        samplingFactor;
   Double_t        photoelectronsToAnalog;
   Double_t        simhitToPhotoelectrons;
   vector<double>  *energy;
   vector<int>     *photons;
   vector<double>  *time;
   vector<double>  *tof;
   vector<double>  *tzero;
   vector<double>  *tzero_corrected;
   vector<vector<double> > *t_pe;
   vector<vector<int> > *t_bin;
   vector<double>  *elapsedTime;
   vector<int>     *sampleBin;
   vector<int>     *preciseBin;
   vector<int>     *pe;
   vector<int>     *hitPixels;
   vector<vector<double> > *signal;
   vector<double>  *signalTot;
   Int_t           sumPE;
   Int_t           sumHits;

   // List of branches
   TBranch        *b_nevent;   //!
   TBranch        *b_id;   //!
   TBranch        *b_subdet;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_depth;   //!
   TBranch        *b_fCtoGeV;   //!
   TBranch        *b_samplingFactor;   //!
   TBranch        *b_photoelectronsToAnalog;   //!
   TBranch        *b_simhitToPhotoelectrons;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_photons;   //!
   TBranch        *b_time;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_tzero;   //!
   TBranch        *b_tzero_corrected;   //!
   TBranch        *b_t_pe;   //!
   TBranch        *b_t_bin;   //!
   TBranch        *b_elapsedTime;   //!
   TBranch        *b_sampleBin;   //!
   TBranch        *b_preciseBin;   //!
   TBranch        *b_pe;   //!
   TBranch        *b_hitPixels;   //!
   TBranch        *b_signal;   //!
   TBranch        *b_signalTot;   //!
   TBranch        *b_sumPE;   //!
   TBranch        *b_sumHits;   //!

   DigiClass(TTree *tree=0);
   virtual ~DigiClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DigiClass_cxx
DigiClass::DigiClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("step2_HcalSiPMntuple.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("step2_HcalSiPMntuple.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

DigiClass::~DigiClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DigiClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DigiClass::LoadTree(Long64_t entry)
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

void DigiClass::Init(TTree *tree)
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
   tzero_corrected = 0;
   t_pe = 0;
   t_bin = 0;
   elapsedTime = 0;
   sampleBin = 0;
   preciseBin = 0;
   pe = 0;
   hitPixels = 0;
   signal = 0;
   signalTot = 0;
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
   fChain->SetBranchAddress("fCtoGeV", &fCtoGeV, &b_fCtoGeV);
   fChain->SetBranchAddress("samplingFactor", &samplingFactor, &b_samplingFactor);
   fChain->SetBranchAddress("photoelectronsToAnalog", &photoelectronsToAnalog, &b_photoelectronsToAnalog);
   fChain->SetBranchAddress("simhitToPhotoelectrons", &simhitToPhotoelectrons, &b_simhitToPhotoelectrons);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("photons", &photons, &b_photons);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("tof", &tof, &b_tof);
   fChain->SetBranchAddress("tzero", &tzero, &b_tzero);
   fChain->SetBranchAddress("tzero_corrected", &tzero_corrected, &b_tzero_corrected);
   fChain->SetBranchAddress("t_pe", &t_pe, &b_t_pe);
   fChain->SetBranchAddress("t_bin", &t_bin, &b_t_bin);
   fChain->SetBranchAddress("elapsedTime", &elapsedTime, &b_elapsedTime);
   fChain->SetBranchAddress("sampleBin", &sampleBin, &b_sampleBin);
   fChain->SetBranchAddress("preciseBin", &preciseBin, &b_preciseBin);
   fChain->SetBranchAddress("pe", &pe, &b_pe);
   fChain->SetBranchAddress("hitPixels", &hitPixels, &b_hitPixels);
   fChain->SetBranchAddress("signal", &signal, &b_signal);
   fChain->SetBranchAddress("signalTot", &signalTot, &b_signalTot);
   fChain->SetBranchAddress("sumPE", &sumPE, &b_sumPE);
   fChain->SetBranchAddress("sumHits", &sumHits, &b_sumHits);
   Notify();
}

Bool_t DigiClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DigiClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DigiClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DigiClass_cxx
