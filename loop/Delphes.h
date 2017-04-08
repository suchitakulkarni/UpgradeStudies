//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr  6 22:57:00 2017 by ROOT version 6.06/06
// from TTree Delphes/Analysis tree
// found on file: /home/gzevi/UpgradeSkims/BB-4p-300-700-v1510_14TEV_200PU_PhaseII_Substructure_PIX4022_1.root.root
//////////////////////////////////////////////////////////

#ifndef Delphes_h
#define Delphes_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"

class Delphes {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static const Int_t kMaxParticle = 105;
   static const Int_t kMaxElectron = 6;
   static const Int_t kMaxMuonTight = 6;
   static const Int_t kMaxJetPUPPI = 18;
   static const Int_t kMaxPuppiMissingET = 1;
   static const Int_t kMaxScalarHT = 1;

   // Declaration of leaf types
   Int_t           Particle_;
   Int_t           Particle_PID[kMaxParticle];   //[Particle_]
   Int_t           Particle_M1[kMaxParticle];   //[Particle_]
   Int_t           Particle_M2[kMaxParticle];   //[Particle_]
   Float_t         Particle_PT[kMaxParticle];   //[Particle_]
   Float_t         Particle_Eta[kMaxParticle];   //[Particle_]
   Float_t         Particle_Phi[kMaxParticle];   //[Particle_]
   Int_t           Electron_;
   Float_t         Electron_PT[kMaxElectron];   //[Electron_]
   Float_t         Electron_Eta[kMaxElectron];   //[Electron_]
   Float_t         Electron_Phi[kMaxElectron];   //[Electron_]
   Int_t           Electron_Charge[kMaxElectron];   //[Electron_]
   Float_t         Electron_IsolationVarRhoCorr[kMaxElectron];   //[Electron_]
   Int_t           MuonTight_;
   Float_t         MuonTight_PT[kMaxMuonTight];   //[MuonTight_]
   Float_t         MuonTight_Eta[kMaxMuonTight];   //[MuonTight_]
   Float_t         MuonTight_Phi[kMaxMuonTight];   //[MuonTight_]
   Int_t           MuonTight_Charge[kMaxMuonTight];   //[MuonTight_]
   Float_t         MuonTight_IsolationVarRhoCorr[kMaxMuonTight];   //[MuonTight_]
   Int_t           JetPUPPI_;
   Float_t         JetPUPPI_PT[kMaxJetPUPPI];   //[JetPUPPI_]
   Float_t         JetPUPPI_Eta[kMaxJetPUPPI];   //[JetPUPPI_]
   Float_t         JetPUPPI_Phi[kMaxJetPUPPI];   //[JetPUPPI_]
   UInt_t          JetPUPPI_BTag[kMaxJetPUPPI];   //[JetPUPPI_]
   UInt_t          JetPUPPI_BTagAlgo[kMaxJetPUPPI];   //[JetPUPPI_]
   UInt_t          JetPUPPI_BTagPhys[kMaxJetPUPPI];   //[JetPUPPI_]
   Int_t           PuppiMissingET_;
   Float_t         PuppiMissingET_MET[kMaxPuppiMissingET];   //[PuppiMissingET_]
   Float_t         PuppiMissingET_Phi[kMaxPuppiMissingET];   //[PuppiMissingET_]
   Int_t           ScalarHT_;
   Float_t         ScalarHT_HT[kMaxScalarHT];   //[ScalarHT_]

   // List of branches
   TBranch        *b_Particle_;   //!
   TBranch        *b_Particle_PID;   //!
   TBranch        *b_Particle_M1;   //!
   TBranch        *b_Particle_M2;   //!
   TBranch        *b_Particle_PT;   //!
   TBranch        *b_Particle_Eta;   //!
   TBranch        *b_Particle_Phi;   //!
   TBranch        *b_Electron_;   //!
   TBranch        *b_Electron_PT;   //!
   TBranch        *b_Electron_Eta;   //!
   TBranch        *b_Electron_Phi;   //!
   TBranch        *b_Electron_Charge;   //!
   TBranch        *b_Electron_IsolationVarRhoCorr;   //!
   TBranch        *b_MuonTight_;   //!
   TBranch        *b_MuonTight_PT;   //!
   TBranch        *b_MuonTight_Eta;   //!
   TBranch        *b_MuonTight_Phi;   //!
   TBranch        *b_MuonTight_Charge;   //!
   TBranch        *b_MuonTight_IsolationVarRhoCorr;   //!
   TBranch        *b_JetPUPPI_;   //!
   TBranch        *b_JetPUPPI_PT;   //!
   TBranch        *b_JetPUPPI_Eta;   //!
   TBranch        *b_JetPUPPI_Phi;   //!
   TBranch        *b_JetPUPPI_BTag;   //!
   TBranch        *b_JetPUPPI_BTagAlgo;   //!
   TBranch        *b_JetPUPPI_BTagPhys;   //!
   TBranch        *b_PuppiMissingET_;   //!
   TBranch        *b_PuppiMissingET_MET;   //!
   TBranch        *b_PuppiMissingET_Phi;   //!
   TBranch        *b_ScalarHT_;   //!
   TBranch        *b_ScalarHT_HT;   //!

   Delphes(TTree *tree=0);
   virtual ~Delphes();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Delphes_cxx
Delphes::Delphes(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/gzevi/UpgradeSkims/BB-4p-300-700-v1510_14TEV_200PU_PhaseII_Substructure_PIX4022_1.root.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/gzevi/UpgradeSkims/BB-4p-300-700-v1510_14TEV_200PU_PhaseII_Substructure_PIX4022_1.root.root");
      }
      f->GetObject("Delphes",tree);

   }
   Init(tree);
}

Delphes::~Delphes()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Delphes::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Delphes::LoadTree(Long64_t entry)
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

void Delphes::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Particle", &Particle_, &b_Particle_);
   fChain->SetBranchAddress("Particle.PID", Particle_PID, &b_Particle_PID);
   fChain->SetBranchAddress("Particle.M1", Particle_M1, &b_Particle_M1);
   fChain->SetBranchAddress("Particle.M2", Particle_M2, &b_Particle_M2);
   fChain->SetBranchAddress("Particle.PT", Particle_PT, &b_Particle_PT);
   fChain->SetBranchAddress("Particle.Eta", Particle_Eta, &b_Particle_Eta);
   fChain->SetBranchAddress("Particle.Phi", Particle_Phi, &b_Particle_Phi);
   fChain->SetBranchAddress("Electron", &Electron_, &b_Electron_);
   fChain->SetBranchAddress("Electron.PT", Electron_PT, &b_Electron_PT);
   fChain->SetBranchAddress("Electron.Eta", Electron_Eta, &b_Electron_Eta);
   fChain->SetBranchAddress("Electron.Phi", Electron_Phi, &b_Electron_Phi);
   fChain->SetBranchAddress("Electron.Charge", Electron_Charge, &b_Electron_Charge);
   fChain->SetBranchAddress("Electron.IsolationVarRhoCorr", Electron_IsolationVarRhoCorr, &b_Electron_IsolationVarRhoCorr);
   fChain->SetBranchAddress("MuonTight", &MuonTight_, &b_MuonTight_);
   fChain->SetBranchAddress("MuonTight.PT", MuonTight_PT, &b_MuonTight_PT);
   fChain->SetBranchAddress("MuonTight.Eta", MuonTight_Eta, &b_MuonTight_Eta);
   fChain->SetBranchAddress("MuonTight.Phi", MuonTight_Phi, &b_MuonTight_Phi);
   fChain->SetBranchAddress("MuonTight.Charge", MuonTight_Charge, &b_MuonTight_Charge);
   fChain->SetBranchAddress("MuonTight.IsolationVarRhoCorr", MuonTight_IsolationVarRhoCorr, &b_MuonTight_IsolationVarRhoCorr);
   fChain->SetBranchAddress("JetPUPPI", &JetPUPPI_, &b_JetPUPPI_);
   fChain->SetBranchAddress("JetPUPPI.PT", JetPUPPI_PT, &b_JetPUPPI_PT);
   fChain->SetBranchAddress("JetPUPPI.Eta", JetPUPPI_Eta, &b_JetPUPPI_Eta);
   fChain->SetBranchAddress("JetPUPPI.Phi", JetPUPPI_Phi, &b_JetPUPPI_Phi);
   fChain->SetBranchAddress("JetPUPPI.BTag", JetPUPPI_BTag, &b_JetPUPPI_BTag);
   fChain->SetBranchAddress("JetPUPPI.BTagAlgo", JetPUPPI_BTagAlgo, &b_JetPUPPI_BTagAlgo);
   fChain->SetBranchAddress("JetPUPPI.BTagPhys", JetPUPPI_BTagPhys, &b_JetPUPPI_BTagPhys);
   fChain->SetBranchAddress("PuppiMissingET", &PuppiMissingET_, &b_PuppiMissingET_);
   fChain->SetBranchAddress("PuppiMissingET.MET", PuppiMissingET_MET, &b_PuppiMissingET_MET);
   fChain->SetBranchAddress("PuppiMissingET.Phi", PuppiMissingET_Phi, &b_PuppiMissingET_Phi);
   fChain->SetBranchAddress("ScalarHT", &ScalarHT_, &b_ScalarHT_);
   fChain->SetBranchAddress("ScalarHT.HT", ScalarHT_HT, &b_ScalarHT_HT);
   Notify();
}

Bool_t Delphes::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Delphes::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Delphes::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Delphes_cxx
