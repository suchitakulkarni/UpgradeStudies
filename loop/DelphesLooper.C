#include "DelphesLooper.h"

// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TF1.h"

//#include "Plotting/PlotUtilities.h"
#include "PlotUtilities.h"

// Switches
bool verbose = 0;

// Settings
int lumi = 300; // in fb


void DelphesLooper::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if(nEventsTotal%1000 == 0) {
    if (isatty(1)) {
      if( ( nEventsChain - nEventsTotal ) > period ){
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

float DelphesLooper::DeltaR(float eta1, float eta2, float phi1, float phi2){
  float dEta = eta1 - eta2;
  float dPhi = DeltaPhi(phi1, phi2);
  return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

float DelphesLooper::DeltaR(LorentzVector v1, LorentzVector v2){
  return DeltaR(v1.eta(), v2.eta(), v1.phi(), v2.phi()); 
}

float DelphesLooper::DeltaPhi(float phi1, float phi2){
  float dPhi = phi1 - phi2;
  while (dPhi  >  TMath::Pi()) dPhi -= 2*TMath::Pi();
  while (dPhi <= -TMath::Pi()) dPhi += 2*TMath::Pi();
  return fabs(dPhi);
}

float DelphesLooper::MT(float pt1, float phi1, float pt2, float phi2){
  return sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
}



//Main function
void DelphesLooper::loop(TChain* chain, std::string sample, std::string output_dir, float xsec, int nevt){

  evtweight_ = lumi*xsec*1000./nevt;
  cout << "[DelphesLooper::loop] Event weight for this sample, "<<sample<<", is "<<evtweight_<<endl;

  //Set up loop over chain
  unsigned int nEventsDone = 0;
  unsigned int nEventsToDo = chain->GetEntries();
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  std::map<std::string, TH1*> h_1d;

  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[DelphesLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.Data(),"RECREATE") ; 

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) { 

    // Get File Content
    if(nEventsDone >= nEventsToDo) continue;
    TFile *file = new TFile( currentFile->GetTitle() );
    TString filename(currentFile->GetTitle());
    
    TTree *tree = (TTree*)file->Get("Delphes");
    Delphes d(tree); // Create an instance of the delphes class and connect it to this tree
  
    // Loop over Events in current file
    unsigned int nEventsTree = tree->GetEntriesFast();
    for(unsigned int evt = 0; evt < nEventsTree; evt++){  // Real loop over all events
    //    for(unsigned int evt = 0; evt < 10; evt++){  // For Testing
	  
      // Get Event Content
      if(nEventsDone >= nEventsToDo) continue;   
      d.GetEntry(evt);
      nEventsDone++;

      //Debug mode
      if (verbose) cout << "file name is " << file->GetName() << endl;
     
      // Progress
      progress(nEventsDone, nEventsToDo);

      
      for ( int i = 0; i < d.Electron_ ; ++i) {
	//cout<<d.Electron_PT[i]<<endl;
	float pt = d.Electron_PT[i];
	plot1D("h_elpt", pt,  evtweight_, h_1d, "pT [GeV]", 40, 0, 400);
      }
      for ( int i = 0; i < d.MuonTight_ ; ++i) {
	//cout<<d.MuonTight_PT[i]<<endl;
	float pt = d.MuonTight_PT[i];
	plot1D("h_mupt", pt,  evtweight_, h_1d, "pT [GeV]", 40, 0, 400);
      }


      HT2p5_ = 0, HT_ = 0, njet302p5_ = 0, njet30_ = 0;
      for ( int i = 0; i < d.JetPUPPI_ ; ++i) {
	// Overlap Removal
	for ( int j = 0; j < d.Electron_ ; ++j) {
	  float DR = DeltaR(d.JetPUPPI_Eta[i], d.Electron_Eta[j], d.JetPUPPI_Phi[i], d.Electron_Phi[j]);
	  if (DR<0.4) continue;
	}
	for ( int j = 0; j < d.MuonTight_ ; ++j) {
	  float DR = DeltaR(d.JetPUPPI_Eta[i], d.MuonTight_Eta[j], d.JetPUPPI_Phi[i], d.MuonTight_Phi[j]);
	  if (DR<0.4) continue;
	}
	//cout<<d.JetPUPPI_PT[i]<<endl;
	float pt = d.JetPUPPI_PT[i];
	plot1D("h_jetpt", pt,  evtweight_, h_1d, "pT [GeV]", 200, 0, 500);
	if (pt>30) {
	  if (fabs(d.JetPUPPI_Eta[i]) < 2.5 ) {
	    HT2p5_ += pt;
	    njet302p5_ += pt;
	  }
	  HT_ += pt;
	  njet30_++;
	}
	    
      }
      
      MET_ = d.PuppiMissingET_MET[0];

      fillHistos(h_1d_base, "Base", ""); // Fill histos with no cuts

      if (njet30_>2)  fillHistos(h_1d_2j, "2Jets", ""); // Now fill histos with some cuts

      
      
    }// end of event loop
    
    file->Close();
    delete file;
    //delete tree;
    cout<<"\nFile done"<<endl;

  }//end of  file loop
  
  cout<<"\nWriting file"<<endl;
  savePlotsDir(h_1d, outfile_, "");
  savePlotsDir(h_1d_base, outfile_, "Base");
  savePlotsDir(h_1d_2j, outfile_, "2Jets");
  outfile_->Write();
  outfile_->Close();
  delete outfile_;
  
  return;  
  
}

void DelphesLooper::fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s){
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_HT2p5"+s, HT2p5_,  evtweight_, h_1d, "HT central [GeV]", 200, 0, 2000);
  plot1D("h_HTfull"+s, HT_,  evtweight_, h_1d, "HT [GeV]", 200, 0, 2000);
  plot1D("h_njet302p5"+s, njet302p5_,  evtweight_, h_1d, "N_j central", 10, 0, 10);
  plot1D("h_njet30"+s, njet30_,  evtweight_, h_1d, "N_j", 10, 0, 10);
  plot1D("h_MET"+s, MET_,  evtweight_, h_1d, "pT [GeV]", 200, 0, 2000);

  return;
}


