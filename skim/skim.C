#include <fstream>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

using namespace std;

void skim(string inpath = "/nfs-6/userdata/mt2/V00-00-03", string outpath = "/tmp/gzevi", string tag = "ttall_msdecays") {
  
  //--------------------------------------------------
  // cut for output files
  //--------------------------------------------------

  // Test: request that the two leading muons are isolated and same-sign
  const char* sel = "MuonTight[0].IsolationVarRhoCorr/MuonTight[0].PT<0.1 && MuonTight[1].IsolationVarRhoCorr/MuonTight[1].PT<0.1 && MuonTight[0].Charge*MuonTight[1].Charge>0";

  cout << "Skimming with selection : "<<sel<<endl;

  //--------------------------------------------------
  // input and output file
  //--------------------------------------------------
  
  const char* infilename = Form("%s/%s",inpath.c_str(),tag.c_str());  // no star, no root
  const char* infilename1 = Form("%s/%s*.root",inpath.c_str(),tag.c_str()); // file*.root
  const char* infilename2 = Form("%s/%s/*.root",inpath.c_str(),tag.c_str());  // dir/*.root
  const char* outfilename = Form("%s/%s.root",outpath.c_str(),tag.c_str());

  //--------------------------------------------------
  // cout stuff
  //--------------------------------------------------
  
  cout << "Reading in : " << infilename << endl;
  cout << "Writing to : " << outfilename << endl;
  cout << "Selection : " << sel << endl;
  
  //--------------------------------------------------
  // read input file, write to output files
  //--------------------------------------------------
  
  long long max_tree_size = 5000000000LL; // 5GB
  //long long max_tree_size = 2500000000LL; // 2.5GB
  TTree::SetMaxTreeSize(max_tree_size);
  
  TChain *chain = new TChain("Delphes");
  chain->Add(infilename);
  if (chain->GetEntries() == 0) {
    cout<<"No events in inputfile, trying "<<infilename1<<endl; 
    chain->Add(infilename1);
  }
  if (chain->GetEntries() == 0) {
    cout<<"No events in inputfile, trying "<<infilename2<<endl; 
    chain->Add(infilename2);
  }

  unsigned int input_entries = chain->GetEntries();
  cout << "Input tree has entries: " << input_entries << endl;
  
  TDatime a; 
  cout<< a.AsString() <<endl;


  //-------------------
  // slim
  //-------------------
  chain->SetBranchStatus("*",0);
  chain->SetBranchStatus("MuonTight.PT",1);
  chain->SetBranchStatus("MuonTight.Eta",1);
  chain->SetBranchStatus("MuonTight.Phi",1);
  chain->SetBranchStatus("MuonTight.Charge",1);
  chain->SetBranchStatus("MuonTight.IsolationVarRhoCorr",1);
  chain->SetBranchStatus("Electron.PT",1);
  chain->SetBranchStatus("Electron.Eta",1);
  chain->SetBranchStatus("Electron.Phi",1);
  chain->SetBranchStatus("Electron.Charge",1);
  chain->SetBranchStatus("Electron.IsolationVarRhoCorr",1);
  chain->SetBranchStatus("JetPUPPI.PT",1);
  chain->SetBranchStatus("JetPUPPI.Eta",1);
  chain->SetBranchStatus("JetPUPPI.Phi",1);
  chain->SetBranchStatus("JetPUPPI.BTag",1);
  chain->SetBranchStatus("JetPUPPI.BTagAlgo",1);
  chain->SetBranchStatus("JetPUPPI.BTagPhys",1);
  chain->SetBranchStatus("PuppiMissingET.MET",1);
  chain->SetBranchStatus("PuppiMissingET.Phi",1);
  chain->SetBranchStatus("ScalarHT.HT",1);

  // More variables, not in Basil's skim
  chain->SetBranchStatus("Particle.PID",1);
  chain->SetBranchStatus("Particle.PT",1);
  chain->SetBranchStatus("Particle.Eta",1);
  chain->SetBranchStatus("Particle.Phi",1);
  chain->SetBranchStatus("Particle.Status",1);
//  chain->SetBranchStatus("Particle.M1",1);// Useless because the CMS skim breaks mother-daughter connections: https://github.com/delphes/delphes/blob/master/modules/StatusPidFilter.cc
//  chain->SetBranchStatus("Particle.M2",1);

  //-------------------
  // skim
  //-------------------
  TFile *outfile = TFile::Open(outfilename, "RECREATE");
  assert( outfile != 0 );
  TTree* outtree = chain->CopyTree( sel );
  unsigned int output_entries = outtree->GetEntries();
  cout << "Output tree has entries: " << output_entries << endl
       << "Reduction factor of: " << double(input_entries)/double(output_entries) << endl;
  outtree->Write();
  outfile->Close();

  TDatime b; 
  cout<< b.AsString() <<endl;

}
