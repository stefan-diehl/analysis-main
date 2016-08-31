#include <iostream>
#include <vector>
using namespace std;

#include "DBins.h"
#include "TRandom3.h"

int main()
{

  cout << " Bin Example! " << endl; 

  TRandom3 * rand = new TRandom3(); 

  // Demonstrates how to make symmetric bins.  
  DLineBins xBins(20, 0, 0.6); 

  // Do a little test fill 
  for (int i=0; i<10000; i++){
    xBins.Fill( rand->Uniform(1) );
  }

  // Print your bin content out. 
  xBins.Print(); 
  
  // Do an asymmetric rebinning of the xBins into 4 big bins. 
  vector<double> xLimits;
  xLimits.push_back(0.1);
  xLimits.push_back(0.2);
  xLimits.push_back(0.4);
  xLimits.push_back(0.6); 
  int n_x_rebins = xLimits.size()-1; 

  // Get the new bins
  DLineBins newXBins = xBins.Rebin(n_x_rebins, xLimits); 
  newXBins.Print(); 

  // Showing how to initialize empty bins with the structure of the newXBins
  DLineBins emptyNewXBins(n_x_rebins, xLimits);
  emptyNewXBins.Print(); 

  // Setup new bins for testing of Statistical Rebinning 
  DLineBins yBins(50, -5, 5);

  for (int i=0; i<10000; i++) {
    yBins.Fill( rand->Gaus(0, 2.9) );
  }

  cout << " Statistical Rebin.. " << endl; 
  yBins.Print();

  for (int i=0; i<10; i++){ 
    yBins.SetStatRebinThreshold((double)i*0.02); 
    DLineBins newYBins = yBins.StatisticalRebin(4); 
    cout << " Threshold: " << (double) i*0.02 << endl; 
    newYBins.Print();
  }
  
  return 0;
}