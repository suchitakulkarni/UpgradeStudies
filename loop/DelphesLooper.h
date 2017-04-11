#ifndef DELPHESLOOPER_h
#define DELPHESLOOPER_h

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "Math/LorentzVector.h"
#include "TLorentzVector.h"


#include "Delphes.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

struct part {
  TLorentzVector vec;
  int id;
  float mt;
};


class DelphesLooper {
  
 public:
  
//  DelphesLooper();
//  ~DelphesLooper();

  // Work is done here
  void loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt);

  // Other useful functions
  float DeltaR(float eta1, float eta2, float phi1, float phi2);
  float DeltaR(LorentzVector v1, LorentzVector v2); 
  float DeltaPhi(float phi1, float phi2);
  float MT(float pt1, float phi1, float pt2, float phi2);
  void progress( int nEventsTotal, int nEventsChain ); 
  void fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname = "", const std::string& s = "");
  
 private:
  
  TFile * outfile_;
  //Delphes t;
  float evtweight_;

  float HT2p5_;
  float HT_;
  int njet30central_;
  int njet30forward_;
  int njet30_;
  float MET_;
  int nlep_;
  int nlepIso_;
  int BBtype_;
//  float lep1pt_;
//  float lep1eta_;
//  float lep1phi_;
//  float lep2pt_;
//  float lep2eta_;
//  float lep2phi_;
  std::vector<part> leptons_;
  std::vector<part> leptonsVeto_;


  // One histogram map for every directory, plus a global one (h_1d)
  std::map<std::string, TH1*> h_1d;
  std::map<std::string, TH1*> h_1d_base;
  std::map<std::string, TH1*> h_1d_2j;
  std::map<std::string, TH1*> h_1d_WZ;
  std::map<std::string, TH1*> h_1d_ZZ;
  std::map<std::string, TH1*> h_1d_SSWW;
  std::map<std::string, TH1*> h_1d_OSWW;
  std::map<std::string, TH1*> h_1d_lepVeto;
  std::map<std::string, TH1*> h_1d_Zveto;

};

#endif
