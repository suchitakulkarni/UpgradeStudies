#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "THStack.h"
#include "TColor.h"
#include "TF1.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"

//#include "plotUtilities.C"
#include "CMS_lumi.C"
#include <vector>

using namespace std;

TCanvas *canv;
TString outname = "prova.root";

//int favColors[5] = { kBlack, kOrange-2, kAzure-2, kViolet-2, kSpring-2};
int favColors[5] = { kOrange-2, kAzure-2, kViolet-2, kSpring-2};

//--------Turn on/off different plots-----------

//electron commissioning plots
bool doElCommish = true;
//doubleEl trigger turn-on plots
bool doDoubleElTrig = false;
//soft lepton wjets studies plots
bool doSoftLep = false;
//MT2 CRGJ data vs MC plots
bool doCRGJData = false;

//----------------------------------------------

/////////////////////////

const int iPeriod = 4; // 13 tev

/////////////////////////

// iPos drives the position of the CMS logo in the plot
// iPos=11 : top-left, left-aligned
// iPos=33 : top-right, right-aligned
// iPos=22 : center, centered
// mode generally : 
//   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
const int iPos = 11;

/////////////////////////

void Superimp(TString file1, TString file2, TString la1, TString la2, TString nplot, TString xname, int reb =1, TString fix="")
{
  TFile f1( file1, "READ");
  TFile f2( file2, "READ");

  TH1D* h1= (TH1D*) f1.Get(nplot)->Clone("h1");
  TH1D* h2= (TH1D*) f2.Get(nplot+fix)->Clone("h2");
  h1->Rebin(reb);
  h2->Rebin(reb);
  TLegend *l = new TLegend(0.80,0.75, 0.88,0.88);
//  TLegend *l = new TLegend(0.15,0.60, 0.45,0.85);
  l->AddEntry(h1, la1, "lp");
  l->AddEntry(h2, la2, "lp");
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);

  canv = new TCanvas();
  canv->cd();
  gStyle->SetOptStat(0);

  h1->SetMarkerStyle(23);
  h2->SetMarkerStyle(22);
  h2->SetLineColor(2);
  h2->SetMarkerColor(2);
  h1->GetXaxis()->SetTitle(xname);
  h2->GetXaxis()->SetTitle(xname);
  h1->GetYaxis()->SetRangeUser(0, h1->GetMaximum()*1.3);

  if ( xname.Contains("ntrk") )h1->GetXaxis()->SetRangeUser(0,20);
  if ( xname.Contains("Mass") )h1->GetXaxis()->SetRangeUser(0,7);
  h1->SetTitle("");
  h2->SetTitle("");
  h1->DrawNormalized();
  h2->DrawNormalized("same");
  //h1->Draw();
  //h2->Draw("same");
  l->Draw("same");

  canv->Print("compare/" + nplot + fix + ".pdf"); 
  delete canv;
  delete l   ;
  return;
}

void SuperimpVec(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool doLog = false, int reb=1 )
{
  
  canv = new TCanvas("c1_n48", "c1_n48");
  canv->SetBottomMargin(0.14);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.25,0.75, 0.65,0.98);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  bool firstToDraw = true;
  int skipped = 0;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (files[i]=="") {
      l->SetHeader(labels[i]);
      skipped++;
    }
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    h1->Rebin(reb);
    if ( labels[i].Contains("True Z inv") ) l->AddEntry(h1, labels[i], "f");
    else l->AddEntry(h1, labels[i], "lpe");
    h1->SetMarkerStyle(21+i-skipped);
    h1->SetLineColor(i+1-skipped);
    h1->SetMarkerColor(i+1-skipped);
    h1->GetXaxis()->SetTitle(xname);
    h1->GetXaxis()->SetTitleOffset(0.9);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    
    if (norm) {
      h1->Scale(1./h1->Integral());
    }
    // if (nplots[i].Contains("LooseNotTight")) {
    //   h1->Scale(0.05);
    // }

    if (nplots[i].Contains("matchedGenLess5")){
      h1->SetLineColor(kRed);
      h1->SetMarkerColor(kRed);
    }
    if (nplots[i].Contains("matchedGenGreater5")){
      h1->SetLineColor(kGreen);
      h1->SetMarkerColor(kGreen);
    }
    if (nplots[i].Contains("notMatched")){
      h1->SetLineColor(kBlue);
      h1->SetMarkerColor(kBlue);
    }
    if (nplots[i].Contains("SBNotMatched")){
      h1->SetLineColor(kBlue);
      h1->SetMarkerColor(kBlue);
      h1->SetLineStyle(2);
    }
    if (nplots[i].Contains("SBMatched")){
      h1->SetLineColor(kGreen+3);
      h1->SetMarkerColor(kGreen+3);
      h1->SetLineStyle(2);
    }

    TString drawing = "";
    
    if ( labels[i].Contains("True Z inv") ) {
      h1->SetFillColor(i+1);
      h1->SetLineStyle(0);
      h1->SetMarkerSize(0);
      drawing = "E2";
    }
    if (firstToDraw) {h1->Draw(drawing); firstToDraw=false;}
    else h1->Draw("same");
    if (maxY==0 && !doLog) maxY = h1->GetMaximum()*1.3;
    if (doLog) {  
      canv->SetLogy(1);
      if (maxY==0) maxY = h1->GetMaximum()*2;
      h1->GetYaxis()->SetRangeUser(0.001, maxY);
    }
    else h1->GetYaxis()->SetRangeUser(0, maxY);
    
    if (nplots[i].Contains("dijetflav")) {
      h1->GetXaxis()->SetBinLabel(1, "jj");
      h1->GetXaxis()->SetBinLabel(2, "cj");
      h1->GetXaxis()->SetBinLabel(3, "bj");
      h1->GetXaxis()->SetBinLabel(4, "cc");
      h1->GetXaxis()->SetBinLabel(5, "bc");      
      h1->GetXaxis()->SetBinLabel(6, "bb"); 
      h1->GetXaxis()->SetLabelSize(0.05);
      //h1->GetXaxis()->SetLabelOffset(0.02);
      
    }
    
    
    f1.Close();
  }
  l->Draw("same");
  canv->Print("plots/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;

}


void SuperimpRatios(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool unsetSumW2 = false, int rebin = 1, TString yname="")
{
  
  canv = new TCanvas("c1_n48", "c1_n48");
  canv->SetBottomMargin(0.14);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.50,0.65, 0.88,0.88);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  bool firstToDraw = true;
  int skipped = 0;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (files[i]=="") {
      l->SetHeader(labels[i]);
      skipped++;
    }
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");
    
    TFile f2( files[i+1], "READ");
    if(!f2.IsOpen()){
      std::cout<<"File "<<files[i+1]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f2.Get(nplots[i+1])==0){
      std::cout<<"Histogram "<<nplots[i+1]<<" not found in file "<<files[i+1]<<std::endl;
      continue;
    }
    TH1D* h2= (TH1D*) f2.Get(nplots[i+1])->Clone("h2");
    h1->Rebin(rebin);
    h2->Rebin(rebin);
    
    if (unsetSumW2) {
      h1->GetSumw2()->Set(0);   
      h2->GetSumw2()->Set(0); 
      h1->Sumw2(); // need to reset these so that the division is done correctly
      h2->Sumw2();
    }

    h1->Divide(h1,h2,1,1,"B");
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i-skipped);
    h1->SetLineColor(i+1-skipped);
    h1->SetMarkerColor(i+1-skipped);
    h1->GetXaxis()->SetTitle(xname);
    h1->GetXaxis()->SetTitleOffset(0.9);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    h1->GetYaxis()->SetTitleOffset(0.75);
    h1->GetYaxis()->SetTitle(yname);
    
    TString drawing = "";

    if (unsetSumW2) drawing = "E1";
    
    if (firstToDraw) {h1->Draw(drawing); firstToDraw=false;}
    else h1->Draw(drawing+"same");
    


    
    if (maxY==0) maxY = h1->GetMaximum()*1.3;
    
    h1->GetYaxis()->SetRangeUser(0, maxY);
    
    f1.Close();
    f2.Close();
    i++;
  }
  l->Draw("same");
  canv->Print("plots/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;
  
}


void SuperimpVecRatio(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, TString yname = "", float maxY=0, TString saveas="", bool norm=0, int X1 = 30, int X2 = 150, bool doLog = false)
{
  
  canv = new TCanvas("c1_n48", "c1_n48",10,32,787,573);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.70,0.35, 0.9,0.58);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  
  
  TPad* p01=new TPad("p01","p01",0,0.25,1,1);
  p01->SetFillStyle(4000);
  p01->SetFillColor(0);
  p01->SetFrameFillStyle(4000);
  p01->SetFrameFillColor(0);
  p01->SetFrameLineColor(0);
  p01->Range(-70.32233,-8.217202,421.3903,1071.598);
  p01->SetBorderMode(0);
  p01->SetBorderSize(2);
  p01->SetTickx(1);
  p01->SetTicky(1);
  p01->SetLeftMargin(0.1430151);
  p01->SetRightMargin(0.04350161);
  p01->SetTopMargin(0.05269226);
  p01->SetBottomMargin(0.007609824);
  p01->SetFrameBorderMode(0);
  p01->SetFrameBorderMode(0);
  p01->Draw();
  p01->cd();

  float autoMaxY = -999;
  bool firstToDraw = true;
  for (unsigned int i = 0; i < files.size(); i++) {
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");                
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i);
    h1->SetLineColor(i+1);
    h1->SetMarkerColor(i+1);

    if (saveas.Contains("Purity") && i+1 == 5){
      h1->SetLineColor(kViolet);
      h1->SetMarkerColor(kViolet);
    }
    h1->GetXaxis()->SetTitle(xname);
    h1->GetYaxis()->SetTitle(yname);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    if (norm) {
      h1->Scale(1./h1->Integral());
    }
    if (firstToDraw) {h1->Draw(); firstToDraw=false;}
    else h1->Draw("same");
    
    if (autoMaxY < h1->GetMaximum()*1.3 ) autoMaxY = h1->GetMaximum()*1.3;
    if (doLog) {  
      p01->SetLogy(1);   
      h1->GetYaxis()->SetRangeUser(1, maxY==0 ? autoMaxY*2 : maxY*2);
    }
    else h1->GetYaxis()->SetRangeUser(0, maxY==0 ? autoMaxY : maxY); // this doesn't quite work. Only first histogram is taken into account
    if ( saveas.Contains("Purity") )
      h1->GetYaxis()->SetRangeUser(0.7, 1.05);
    

    f1.Close();
  }
  l->Draw("same");
  
  canv->cd();
  TPad* p02=new TPad("p02","p02",0,0,1,0.25);
  p02->SetFillStyle(4000);
  p02->SetFillColor(0);
  p02->SetFrameFillStyle(4000);
  p02->SetFrameFillColor(0);
  p02->SetFrameLineColor(0);
  p02->Range(-69.27659,-17.79753,420.5338,19.06095);
  p02->SetLeftMargin(0.1414355);
  p02->SetRightMargin(0.04192203);
  p02->SetTopMargin(0.01176064);
  p02->SetBottomMargin(0.39156);
  p02->SetTickx(1);
  p02->SetTicky(1);
  p02->SetGridy();
  p02->SetBorderMode(0);
  p02->SetFrameBorderMode(0);
  p02->SetFrameBorderMode(0);
  p02->Draw();
  p02->cd();
  
  TFile f( files[0], "READ");
  if(!f.IsOpen()){
    std::cout<<"File "<<files[0]<<" can't be found, will not be used for ratio histogram "<<nplots[0]<<std::endl;
    //    continue;
  }
  if((TH1D*) f.Get(nplots[0])==0){
    std::cout<<"Histogram "<<nplots[0]<<" for ratio not found in file "<<files[0]<<std::endl;
    //continue;
  }
  TH1D* h0= (TH1D*) f.Get(nplots[0])->Clone("h0"); 
  TString nameTitle = files[0] + nplots[0] + labels[0];
  h0->SetNameTitle("","");
  h0->SetDirectory(0);
  f.Close();
  
  firstToDraw = true;
  for (unsigned int i = 1; i < files.size(); i++) {
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for ratio histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");                
    h1->Divide(h0);

    //dont include MCtruth in ratio
    if (nplots[i].Contains("purityTrue")) {
      continue;
    }
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    //l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i);
    h1->SetLineColor(i+1);
    h1->SetMarkerColor(i+1);
    h1->GetXaxis()->SetTitle(xname);
    
    h1->GetXaxis()->SetLabelSize(0.16);
    h1->GetXaxis()->SetTitleSize(0.17);
    //diff->GetXaxis()->SetTitleFont(42);
    h1->GetYaxis()->SetTitle("/ black");
    if ( saveas.Contains("Purity") )
      h1->GetYaxis()->SetTitle("Divided by Black");
    h1->GetYaxis()->SetNdivisions(505);
    //diff->GetYaxis()->SetLabelFont(42);
    h1->GetYaxis()->SetLabelSize(0.1);
    h1->GetYaxis()->SetTitleSize(0.12);
    h1->GetYaxis()->SetTitleOffset(0.38);
    //diff->GetYaxis()->SetTitleFont(42);
    if (norm || nplots[i].Contains("eff") || nplots[i].Contains("fr"))     
      h1->GetYaxis()->SetRangeUser(0.2, 1.8);

    if ( saveas.Contains("Purity") )
      h1->GetYaxis()->SetRangeUser(0.9, 1.1);

    // if ( saveas.Contains("lepPt_") )
    //   h1->GetYaxis()->SetRangeUser(0.5, 1.1);

    
    // if ( saveas.Contains("leppt") )
    //   h1->GetYaxis()->SetRangeUser(0.3, 0.7);
    
    if ( saveas.Contains("ratio") )
      h1->GetYaxis()->SetRangeUser(0.0, 0.5);
    
    if ( saveas.Contains("bkgsMuons") )
      h1->GetYaxis()->SetRangeUser(0.0, 0.5);

    //if ( saveas.Contains("base") || saveas.Contains("nocut") ) h1->GetYaxis()->SetRangeUser(0, 0.6);
    
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    if (nplots[i].Contains("fo_ID")) {
      h1->GetXaxis()->SetBinLabel(1, "");
      h1->GetXaxis()->SetBinLabel(2, "other");
      h1->GetXaxis()->SetBinLabel(3, "C-had");
      h1->GetXaxis()->SetBinLabel(4, "B-had");      
      h1->GetXaxis()->SetBinLabel(5, "fake"); 
      h1->GetXaxis()->SetLabelSize(0.20);
      h1->GetXaxis()->SetLabelOffset(0.02);

    }

    if (firstToDraw) {h1->Draw(); firstToDraw=false;}
    else h1->Draw("same");
    //h1->GetYaxis()->SetRangeUser(0, maxY);
    
    f1.Close();
  }
  //l->Draw("same");

  
  
  
  canv->Print("plots/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;
  
}


void makeCMSPlotRatioOnly(  vector<TString> files,  vector<TString> labels, vector<TString> nplots , TString saveas, const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = false, bool norm = false, bool doRatio = false , int plateau = 0, TString canvName = "") {

  bool doubleErf = true;

  cout << "-- plotting hist: " << nplots[0] << " etc "<<endl;
  
  cmsText = "CMS";
  writeExtraText = true;

  lumi_13TeV = "2.1 fb^{-1}";
  // lumi_13TeV = "";
  
  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);
  
  gStyle->SetOptFit(1);

  TString canvas_name = saveas;
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  string typeStr = "";
  
  // if (canvName != ""){
  //   TFile fCanv( Form("canvasFiles/%s.root",canvName.Data()), "READ");
  //   if (fCanv.IsOpen()) can = (TCanvas*) fCanv.Get(canvas_name);
  // }
  can->cd();
  
  // splitting canvas for ratio plots
  TPad* fullpad(0);
  TPad* plotpad(0);
  
  
  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  //TLegend* leg = new TLegend(0.55,0.7,0.85,0.90);
  TLegend* leg = new TLegend(0.5,0.25,0.85,0.35);
  if (labels[1].Contains("MC")) leg = new TLegend(0.5,0.19,0.85,0.25);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.028);
  if (doRatio) leg->SetTextSize(0.05);
  
  TH1D* data_hist(0);

  TH1D* h_bgtot = 0;
 
  vector<TH1D*> sig_hists;
  vector<TString> sig_names;

  // signal hists - all samples must have "sig" in the name
  float dataintegral = 0;
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i]);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    TH1D* h = (TH1D*) h_temp->Clone("h1");
    h->SetDirectory(0);
    //    h->Sumw2();
    
    // h->SetLineColor(1+i);
    // if (i>=2) h->SetLineColor(2+i);
    // if (i>=3) h->SetLineColor(3+i);

    h->SetLineColor(favColors[i]);
    if (labels[i].Contains("MC")) h->SetLineColor(favColors[2*i]);
	
    h->SetLineWidth(3);
    if (rebin > 1) h->Rebin(rebin);
    if (norm ) {
      if (nplots[i].Contains("Mass") ) {
        if (files[i].Contains("data")) dataintegral = h->Integral();
        if (!files[i].Contains("data")) h->Scale(  dataintegral / h->Integral()  );
      }
      else h->Scale(1./h->Integral());
    }
    sig_hists.push_back(h);
    sig_names.push_back(labels[i]);
  }
  
  float ymax = 0;
  //if(h_bgtot) ymax = h_bgtot->GetMaximum();
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    if (sig_hists.at(isig)->GetMaximum() > ymax) ymax = sig_hists.at(isig)->GetMaximum();
  }
  float ymin = 0;
  if( logplot ) {ymax*=20; ymin = 0.1;}
  else    ymax*=1.5;
  
  if (logplot && norm) ymin = 0.001;
  
  
  if ( xmin==0 ) xmin = sig_hists.at(0)->GetXaxis()->GetXmin();
  if ( xmax==0 ) xmax = sig_hists.at(0)->GetXaxis()->GetXmax();
  
  
  //TH2F* h_axes = new TH2F(nplots[0],"",1000,xmin,xmax,1000,ymin,ymax);
  TH2F* h_axes = new TH2F("h_axes","",1000,xmin,xmax,1000,0,1); // Make Ratio between 0 and 1
  if (plateau == -1) h_axes->GetYaxis()->SetRangeUser(0.9,1); //make ratio between 0.9 and 1 for DZ eff
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.04);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  h_axes->GetYaxis()->SetTitleOffset(1.12);
  h_axes->GetYaxis()->SetTitleSize(0.04);
  if (doRatio) {
    h_axes->GetXaxis()->SetLabelSize(0.);
    h_axes->GetXaxis()->SetTitleSize(0.);
  }
  h_axes->Draw("sames");
  
  //t->Draw("hist same");
  h_bgtot=(TH1D*) sig_hists.at(0)->Clone("");
  h_bgtot->Sumw2();
  // add signal hists
  for (unsigned int isig = 1; isig < sig_hists.size(); ++isig) {
    //sig_hists.at(isig)->Draw("hist same");
    //sig_hists.at(isig)->Draw("histsame");
    leg->AddEntry(sig_hists.at(isig),sig_names.at(isig),"lp");
    // make ratio pad and plot
    TH1D* h_ratio = (TH1D*) sig_hists.at(isig)->Clone(Form("ratio_%s",sig_hists.at(isig)->GetName()));
    h_ratio->Sumw2();
    h_ratio->Divide(h_ratio, h_bgtot, 1, 1, "B");
    TGraphErrors* g_ratio = new TGraphErrors(h_ratio);
    g_ratio->SetName(Form("%s_graph",h_ratio->GetName()));
    for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
      g_ratio->SetPointError(ibin, h_ratio->GetBinWidth(ibin+1)/2., h_ratio->GetBinError(ibin+1));
    }
    //g_ratio->SetLineColor(kBlack);
    //g_ratio->SetMarkerColor(kBlack);
    //g_ratio->SetMarkerStyle(20);
    g_ratio->Draw("p0sames");
    gPad->Update();


    if (doDoubleElTrig) {

      if(!sig_names[isig].Contains("MC")) typeStr = "DATA";
      if( sig_names[isig].Contains("MC")) typeStr = "MC";
      
      
      if(plateau>0){
	//fitting code
	if(!sig_names[isig].Contains("MC")){
	  if(!doubleErf){
	    TF1* fitfcn = new TF1("fitfcn","0.5 * [0] * (1+TMath::Erf((x-[1])/TMath::Sqrt(2)*[2]))",0,60);
	    fitfcn->SetParameters(0.9,10,0.2);
	    fitfcn->SetParLimits(0,0.75,1);
	    fitfcn->SetParLimits(1,0,30);
	    fitfcn->SetParLimits(2,0.1,0.6);
	    fitfcn->SetLineColor(kOrange);
	    g_ratio->Fit("fitfcn","MEX0RS","",plateau,75);
	    g_ratio->Fit("fitfcn","MEX0RS","",plateau,75);
	    g_ratio->Fit("fitfcn","MEX0RS","",plateau,75);
	    gPad->Update();
	    //effPlat = fitfcn->GetParameter(0);
	  }
	  
	  //double erf
	  else{
	    TF1* fitfcn = new TF1("fitfcn","0.5 * [0] * (1+TMath::Erf((x-[1])/TMath::Sqrt(2)*[2])) + 0.5 * [3] * (1+TMath::Erf((x-[4])/TMath::Sqrt(2)*[5]))",0,60);
	    fitfcn->SetParameters(0.85,20,0.7,0.1,30,0.1);
	    fitfcn->SetParLimits(0,0.5,0.9);
	    fitfcn->SetParLimits(1,5,40);
	    fitfcn->SetParLimits(2,0,1);
	    fitfcn->SetParLimits(3,0.0,0.5);
	    fitfcn->SetParLimits(4,5,40);
	    fitfcn->SetParLimits(5,0,0.5);
	    fitfcn->GetParameter(0);
	    fitfcn->SetLineColor(kOrange);
	    g_ratio->Fit("fitfcn","MEX0RS","",0,70);
	    g_ratio->Fit("fitfcn","MEX0RS","",0,70);
	    g_ratio->Fit("fitfcn","MEX0RS","",0,70);
	    gPad->Update();
	  }
	}
	if(sig_names[isig].Contains("MC")){
	  if(!doubleErf){
	    TF1* fitfcnMC = new TF1("fitfcnMC","0.5 * [0] * (1+TMath::Erf((x-[1])/TMath::Sqrt(2)*[2]))",0,60);
	    fitfcnMC->SetParameters(0.9,10,0.2);
	    fitfcnMC->SetParLimits(0,0.75,1);
	    fitfcnMC->SetParLimits(1,0,30);
	    fitfcnMC->SetParLimits(2,0.1,0.6);
	    //fitfcnMC->SetLineStyle(4);
	    fitfcnMC->SetLineColor(kAzure);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",plateau,75);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",plateau,75);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",plateau,75);
	    gPad->Update();
	    //effPlat = fitfcnMC->GetParameter(0);
	  }
	  
	  //double erf
	  else{
	    TF1* fitfcnMC = new TF1("fitfcnMC","0.5 * [0] * (1+TMath::Erf((x-[1])/TMath::Sqrt(2)*[2])) + 0.5 * [3] * (1+TMath::Erf((x-[4])/TMath::Sqrt(2)*[5]))",0,60);
	    fitfcnMC->SetParameters(0.85,20,0.7,0.1,30,0.1);
	    fitfcnMC->SetParLimits(0,0.5,0.9);
	    fitfcnMC->SetParLimits(1,5,40);
	    fitfcnMC->SetParLimits(2,0,1);
	    fitfcnMC->SetParLimits(3,0.0,0.5);
	    fitfcnMC->SetParLimits(4,5,40);
	    fitfcnMC->SetParLimits(5,0,0.5);
	    fitfcnMC->GetParameter(0);
	    fitfcnMC->SetLineColor(kAzure);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",0,70);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",0,70);
	    g_ratio->Fit("fitfcnMC","MEX0RS","",0,70);
	    gPad->Update();	
	  }
	}
      }//plateau>0
      
      if (plateau==-1 && !sig_names[isig].Contains("MC")){
	//fitting code
	TF1* fitfcn = new TF1("fitfcn","pol0",0,60);
	g_ratio->Fit("fitfcn","","",20,75);
	gPad->Update();
      }
      
    }//doDoubleElTrig
    
    
    
  }

  
  //if (data_hist) data_hist->Draw("pe same");
  
  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  float label_y_start = 0.82;
  float label_y_spacing = 0.04;
  
  leg->Draw("same");
  //h_axes->Draw("axissame");
  
  
  // -- for CMS_lumi label
  
  const int iPeriod = 4; // 13 tev
  
  // iPos drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally :
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  //const int iPos = 11;
  const int iPos = 11;
  
  
  
  lumiTextSize     = 0.45;
  cmsTextSize      = 0.5;
  CMS_lumi( can, iPeriod, iPos );
  
  
  gPad->Modified();


  can->SaveAs(Form("canvasFiles/%s%s.root",canvName.Data(),typeStr.c_str()));
  // if (plateau==-1 || sig_names[0].Contains("Ele23")) can->Print(Form("plots/%s.pdf",canvas_name.Data()));
  if (typeStr != "MC") can->Print(Form("plots/%s.pdf",canvas_name.Data()));

  
  TCanvas* canData = new TCanvas(canvas_name,canvas_name, 600, 600);
  TCanvas* canMC = new TCanvas(canvas_name,canvas_name, 600, 600);
  TCanvas* canFinal = new TCanvas(canvas_name,canvas_name, 600, 600);
  
  if (canvName != "" && (typeStr=="MC" && !sig_names[0].Contains("Ele23")) && doDoubleElTrig){
    TFile fCanvData( Form("canvasFiles/%s%s.root",canvName.Data(),"DATA"), "READ");
    if (fCanvData.IsOpen()) canData = (TCanvas*) fCanvData.Get(canvas_name);
    TFile fCanvMC( Form("canvasFiles/%s%s.root",canvName.Data(),"MC"), "READ");
    if (fCanvMC.IsOpen()) canMC = (TCanvas*) fCanvMC.Get(canvas_name);

    TH2F* haxes = (TH2F*) canData->FindObject("h_axes");
    TGraphErrors * g1 = (TGraphErrors*) canData->FindObject("ratio_h1_graph");
    TGraphErrors * g2 = (TGraphErrors*) canMC->FindObject("ratio_h1_graph");
    TPaveStats * st1 = (TPaveStats*) g1->FindObject("stats");
    TPaveStats * st2 = (TPaveStats*) g2->FindObject("stats");
    TLegend * l1 = (TLegend*) canData->FindObject("TPave");
    TLegend * l2 = (TLegend*) canMC->FindObject("TPave");

    gStyle->SetOptFit(1);
		
    //show stats box
    //st1->SetTextColor(kOrange);
    st1->SetLineColor(kOrange);
    st1->SetX1NDC(0.50); //new x start position
    st1->SetX2NDC(0.85); //new x end position
    st1->SetY1NDC(0.40); //new y start position
    st1->SetY2NDC(0.54); //new y end position
  
    //st2->SetTextColor(kAzure);
    st2->SetLineColor(kAzure);
    st2->SetX1NDC(0.50); //new x start position
    st2->SetX2NDC(0.85); //new x end position
    st2->SetY1NDC(0.56); //new y start position
    st2->SetY2NDC(0.70); //new y end position
      
    canFinal->cd();

    haxes->Draw();
    g1->Draw("sames");
    gPad->Update();
    g2->Draw("sames");
    l1->Draw("same");
    l2->Draw("same");
    CMS_lumi( canFinal, iPeriod, iPos );

    
    //write SF on canvas
    TF1* f1 = (TF1*) g1->FindObject("fitfcn");
    TF1* f2 = (TF1*) g2->FindObject("fitfcnMC");
    float eff1 = f1->GetParameter(0);
    if (doubleErf) eff1 += f1->GetParameter(3);
    float eff2 = f2->GetParameter(0);
    if (doubleErf) eff2 += f2->GetParameter(3);
    float sf =eff1/eff2;
    //write SF down for single erf fit
    stringstream ss (stringstream::in | stringstream::out);
    ss << std::setprecision(3) << sf; 
    TLatex *t = new TLatex();
    t->SetNDC();
    t->SetTextAlign(22);
    t->SetTextFont(63);
    t->SetTextSizePixels(22);
    string platStr = "Data/MC: "+ss.str();
    t->DrawLatex(.7,.75,platStr.c_str());
      
    
    
    canFinal->Print(Form("plots/%s.pdf",canvas_name.Data()));

  }  

  //if (plateau==-1) can->Print(Form("plots/%s.pdf",canvas_name.Data()));
    
  return;
}

void makeCMSPlotSignalBackground(  vector<TString> files,  vector<TString> labels, vector<TString> nplots , vector<TString> purpose , TString saveas, const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = false, bool norm = false, bool doRatio = false ) {
  
  cout << "-- plotting hist: " << nplots[0] << " etc "<<endl;
  
  cmsText = "CMS Simulation Preliminary";
  writeExtraText = false;
  //lumi_13TeV = "42 pb^{-1}";
  //lumi_13TeV = "20.38 pb^{-1}";
  lumi_13TeV = "300 fb^{-1}";
  
  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);
  
  // if ratio was requested, check if data is present.  If not, turn ratio off again
  //  const unsigned int n = samples.size();
  //  if (doRatio) {
  //    bool foundData = false;
  //    for( unsigned int i = 0 ; i < n ; ++i ) {
  //      if( TString(names.at(i)).Contains("data")  ) {
  //        foundData = true;
  //        break;
  //      }
  //    }
  //    if (!foundData) {
  //      cout << "ratio requested but no data hist found.  Not plotting ratio" << endl;
  //      doRatio = false;
  //    }
  //  } // if doRatio
  
  if (!doRatio) {
    //these 4 lines shift plot to make room for axis labels
    gStyle->SetPadTopMargin(0.08);
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
  }
  
  TString canvas_name = saveas;
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  can->cd();
  if (!doRatio) {
    if (logplot) can->SetLogy();
    gPad->SetRightMargin(0.05);
    gPad->Modified();
  }
  
  // splitting canvas for ratio plots
  TPad* fullpad(0);
  TPad* plotpad(0);
  
  if (doRatio) {
    // master pad
    fullpad = new TPad("fullpad","fullpad",0,0,1,1);
    fullpad->Draw();
    fullpad->cd();
    
    // main plot pad, for ratio on bottom
    // plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    plotpad->SetTopMargin(0.05);
    plotpad->SetRightMargin(0.05);
    plotpad->SetBottomMargin(0.05);
    plotpad->Draw();
    plotpad->cd();
    if( logplot ) plotpad->SetLogy();
  }
  
  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  //TLegend* leg = new TLegend(0.55,0.7,0.85,0.90);
  //  TLegend* leg = new TLegend(0.25,0.7,0.85,0.90);
  TLegend* leg = new TLegend(0.65,0.75,0.85,0.90);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);
  if (doRatio) leg->SetTextSize(0.05);
  
  TH1D* data_hist(0);
  TString data_name;
  float dataintegral = 0;
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    if( !TString(purpose.at(i)).Contains("data")  && !TString(purpose.at(i)).Contains("Data")) continue;
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i]);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    data_hist = (TH1D*) h_temp->Clone("h1");
    data_hist->SetDirectory(0);
    //    data_hist->Sumw2();
    dataintegral = data_hist->Integral();
    data_name = labels[i];
    data_hist->SetLineColor(kBlack);
    data_hist->SetMarkerColor(kBlack);
    data_hist->SetMarkerStyle(20);
    if (rebin > 1) data_hist->Rebin(rebin);
    // fake data -> set error bars to correspond to data stats
    if (TString(data_name).Contains("fakedata")) {
      for (int ibin = 0; ibin <= data_hist->GetNbinsX(); ++ibin) {
	data_hist->SetBinError( ibin, sqrt(data_hist->GetBinContent(ibin)) );
      }
    } // if fakedata
    // expect to only find 1 data hist
    break;
  }
  
  if (data_hist) leg->AddEntry(data_hist,data_name,"pe");
  
  THStack* t = new THStack("stack","stack");
  TH1D* h_bgtot = 0;
  //
  //  // to make legend and find max yvalue
  vector<TH1D*> bg_hists;
  vector<TString> bg_names;
  vector<TH1D*> sig_hists;
  vector<TString> sig_names;
  float bkgintegral = 0;
  // background hists
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    if( !TString(purpose.at(i)).Contains("bkg")  ) continue;
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i]);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    TH1D* h = (TH1D*) h_temp->Clone("h1");
    h->SetDirectory(0);
    
    h->SetFillColor(i+1);
    h->SetLineColor(i+1);
    if (i>=2) h->SetFillColor(2+i);
    if (i>=3) h->SetFillColor(3+i);
    if (i>=2) h->SetLineColor(2+i);
    if (i>=3) h->SetLineColor(3+i);
    if (rebin > 1) h->Rebin(rebin);
    
    
    //hack to fix colors for one bkg
    // h->SetFillColor(kOrange-2);
    // h->SetLineColor(kOrange-2);

    h->SetFillColor(favColors[i]);
    h->SetLineColor(favColors[i]);
    
      
    //t->Add(h);
    if( h_bgtot==0 ) {
      
      h_bgtot = (TH1D*) h->Clone("bkgtot");
      h_bgtot->SetDirectory(0);
	
    }
    else {h_bgtot->Add(h); }
    
    bkgintegral += h->Integral();
    bg_hists.push_back(h);
    bg_names.push_back(labels[i]);
  }
  
  // loop backwards to normalize and add to legend
  for (int ibg = (int) bg_hists.size()-1; ibg >= 0; --ibg) {
    leg->AddEntry(bg_hists.at(ibg),bg_names.at(ibg),"f");
    if (norm && bkgintegral > 0) {
      bg_hists.at(ibg)->Scale( dataintegral / bkgintegral);
      //h_bgtot->Scale( dataintegral / bkgintegral);
    }
    t->Add(bg_hists.at(ibg));
  }
  if (norm && bkgintegral > 0) { h_bgtot->Scale( dataintegral / bkgintegral);}
  cout << "Data/MC: " << dataintegral / bkgintegral << endl;  
  //for wjets SR/CR ratio
  if (doSoftLep) {  cout << "Data/MC: " << dataintegral / bkgintegral << endl; }
  float srIntegral = -1;
  float crIntegral = -1;
  
  
  // signal hists - all samples must have "sig" in the name
  
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    if( !TString(purpose.at(i)).Contains("sig")  ) continue;
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i]);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    TH1D* h = (TH1D*) h_temp->Clone("h1");
    h->SetDirectory(0);
    //    h->Sumw2();
    h->SetLineColor(sig_hists.size()+1);
    h->SetLineWidth(2 );

    //h->SetLineColor(favColors[i]);
    cout << "sig " << i << " integral:" << h->Integral() << endl;
    if (i==0) srIntegral = h->Integral();
    if (i==1) crIntegral = h->Integral();
    
    if (rebin > 1) h->Rebin(rebin);
    if (norm ) {
      if (dataintegral != 0) h->Scale(  dataintegral / h->Integral()  );
      else h->Scale(  1 / h->Integral()  );
    }
    sig_hists.push_back(h);
    sig_names.push_back(labels[i]);
  }

  if (doSoftLep) {
    float crOverSr = crIntegral/srIntegral;
    cout << "CR/SR: " << crOverSr << endl;
    stringstream ss (stringstream::in | stringstream::out);
    ss << std::setprecision(2) << crOverSr;
    lumi_13TeV = "CR/SR = "+ss.str();
  }

  if (doDoubleElTrig) {
    lumi_13TeV = "2.1 fb^{-1}";
  }
  
  float ymax = 0;
  if(h_bgtot!=0) ymax = h_bgtot->GetMaximum();
  
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    sig_hists.at(isig)->GetXaxis()->SetRangeUser(xmin, xmax);
    if (sig_hists.at(isig)->GetMaximum() > ymax) ymax = sig_hists.at(isig)->GetMaximum();
  }
  
  //also check data for max y val
  if (data_hist) { if (data_hist->GetMaximum() > ymax) ymax = data_hist->GetMaximum(); }
      
  float ymin = 0;
  if( logplot ) {ymax*=20; ymin = 0.1;}
  else    ymax*=1.5;
  
  if (logplot && norm) ymin = 0.1;
  
  
  if ( !saveas.Contains("fbrem") && xmin==0 && sig_hists.size()>0) xmin = sig_hists.at(0)->GetXaxis()->GetXmin();
  if ( xmax==0 && sig_hists.size()>0) xmax = sig_hists.at(0)->GetXaxis()->GetXmax();
  if ( !saveas.Contains("fbrem") && xmin==0 && data_hist) xmin = data_hist->GetXaxis()->GetXmin();
  if ( xmax==0 && data_hist) xmax = data_hist->GetXaxis()->GetXmax();
  
  
  TH2F* h_axes = new TH2F(nplots[0],"",1000,xmin,xmax,1000,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  h_axes->GetYaxis()->SetTitleOffset(1.09);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  if (doRatio) {
    h_axes->GetXaxis()->SetLabelSize(0.);
    h_axes->GetXaxis()->SetTitleSize(0.);
  }
  h_axes->Draw();
  
  t->Draw("hist same");
  
  // add signal hists
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    //sig_hists.at(isig)->Draw("hist same");
    sig_hists.at(isig)->Draw("histsame e");
    leg->AddEntry(sig_hists.at(isig),sig_names.at(isig),"lp");
  }
  
  if (data_hist) data_hist->Draw("pe same");

  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  float label_y_start = 0.82;
  float label_y_spacing = 0.04;
  if (doRatio) {
    label.SetTextSize(0.039);
    label_y_start = 0.84;
    label_y_spacing = 0.04;
  }

  //  //TString ht_label = getHTPlotLabel(histdir);
  //  TString ht_label = getHTPlotLabel(samples.at(0), histdir);
  //  TString region_label = getJetBJetPlotLabel(samples.at(0), histdir);
  //  TString region_label_line2 = getMT2PlotLabel(samples.at(0), histdir);
  //  //label.DrawLatex(0.2,0.85,ht_label);
  //  label.DrawLatex(0.187,label_y_start,ht_label);
  //  // base region plots all have at least 2 jets
  //  if ((histdir.find("base") != std::string::npos)) region_label = "#geq 2j";
  //  // minMT plot always requires at least 2 bjets
  //  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  //  // lostlepton CR
  //  if ((histdir.find("crsl") != std::string::npos)) region_label += ", 1 lepton";
  //
  //  if (region_label.Length() > 0) label.DrawLatex(0.187,label_y_start - label_y_spacing,region_label);
  //  if (region_label_line2.Length() > 0) label.DrawLatex(0.187,label_y_start - 2 * label_y_spacing,region_label_line2);
  
  leg->Draw();
  h_axes->Draw("axissame");

  
  // -- for CMS_lumi label

  const int iPeriod = 4; // 13 tev
  
  // iPos drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally :
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  //const int iPos = 11;
  const int iPos = 3;
  
  
  if (doRatio) {
    lumiTextSize     = 0.8;
    cmsTextSize      = 1.0;
    CMS_lumi( plotpad, iPeriod, iPos );
  }
  else {
    lumiTextSize     = 0.45;
    cmsTextSize      = 0.55;
    CMS_lumi( can, iPeriod, iPos );
  }

  // make ratio pad and plot
  if (doRatio) {
    // draw ratio pad
    fullpad->cd();
    TPad* ratiopad = new TPad("ratiopad","ratiopad",0.,0.,1,0.23);
    //ratiopad->SetLeftMargin(0.16);
    //ratiopad->SetLeftMargin(0.0);
    ratiopad->SetRightMargin(0.05);
    ratiopad->SetTopMargin(0.08);
    ratiopad->SetBottomMargin(0.44);
    ratiopad->SetGridy();
    ratiopad->Draw();
    ratiopad->cd();

    
    TH1D* h_ratio = (TH1D*) data_hist->Clone(Form("ratio_%s",data_hist->GetName()));
    h_ratio->Sumw2();
    h_bgtot->Sumw2();
    h_ratio->Divide(h_bgtot);

    //get max/min for ratio
    float ratio_min = h_ratio->GetMinimum();
    float ratio_max = h_ratio->GetMaximum();

    //recompute min/max by calculating average of ratio histogram
    float ratio_average = h_ratio->Integral();
    ratio_average = ratio_average / (xmax-xmin);
    ratio_min = ratio_average * 0.5;
    ratio_max = ratio_average * 1.5;
    
    // draw axis only
    TH1F* h_axis_ratio = new TH1F(Form("%s_axes",h_ratio->GetName()),"",100,xmin,xmax);
    h_axis_ratio->GetYaxis()->SetTitleOffset(0.3);
    h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
    h_axis_ratio->GetYaxis()->SetNdivisions(5);
    h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
    h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
//    h_axis_ratio->GetYaxis()->SetRangeUser(0.5,2.5);
    //h_axis_ratio->GetYaxis()->SetRangeUser(ratio_min*0.9,ratio_max*1.1);
    //h_axis_ratio->GetYaxis()->SetRangeUser(0.001,2.0);
    h_axis_ratio->GetYaxis()->SetTitle("Data/MC");
    h_axis_ratio->GetXaxis()->SetTitle(h_axes->GetXaxis()->GetTitle());
    h_axis_ratio->GetXaxis()->SetTitleSize(0.17);
    h_axis_ratio->GetXaxis()->SetLabelSize(0.17);
    h_axis_ratio->GetXaxis()->SetTitleOffset(1.0);
    h_axis_ratio->GetXaxis()->SetTickLength(0.07);
    h_axis_ratio->Draw("axis");
    
    TGraphErrors* g_ratio = new TGraphErrors(h_ratio);
    g_ratio->SetName(Form("%s_graph",h_ratio->GetName()));
    for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
      g_ratio->SetPointError(ibin, h_ratio->GetBinWidth(ibin+1)/2., h_ratio->GetBinError(ibin+1));
    }
    g_ratio->SetLineColor(kBlack);
    g_ratio->SetMarkerColor(kBlack);
    g_ratio->SetMarkerStyle(20);
    g_ratio->Draw("p0same");

    //h_axis_ratio->Delete();
    
  } // if (doRatio)

  gPad->Modified();

  can->Print(Form("plots/%s.pdf",canvas_name.Data()));

  return;
}

void compareMultiPlot()
{

  string dir = "~/UCSD/Upgrade/Hists/";
  // Want to run Superimp with a vector of files, a vector of plots, a vector of labels.
  //void SuperimpVec(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas)
  vector<TString> files; std::vector<TString> labels; vector<TString> nplots; vector<TString> purpose;

  string plotName = "";
  string dataset = "";
  string BBFile = "BBall.root";
  string BBBFile = "BBBall.root";
  string sigFile = "rns_c2n4_decayedALL.root";
  
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("Diboson");                     nplots.push_back("Base/h_MET"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("Base/h_MET"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("Base/h_MET"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "METbeforeLepVeto", /*xT*/"MET" , /*yT*/ "Events" , /*xM*/ 0, /*xM*/1000 , /*rebin*/ 4 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_MET"); purpose.push_back("bkg");
//  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_MET"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_MET"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("lepVeto/h_MET"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "MET", /*xT*/"MET" , /*yT*/ "Events" , /*xM*/ 0, /*xM*/1000 , /*rebin*/ 4 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
    
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_HTfull"); purpose.push_back("bkg");
  //  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_HTfull"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_HTfull"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("lepVeto/h_HTfull"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "HT", /*xT*/"HT(jet 30 GeV)" , /*yT*/ "Events" , /*xM*/ 0, /*xM*/1000 , /*rebin*/ 4 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_njet30forward"); purpose.push_back("bkg");
  //  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_njet30forward"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_njet30forward"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("Base/h_njet30forward"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "NJforward", /*xT*/"N(jet 30 GeV)" , /*yT*/ "Events" , /*xM*/ -0.5, /*xM*/9.5 , /*rebin*/ 1 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  
  
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_njet30central"); purpose.push_back("bkg");
  //  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_njet30central"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_njet30central"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("Base/h_njet30central"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "NJcentral", /*xT*/"N(jet 30 GeV)" , /*yT*/ "Events" , /*xM*/ -0.5, /*xM*/9.5 , /*rebin*/ 1 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  

  
  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_njet30"); purpose.push_back("bkg");
  //  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_njet30"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_njet30"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("Base/h_njet30"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "NJ", /*xT*/"N(jet 30 GeV)" , /*yT*/ "Events" , /*xM*/ -0.5, /*xM*/9.5 , /*rebin*/ 1 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  

  
  files.clear(); labels.clear(); nplots.clear(); purpose.clear();
  files.push_back(dir+BBFile);        labels.push_back("WZ");                     nplots.push_back("WZ/h_mtmin"); purpose.push_back("bkg");
  //  files.push_back(dir+BBFile);        labels.push_back("OSWW");                     nplots.push_back("OSWW/h_MET"); purpose.push_back("sig"); --> Should try to understand OSWW
  files.push_back(dir+BBFile);        labels.push_back("SSWW");                     nplots.push_back("SSWW/h_mtmin"); purpose.push_back("bkg");
  files.push_back(dir+BBBFile);        labels.push_back("Triboson");                     nplots.push_back("lepVeto/h_mtmin"); purpose.push_back("bkg");
  files.push_back(dir+sigFile);       labels.push_back("C2N4");                   nplots.push_back("lepVeto/h_mtmin"); purpose.push_back("sig");
  makeCMSPlotSignalBackground(files, labels, nplots, purpose, "MTmin", /*xT*/"MTmin [GeV]" , /*yT*/ "Events" , /*xM*/ 0, /*xM*/0 , /*rebin*/ 10 , /*log*/ 1, /*norm*/ 0, /*ratio*/ 0 );
  
  
  
  
  return;
}
