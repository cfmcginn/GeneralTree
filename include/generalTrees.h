#ifndef GENERALTREES_H
#define GENERALTREES_H

#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TDirectory.h"
#include "TObjArray.h"
#include "TMath.h"

#include "include/doGlobalDebug.h"
#include "include/generalTree.h"
#include "include/returnRootFileContentsList.h"
#include "include/stringToBool.h"

class generalTrees
{
 public:
  generalTrees(std::string inConfigFileName);
  ~generalTrees(){};

  std::string configFileName;
  std::vector< generalTree* > generalTreeVect;
  std::vector< std::string > treeList;
  std::vector< std::vector<std::string> > varList;
  std::vector< std::vector<std::string> > cutListAND;
  std::vector< std::vector<std::string> > cutListOR;
  std::vector< Int_t > treeNEntries;
  std::vector< Int_t > uniqueTreeNEntries;
  
  Int_t maxNEntries = -1;
  Bool_t keepAllTrees = true;

  Bool_t globalOR = true;

  std::vector<TTree*> treeVect;

  TFile* outFile_p;

  void addTrees(TFile* inFile_p, TFile* inOutFile_p);
  void processTrees();
  void writeTrees();
  void clean();
  void printTreesAndBranches();
};

generalTrees::generalTrees(std::string inConfigFileName)
{
  configFileName = inConfigFileName;

  std::ifstream file(configFileName.c_str());
  std::string tempStr;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  while(std::getline(file, tempStr)){
    while(tempStr.find(" ") != std::string::npos){tempStr.replace(tempStr.find(" "), 1, "");}
    if(tempStr.size() == 0) continue;
    if(tempStr.substr(0,1).find("#") != std::string::npos) continue;//allow for comments
				

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;


    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(tempStr.find("GLOBAL:") != std::string::npos){
      tempStr.replace(0, tempStr.find(":")+1, "");
      
      while(tempStr.size() != 0){
	if(tempStr.substr(0, tempStr.find("=")).find("maxNEntries") != std::string::npos) maxNEntries = std::stoi(tempStr.substr(tempStr.find("=")+1, tempStr.find(",")-(tempStr.find("=")+1)));

	if(tempStr.substr(0, tempStr.find("=")).find("keepAllTrees") != std::string::npos) keepAllTrees = stringToBool(tempStr.substr(tempStr.find("=")+1, tempStr.find(",")-(tempStr.find("=")+1)));

	if(tempStr.find(",") != std::string::npos) tempStr.replace(0, tempStr.find(",")+1, "");
	else tempStr = "";
      }

      continue;
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    std::string treeName = tempStr.substr(0,tempStr.find(":"));
    treeList.push_back(treeName);
    tempStr.replace(0, tempStr.find(":")+1, "");
    std::string cutStrAND = "";
    std::string cutStrOR = "";
    if(tempStr.find(";") != std::string::npos){
      cutStrAND = tempStr.substr(tempStr.find(";")+1, tempStr.size() - (tempStr.find(";")+1));
      tempStr.replace(tempStr.find(";"), tempStr.size() - (tempStr.find(";")), "");
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  
    if(cutStrAND.find(";") != std::string::npos){
      cutStrOR = cutStrAND.substr(cutStrAND.find(";")+1, cutStrAND.size() - (cutStrAND.find(";")+1));
      cutStrAND.replace(cutStrAND.find(";"), cutStrAND.size() - (cutStrAND.find(";")+1), "");
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(cutStrAND.size() != 0){
      while(cutStrAND.substr(cutStrAND.size()-1,1).find(";") != std::string::npos){
	cutStrAND.replace(cutStrAND.size()-1,1,"");
	if(cutStrAND.size() == 0) break;
      }
    }

    if(cutStrOR.size() != 0){
      while(cutStrOR.substr(cutStrOR.size()-1,1).find(";") != std::string::npos){
	cutStrOR.replace(cutStrOR.size()-1,1,"");
	if(cutStrOR.size() == 0) break;
      }
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (cutStrAND==\'" << cutStrAND << "\')" << std::endl;
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (cutStrOR==\'" << cutStrOR << "\')" << std::endl;

    std::vector<std::string> tempVect;
    while(tempStr.find(",") != std::string::npos){
      std::string varStr = tempStr.substr(0, tempStr.find(","));
      tempVect.push_back(varStr);
      tempStr.replace(0, tempStr.find(",")+1, "");
    }
    if(tempStr.size() != 0) tempVect.push_back(tempStr);

    varList.push_back(tempVect);

    tempVect.clear();
    while(cutStrAND.find("&&") != std::string::npos){
      std::string varStr = cutStrAND.substr(0, cutStrAND.find("&&"));
      tempVect.push_back(varStr);
      cutStrAND.replace(0, cutStrAND.find("&&")+2, "");
    }
    if(cutStrAND.size() != 0) tempVect.push_back(cutStrAND);

    cutListAND.push_back(tempVect);

    tempVect.clear();
    while(cutStrOR.find("||") != std::string::npos){
      std::string varStr = cutStrOR.substr(0, cutStrOR.find("||"));
      tempVect.push_back(varStr);
      cutStrOR.replace(0, cutStrOR.find("||")+2, "");
    }
    if(cutStrOR.size() != 0) tempVect.push_back(cutStrOR);

    cutListOR.push_back(tempVect);

    if(tempVect.size() != 0) globalOR = false;
  }

  file.close();

  return;
}

void generalTrees::addTrees(TFile* inFile_p, TFile* inOutFile_p)
{
  outFile_p = inOutFile_p;
  std::vector<std::string> listOfTrees = returnRootFileContentsList(inFile_p, "TTree");

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

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

  for(unsigned int tI = 0; tI < listOfTrees.size(); ++tI){
    std::cout << tI << "/" << listOfTrees.size() << ": " << listOfTrees.at(tI) << std::endl;
  }

  if(keepAllTrees){
    for(unsigned int tI = 0; tI < listOfTrees.size(); ++tI){
      bool isInList = false;
      for(unsigned int tI2 = 0; tI2 < treeList.size(); ++tI2){
	if(listOfTrees.at(tI).size() == treeList.at(tI2).size() && treeList.at(tI2).find(listOfTrees.at(tI)) != std::string::npos){
	  isInList = true;
	  break;
	}
      }

      if(!isInList){
	treeList.push_back(listOfTrees.at(tI));
	varList.push_back({});
	cutListAND.push_back({});
	cutListOR.push_back({});
      }
    }
  }

  unsigned int tI = 0;
  while(treeList.size() > tI){
    bool isGood = false;

    for(unsigned int tI2 = 0; tI2 < listOfTrees.size(); ++tI2){
      if(listOfTrees.at(tI2).size() == treeList.at(tI).size() && treeList.at(tI).find(listOfTrees.at(tI2)) != std::string::npos){
	isGood = true;
	break;
      }
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(isGood) tI++;
    else{
      std::cout << "Warning: Input \'" << treeList.at(tI) << "\' is not found in file \'" << inFile_p->GetName() << "\'." << std::endl;
      std::cout << " Options: ";
      for(unsigned int tI2 = 0; tI2 < listOfTrees.size()-1; ++tI2){
	std::cout << "\'" << listOfTrees.at(tI2) << "\', ";
      }
      std::cout << "\'" << listOfTrees.at(listOfTrees.size()-1) << "\'." << std::endl;
      treeList.erase(treeList.begin()+tI);
      varList.erase(varList.begin()+tI);
      cutListAND.erase(cutListAND.begin()+tI);
      cutListOR.erase(cutListOR.begin()+tI);
    }
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(unsigned int tI = 0; tI < treeList.size(); ++tI){
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << treeList.at(tI) << std::endl;

    std::string dirPath = treeList.at(tI);
    while(dirPath.substr(dirPath.size()-1,1).find("/") == std::string::npos){
      dirPath.replace(dirPath.size()-1,1,"");
      if(dirPath.size() == 0) break;
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    treeVect.push_back((TTree*)inFile_p->Get(treeList.at(tI).c_str()));
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
    generalTreeVect.push_back(0);
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
    generalTreeVect.at(generalTreeVect.size()-1) = new generalTree(treeVect.at(treeVect.size()-1), outFile_p, dirPath, varList.at(tI), cutListAND.at(tI), cutListOR.at(tI));

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    treeNEntries.push_back(treeVect.at(tI)->GetEntries());

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    bool isUnique = true;
    for(unsigned int j = 0; j < uniqueTreeNEntries.size(); ++j){
      if(uniqueTreeNEntries.at(j) == treeNEntries.at(tI)){
        isUnique = false;
        break;
      }
    }
    if(isUnique) uniqueTreeNEntries.push_back(treeNEntries.at(tI));
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  return;
}

void generalTrees::processTrees()
{
  std::cout << "Begin processing..." << std::endl;
  for(unsigned int j = 0; j < uniqueTreeNEntries.size(); ++j){
    const Int_t uniqueCurrNEntries = uniqueTreeNEntries.at(j);
    Int_t tempCurrNEntries = uniqueTreeNEntries.at(j);
    if(maxNEntries > 0) tempCurrNEntries = TMath::Min(tempCurrNEntries, maxNEntries);
    const Int_t currNEntries = tempCurrNEntries;

    std::cout << "Processing trees w/ entries " << currNEntries << std::endl;

    Int_t divEntry = TMath::Max(1, currNEntries/20);

    for(Int_t entry = 0; entry < currNEntries; ++entry){
      if(entry%divEntry == 0) std::cout << " Entry " << entry << "/" << currNEntries << std::endl;

      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

      bool evtPassesAND = true;
      bool evtPassesOR = globalOR;
      for(unsigned int i = 0; i < treeList.size(); ++i){
        if(treeNEntries.at(i) != uniqueCurrNEntries) continue;
        treeVect.at(i)->GetEntry(entry);
	if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << i << std::endl;
	evtPassesAND = evtPassesAND && generalTreeVect.at(i)->eventPassesAND();
	evtPassesOR = evtPassesOR || generalTreeVect.at(i)->eventPassesOR();
      }

      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
      
      if(!evtPassesAND) continue;
      if(!evtPassesOR) continue;

      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

      for(unsigned int i = 0; i < treeList.size(); ++i){
	if(treeNEntries.at(i) != uniqueCurrNEntries) continue;
        generalTreeVect.at(i)->fill();
      }
    }
  }

  return;
}

void generalTrees::writeTrees()
{
  std::cout << "Writing..." << std::endl;
  for(unsigned int i = 0; i < generalTreeVect.size(); ++i){
    outFile_p->cd();
    generalTreeVect.at(i)->write();
  }

  return;
}

void generalTrees::clean()
{
  for(unsigned int i = 0; i < generalTreeVect.size(); ++i){
    generalTreeVect.at(i)->clean();
    delete generalTreeVect.at(i);
  }

  generalTreeVect.clear();

  return;
}

void generalTrees::printTreesAndBranches()
{
  for(unsigned int tI = 0; tI < treeList.size(); ++tI){
    std::cout << tI << "/" << treeList.size() << ", \'" << treeList.at(tI) << "\': ";

    for(int vI = 0; vI < (int)varList.at(tI).size()-1; ++vI){
      std::cout << varList.at(tI).at(vI) << ", ";
    }
    if(varList.at(tI).size() != 0) std::cout << varList.at(tI).at(varList.at(tI).size()-1);
    std::cout << "." << std::endl;

    std::cout << " Cuts(AND): ";
    for(int vI = 0; vI < (int)cutListAND.at(tI).size()-1; ++vI){
      std::cout << cutListAND.at(tI).at(vI) << ", ";
    }
    if(cutListAND.at(tI).size() != 0) std::cout << cutListAND.at(tI).at(cutListAND.at(tI).size()-1);
    std::cout << "." << std::endl;

    std::cout << " Cuts(OR): ";
    for(int vI = 0; vI < (int)cutListOR.at(tI).size()-1; ++vI){
      std::cout << cutListOR.at(tI).at(vI) << ", ";
    }
    if(cutListOR.at(tI).size() != 0) std::cout << cutListOR.at(tI).at(cutListOR.at(tI).size()-1);
    std::cout << "." << std::endl;
  }

  return;
}

#endif
