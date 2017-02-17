#include <iostream>
using namespace std;

#include "KinematicTree.h"
#include "KinematicTree.cxx"
#include "h22Option.h"

int main(int argc, char *argv[]){

  h22Options opts; 

  opts.set(argc, argv);

  if (argc < 2){ return 0; }

  KinematicTree treeWriter; 
  for (int i=0; i<opts.ifiles.size(); ++i) { treeWriter.AddFile( opts.ifiles[i] ); }
  //  treeWriter.isMonteCarlo = true; 
  treeWriter.SetupTree();
  treeWriter.RunAnalysis(opts.args["N"].arg);
  treeWriter.Save(opts.args["OUT"].args); 

  return 0;
}