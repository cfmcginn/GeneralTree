#include <iostream>
#include <string>

#include "TFile.h"
#include "TDatime.h"
#include "TNamed.h"

#include "include/doGlobalDebug.h"
#include "include/generalTrees.h"
#include "include/returnRootFileContentsList.h"

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
  std::vector<std::string> tnameds = returnRootFileContentsList(inFile_p, "TNamed", "");
  std::vector<TNamed*> tnameds2;
  std::vector<std::string> tnamedDir;

  inFile_p->cd();

  if(doGlobalDebug) std::cout << "Listing tnameds..." << std::endl;
  for(unsigned int i = 0; i < tnameds.size(); ++i){
    if(doGlobalDebug) std::cout << " " << i << "/" << tnameds.size() << ": " << tnameds.at(i) << std::endl;
    tnameds2.push_back(0);
    tnameds2.at(i) = (TNamed*)inFile_p->Get(tnameds.at(i).c_str());
    
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
    std::string tempTNamed = tnameds.at(i);
    while(tempTNamed.substr(tempTNamed.size()-1, 1).find("/") == std::string::npos){
      tempTNamed.replace(tempTNamed.size()-1, 1, "");
      if(tempTNamed.size() == 0) break;

      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << tempTNamed << std::endl;
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(tempTNamed.size() != 0) tnamedDir.push_back(tempTNamed);
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  test.addTrees(inFile_p, outFile_p);
  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  test.processTrees();
  test.writeTrees();
  test.clean();

  outFile_p->cd();

  for(unsigned int i = 0; i < tnamedDir.size(); ++i){
    outFile_p->cd();
    outFile_p->mkdir(tnamedDir.at(i).c_str());
  }

  for(unsigned int i = 0; i < tnameds.size(); ++i){
    outFile_p->cd();
    for(unsigned int j = 0; j < tnamedDir.size(); ++j){
      if(tnameds.at(i).find(tnamedDir.at(j)) != std::string::npos){
	outFile_p->cd(tnamedDir.at(j).c_str());
	break;
      }
    }

    tnameds2.at(i)->Write("", TObject::kOverwrite);
  }

  inFile_p->cd();
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
