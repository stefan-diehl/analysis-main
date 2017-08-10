#include <iostream>

#include "common/Config.h"

#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"

#include "DBins.h"

int main(int argc, char *argv[]){

  if (argc == 2){
    std::string configFile(argv[1]);
    Config testConfig = (Config) ConfigLoader::getConfiguration(configFile); 

    std::cout << "Loaded file " << configFile << std::endl; 
    std::cout << "Configuration name: " << testConfig.name << std::endl; 
    std::cout << "Number of files to be processed: " << testConfig.filesToProcess << std::endl; 
    std::cout << "Found binning schemes: " << std::endl; 

    for (std::pair<std::string, std::vector<double>> bin : testConfig.bins){
      std::cout << bin.first << ", " << bin.second.size() << std::endl; 
    }

    std::cout << "\nFound cuts: " << std::endl; 
    
    for(std::pair<std::string, std::map<std::string, double>> cut : testConfig.cuts){
      std::cout << cut.first << std::endl; 
      
      for(std::pair<std::string, double> param : cut.second){
	std::cout << param.first << ":" << param.second << std::endl; 
      }

      std::cout << std::endl; 
    }


    for (std::pair<std::string, DLineBins> ax : testConfig.axes){ 
      std::cout << ax.second.GetMax() << std::endl; 
    }

  }

  return 0; 
}
