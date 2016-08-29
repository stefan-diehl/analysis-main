////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 5, 2016 

  DBins.h -> Shamelessly using my name for classes now. 

*/
////////////////////////////////////////////////////

#ifndef dbins_h
#define dbins_h

// c++ includes
#include <iostream>
#include <vector>

// root includes

// my includes

class DBins
{
  //Default Constr. Destr.
 public:
  DBins();
  ~DBins();

  DBins(int, double, double);

  // Datatypes
 private:
  int kNumber;
  double kMin, kMax, kWidth;
  std::vector<double> kBins;

  // Member functions 
 public:
  void refresh();
  void operator=(const DBins *);
  void operator=(DBins);
    
  // Setters
  void set_min(double);
  void set_max(double);
  void set_number(int);

  // Getters
  int number(){ return kNumber; }
  int find_bin(double);
  double min(){ return kMin; }
  double max(){ return kMax; }
  double width(){ return kWidth; }
  double bin_center(int);
  std::vector<double> get_bins(){ return kBins; } 
  
};

// For making asymmetric binning schemes 
class DBinsAsymmetric
{
 public:
  DBinsAsymmetric();
  ~DBinsAsymmetric();

  std::vector<double> bins;

  int find_bin(double);
  int number(){ return bins.size()-1; }

  void add_bin_edge(double bin){ bins.push_back(bin); } 

  double width(int);
  double center(int);
  double min(int);
  double max(int);
  
};

// Generic Bin Class 
class DBin
{
 public:
  DBin();
  ~DBin();

  inline DBin(double min, double max){ kMin = min; kMax = max; Refresh(); }
  
  // Data Types
 private:
  int kFills;
  double kWidth, kMin, kMax, kSysError, kStatError, kScale; 

  // Member Functions 
 public:
  void Clear(); 
  void Refresh(); 
  void SetMin(double min){ kMin = min; Refresh(); }
  void SetMax(double max){ kMax = max; Refresh(); }
  void SetStatError(double err){ kStatError = err; }
  void SetSysError(double err){ kSysError = err; }
  void CalcStatError();
  void Fill(){ kFills++; } 
  void SetScale(double s){ kScale = s; }
  void Print(); 
  
  int GetFills(){ return kFills; } 
  double GetWidth(){ return kWidth; }
  double GetMin(){ return kMin; }
  double GetMax(){ return kMax; }
  double GetSysError(){ return kSysError; }
  double GetStatError(){ return kStatError; }
  double GetScale(){ return kScale; }
  double GetContent(){ return double(kScale*kFills); }
  
};

// Line of DBin Object Pointers 
class DLineBins
{
 public:
  DLineBins();
  ~DLineBins();

  // Data Types
 private:
  int kUnderflow, kOverflow, kNumber; 
  vector<DBin> bins; 
  
  
  // Member Functions 
 public:
  void AddBin(DBin b){ bins.push_back(b); kNumber++; }
  void Clear(){ kUnderflow=0; kOverflow=0; kNumber=0; for(int i=0; i<bins.size(); i++) bins[i].Clear(); }
  void Fill(double); 
  void Refresh(){ for(int i=0; i<bins.size(); i++) bins[i].Refresh(); }
  void Print(); 
  int FindBin(double);
  DBin GetBin(int i){ return bins[i]; }
  int GetNumber(){ return kNumber; } 
  int GetUnderflow(){ return kUnderflow; }
  int GetOverflow(){ return kOverflow; }
};

// Plane of Bins
class DPlaneBins
{
 public:
  DPlaneBins();
  ~DPlaneBins(); 

 private:
  DLineBins edge_bins;
  vector<DLineBins> bins; 

 public:
  void SetEdgeBins(DLineBins b){ edge_bins = b; }
  void AddLineBins(DLineBins); /**< Add your line of bins with corresponding edge bin number */
  void Fill(double, double);
  void Print();
  
};

#endif

