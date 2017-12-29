#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TDatime.h"
#include "TObjArray.h"

#include "include/returnRootFileContentsList.h"
#include "include/generalTree.h"
#include "include/doGlobalDebug.h"

int forestJetSkim(const std::string inFileName, const std::string inOutFileName = "")
{
  TDatime* date = new TDatime();
  std::string tempFileName = inOutFileName;
  if(inOutFileName.size() == 0){
    tempFileName = inFileName;
    while(tempFileName.find("/") != std::string::npos){tempFileName.replace(0, tempFileName.find("/")+1, "");}
    if(tempFileName.find(".root") != std::string::npos) tempFileName.replace(tempFileName.find(".root"), 5, ("_SKIM_" + std::to_string(date->GetDate()) + ".root").c_str());
  }
  const std::string outFileName = tempFileName;

  TFile* inFile_p = new TFile(inFileName.c_str(), "READ");  
  std::vector<std::string> listOfTrees = returnRootFileContentsList(inFile_p, "TTree");

  unsigned int pos = 0;
  while(pos < listOfTrees.size()){
    bool cont = true;

    for(unsigned int iter = pos+1; iter < listOfTrees.size(); ++iter){
      if(listOfTrees.at(pos).size() == listOfTrees.at(iter).size() && listOfTrees.at(pos).find(listOfTrees.at(iter)) != std::string::npos){
	listOfTrees.erase(listOfTrees.begin()+iter);
	cont = false;
	break;
      }
    }
    if(cont) ++pos;
  }

  std::vector< Int_t > treeNEntries;
  std::vector< Int_t > uniqueTreeNEntries;
  std::vector< TTree* > inTrees_p;
  std::vector< generalTree* > generalTrees;
  inTrees_p.reserve(listOfTrees.size());
  generalTrees.reserve(listOfTrees.size());

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(unsigned int i = 0; i < listOfTrees.size(); ++i){
    inTrees_p.push_back((TTree*)inFile_p->Get(listOfTrees.at(i).c_str()));
    treeNEntries.push_back(inTrees_p.at(i)->GetEntries());

    bool isUnique = true;
    for(unsigned int j = 0; j < uniqueTreeNEntries.size(); ++j){
      if(uniqueTreeNEntries.at(j) == treeNEntries.at(i)){
	isUnique = false;
	break;
      }
    }
    if(isUnique) uniqueTreeNEntries.push_back(inTrees_p.at(i)->GetEntries());

    generalTrees.push_back(0);
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  TFile* outFile_p = new TFile(outFileName.c_str(), "RECREATE");

  for(unsigned int i = 0; i < inTrees_p.size(); ++i){
    std::string dirPath = listOfTrees.at(i);
    while(dirPath.substr(dirPath.size()-1,1).find("/") == std::string::npos) dirPath.replace(dirPath.size()-1,1,"");

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << dirPath << std::endl;
    generalTrees.at(i) = new generalTree(inTrees_p.at(i), outFile_p, dirPath, {"hiBin", "vz", "nref", "jtpt", "jtphi", "jteta", "refpt", "refphi", "refeta"});
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(unsigned int i = 0; i < generalTrees.size(); ++i){
    //    generalTrees.at(i)->printBranchFullInfo();
  }


  std::cout << "Begin processing..." << std::endl;
  for(unsigned int j = 0; j < uniqueTreeNEntries.size(); ++j){
    const Int_t currNEntries = uniqueTreeNEntries.at(j);

    std::cout << "Processing trees w/ entries " << currNEntries << std::endl;

    for(Int_t entry = 0; entry < currNEntries; ++entry){
      if(entry%10000 == 0) std::cout << " Entry " << entry << "/" << currNEntries << std::endl;

      for(unsigned int i = 0; i < listOfTrees.size(); ++i){
	//	std::cout << "Processing " << listOfTrees.at(i) << "..." << std::endl;

	if(treeNEntries.at(i) != currNEntries) continue;

	if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	if(doGlobalDebug) std::cout << inTrees_p.at(i)->GetName() << std::endl;
	//	if(doGlobalDebug) inTrees_p.at(i)->Print();
	
	inTrees_p.at(i)->GetEntry(entry);

	if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	//	std::cout << inTrees_p.at(i)->GetName() << std::endl;
	generalTrees.at(i)->fill();

	if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
      }
    }
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  std::cout << "Writing..." << std::endl;
  for(unsigned int i = 0; i < generalTrees.size(); ++i){
    outFile_p->cd();
    //    generalTrees.at(i)->printBranchFullInfo();
    generalTrees.at(i)->write();
    generalTrees.at(i)->clean();
    delete (generalTrees.at(i));
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  outFile_p->Close();
  delete outFile_p;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  inFile_p->Close();
  delete inFile_p;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  delete date;

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 2 && argc != 3){
    std::cout << "Usage: ./forestJetSkim.exe <inFileName> <outFileName-optional>" << std::endl;
    return 1;
  }

  int retVal = 0;
  if(argc == 2) retVal += forestJetSkim(argv[1]);
  if(argc == 3) retVal += forestJetSkim(argv[1], argv[2]);
  return retVal;
}
