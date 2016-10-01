
#ifndef base_dis_histograms_h
#define base_dis_histograms_h

// c++ 
#include <iostream>
using namespace std; 

// me 
#include "DBins.h"
#include "PhysicsEvent.h"

// root 
#include "TFile.h"
#include "TH2.h"

class BaseDISHistograms{
 public:
  BaseDISHistograms();
  ~BaseDISHistograms();

  const static int numberOfXBins = 200;
  const static int numberOfQQBins = 200; 
  const static int numberOfWBins = 200;

  const static double xMin  = 0.1;  const static double xMax  = 0.6; 
  const static double qqMin = 1.0;  const static double qqMax = 4.8;
  const static double wMin  = 2.05; const static double wMax  = 3.1; 
  
  TFile * inputFile; 
  TH2I * xByQQ[7]; 
  TH2I * wByQQ[7]; 

  void Init(std::string name, std::string title);
  void Load(std::string filename, std::string name); 
  void Save(std::string filename, std::string option);
  void Fill(PhysicsEvent event, int sector); 
  void Rebin2D(int xFactor, int yFactor);
  
  // These were added for recAndGen coincident in the same bin,
  // there should be a better way to accomplish this.  Keep thinkin.
  void FillxByQQ(double x, double qq, int sector);
  void FillwByQQ(double w, double qq, int sector);
  
  //ClassDef(BaseDISHistograms, 1); 
};
/*
#endif

#ifndef base_dis_histograms_cxx
#define base_dis_histograms_cxx
*/
BaseDISHistograms::BaseDISHistograms(){

}

BaseDISHistograms::~BaseDISHistograms(){
  if (inputFile->IsOpen()){
    inputFile->Close();
  }
}

void BaseDISHistograms::Init(string name, string title){
  for (int isect = 0; isect < 7; isect++){
    xByQQ[isect] = new TH2I(Form("%s_xByQQ_s%d",name.c_str(),isect),Form("%s x vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfXBins, xMin, xMax, numberOfQQBins, qqMin, qqMax);
    wByQQ[isect] = new TH2I(Form("%s_wByQQ_s%d",name.c_str(),isect),Form("%s w vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfWBins, wMin, wMax, numberOfQQBins, qqMin, qqMax);
  }
}

void BaseDISHistograms::Save(string filename, string option){

  TFile * file = TFile::Open(filename.c_str(),option.c_str());

  if (file->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect]->Write();
      wByQQ[isect]->Write();
    }
  } else {
    cout << " Error opening file... " << filename << endl; 
  }

  file->Write();
  file->Close();
}

void BaseDISHistograms::Load(string filename, string name){

  TFile * inputFile = TFile::Open(filename.c_str());

  if (inputFile->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect] = (TH2I*) inputFile->Get(Form("%s_xByQQ_s%d",name.c_str(),isect));
      wByQQ[isect] = (TH2I*) inputFile->Get(Form("%s_wByQQ_s%d",name.c_str(),isect));
    }
  } else {
    cout << " Error opening file... " << filename << endl; 
  }

  //  file->Close();
   
}

void BaseDISHistograms::Fill(PhysicsEvent event, int sector){
  // We assume that nobody sends us sector > 6 or sector < 1
  xByQQ[0]      ->Fill(event.x, event.qq);
  xByQQ[sector] ->Fill(event.x, event.qq);
  wByQQ[0]      ->Fill(event.w, event.qq);
  wByQQ[sector] ->Fill(event.w, event.qq);
}

void BaseDISHistograms::FillxByQQ(double x, double qq, int sector){
  xByQQ[0]      ->Fill(x, qq);
  xByQQ[sector] ->Fill(x, qq);
}

void BaseDISHistograms::FillwByQQ(double w, double qq, int sector){
  wByQQ[0]      ->Fill(w, qq);
  wByQQ[sector] ->Fill(w, qq);
}

void BaseDISHistograms::Rebin2D(int xFactor, int yFactor){
  for (int ihist = 0; ihist < 7; ihist++) {
    xByQQ[ihist]->Rebin2D(xFactor, yFactor);
    wByQQ[ihist]->Rebin2D(xFactor, yFactor); 
  }
}

#endif