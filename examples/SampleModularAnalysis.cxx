#include <iostream>
#include <vector>
#include <map>
using namespace std;

// My Includes 
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"
#include "ElectronIDPackage.h"
#include "h22Reader.h"
#include "h22Event.h"


// This Analysis 
class SampleAnalysis 
{
public:
  SampleAnalysis();
  ~SampleAnalysis();

public:
  h22Reader * fReader; 
  void loop();
  void add_file(string); 
};

SampleAnalysis::SampleAnalysis()
{
  // Nothing to do.
  fReader = new h22Reader(0);
  fReader->Init();
}

SampleAnalysis::~SampleAnalysis()
{
  // Nothing to do. 
}

void SampleAnalysis::add_file(string file)
{
  fReader->AddFile(file); 
  //  cout << " Added file with runno " << fReader->runno() << endl; 
}

void SampleAnalysis::loop()
{

  ElectronSelector sel("epars.dat"); 
  sel.disable_all();
  sel.enable_by_name("EC-U Cut");
  
  //sel.set_runno( fReader->runno() );
  //sel.set_mc_status( false );
  sel.set_info( fReader->runno(), false);
  
  int n_electrons = 0;

  // Loop over events. 
  for (int ievent=0; ievent< fReader->GetEntries(); ievent++)
    {
      fReader->GetEntry(ievent);
      DEvent event( fReader->GetEvent() );
      if (sel.passes(event, 0)) n_electrons++; // Increments the counters 
      
    }

  sel.summarize();
  cout << " Total electrons: " << n_electrons << endl; 
  
}



int main(int argc, char * argv[])
{
  
  SampleAnalysis analysis;
  
  for (int iarg=1; iarg<argc; iarg++) { analysis.add_file(argv[iarg]); } 
  analysis.loop();
  
  return 0;
}