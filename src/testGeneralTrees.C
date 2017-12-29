#include <iostream>
#include <string>

#include "TFile.h"
#include "TDatime.h"

#include "include/doGlobalDebug.h"
#include "include/generalTrees.h"

int testGeneralTrees(std::string inFileName, std::string inConfigFileName, std::string inOutFileName = "")
{
  TDatime* date = new TDatime();
  std::string tempFileName = inOutFileName;
  if(inOutFileName.size() == 0){
    tempFileName = inFileName;
    while(tempFileName.find("/") != std::string::npos){tempFileName.replace(0, tempFileName.find("/")+1, "");}
    if(tempFileName.find(".root") != std::string::npos) tempFileName.replace(tempFileName.find(".root"), 5, ("_SKIM_" + std::to_string(date->GetDate()) + ".root").c_str());
  }
  const std::string outFileName = tempFileName;

  TFile* outFile_p = new TFile(outFileName.c_str(), "RECREATE");

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  generalTrees test(inConfigFileName);
  test.printTreesAndBranches();

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  TFile* inFile_p = new TFile(inFileName.c_str(), "READ");
  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  test.addTrees(inFile_p, outFile_p);
  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  test.processTrees();
  test.writeTrees();
  test.clean();
  inFile_p->Close();
  delete inFile_p;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  outFile_p->Close();
  delete outFile_p;
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 3 && argc != 4){
    std::cout << "Usage: ./testGeneralTrees.exe <inFileName> <inConfigFileName> <outFileName-optional>" << std::endl;
    return 1;
  }

  int retVal = 0;
  if(argc == 3) retVal += testGeneralTrees(argv[1], argv[2]);
  else if(argc == 4) retVal += testGeneralTrees(argv[1], argv[2], argv[3]);
  return retVal;
}
