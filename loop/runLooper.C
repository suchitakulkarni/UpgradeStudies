#include <string>
#include "TChain.h"
#include "TString.h"

#include "DelphesLooper.h"


int main(int argc, char **argv) {

  if (argc < 6) {
    std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir> <xsec> <nevts>" << std::endl;
    return 1;
  }

  std::string input_dir(argv[1]); 
  std::string sample(argv[2]); 
  std::string output_dir(argv[3]);
  TString xsecS(argv[4]);
  TString nevtS(argv[5]);
  float xsec = xsecS.Atof();
  int   nevt = nevtS.Atoi();

  //std::cout<<"Strings: "<<xsecS<<" "<<nevtS<<std::endl;
  //std::cout<<"Floats:  "<<xsec<<" "<<nevt<<std::endl;
  if (nevt < 1000) {
    std::cout << "ERROR: nevt ("<<xsec<<") is very low. is this a real sample?" << std::endl;
    return 2;
  }
  if (xsec < 0.0001 || xsec > 300000) {
    std::cout << "ERROR: xsec value ("<<xsec<<") is out of reasonable bounds. is this a real sample?" << std::endl;
    return 2;
  }
  
  TChain *ch = new TChain("Delphes"); 
 
  TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  ch->Add(infile);
  int entries = ch->GetEntries();
  if (entries == 0) {
    std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
    return 2;
  }
  if (entries != 0) {
    std::cout << "WARNING: GetEntries ("<<entries<<") doesn't match NEVTS ("<<nevt<<") in this sample. Are you running on a skim? filename was: " << infile << std::endl;
    //return 2;
  }


  DelphesLooper *looper = new DelphesLooper();
  looper->loop(ch, sample, output_dir, xsec, nevt); 
  return 0;
}
