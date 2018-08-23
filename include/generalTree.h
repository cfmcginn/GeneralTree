#ifndef GENERALTREE_H
#define GENERALTREE_H

#include <vector>
#include <string>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLeaf.h"
#include "TDirectory.h"
#include "TObjArray.h"

#include "include/doGlobalDebug.h"

class generalTree
{
 public:
  generalTree(TTree* inInTree_p, TFile* inOutFile_p, const std::string inDirPath, std::vector<std::string> inVarList, std::vector<std::string> inCutListAND, std::vector<std::string> inCutListOR, const bool inDocCuts);
  ~generalTree(){};

  bool docCuts;
  std::string numStr = "0123456789";

  std::vector<std::string> branchNames;
  std::vector<std::string> branchDT;
  std::vector<bool> branchIsArr;
  std::vector<std::string> branchArrPeg;
  std::vector<bool> branchIsVect;
  std::vector<bool> branchIsValid;

  std::map<std::string, Int_t> branchArrPegMax;

  std::string dirPath;
  TTree* inTree_p=0;
  TFile* outFile_p=0;
  TTree* outTree_p=0;
  
  std::vector<std::string> varList;
  std::vector<std::string> cutListAND;
  std::vector< std::vector<std::string> > cutSubListAND;
  std::vector<std::string> cutListOR;
  std::vector< std::vector<std::string> > cutSubListOR;

  std::vector<int> branchPos;

  static const Int_t nDataTypes = 33;
  std::string dataTypes[nDataTypes] = {"Char",
				       "Short",
				       "Int",
				       "Float",
				       "Double",
				       "Long64",
				       "UChar",
				       "UShort",
				       "UInt",
				       "ULong64",
				       "Bool",
				       "CharArr",
				       "ShortArr",
				       "IntArr",
				       "FloatArr",
				       "DoubleArr",
				       "Long64Arr",
				       "UCharArr",
				       "UShortArr",
				       "UIntArr",
				       "ULong64Arr",
				       "BoolArr",
				       "CharVect",
				       "ShortVect",
				       "IntVect",
				       "FloatVect",
				       "DoubleVect",
				       "Long64Vect",
				       "UCharVect",
				       "UShortVect",
				       "UIntVect",
				       "ULong64Vect",
				       "BoolVect"};

  std::map<std::string, Int_t> numPerDataType;

  std::vector< Char_t* > branchChars;
  std::vector< Short_t* > branchShorts;
  std::vector< Int_t* > branchInts;
  std::vector< Float_t* > branchFloats;
  std::vector< Double_t* > branchDoubles;
  std::vector< Long64_t* > branchLong64s;
  std::vector< UChar_t* > branchUChars;
  std::vector< UShort_t* > branchUShorts;
  std::vector< UInt_t* > branchUInts;
  std::vector< ULong64_t* > branchULong64s;
  std::vector< Bool_t* > branchBools;

  std::vector< Char_t* > branchCharArrs;
  std::vector< Short_t* > branchShortArrs;
  std::vector< Int_t* > branchIntArrs;
  std::vector< Float_t* > branchFloatArrs;
  std::vector< Double_t* > branchDoubleArrs;
  std::vector< Long64_t* > branchLong64Arrs;
  std::vector< UChar_t* > branchUCharArrs;
  std::vector< UShort_t* > branchUShortArrs;
  std::vector< UInt_t* > branchUIntArrs;
  std::vector< ULong64_t* > branchULong64Arrs;
  std::vector< Bool_t* > branchBoolArrs;

  std::vector< std::vector<int>* > branchVectInts;
  std::vector< std::vector<float>* > branchVectFloats;
  std::vector< std::vector<double>* > branchVectDoubles;

  bool eventPasses(const Bool_t isOr);
  bool eventPassesOR(){return eventPasses(true);}
  bool eventPassesAND(){return eventPasses(false);}
  bool evalStringCut(Char_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(Short_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(Int_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(Long64_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(Float_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(Double_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(UChar_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(UShort_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(UInt_t val, std::string cutType, std::string cutStr);
  bool evalStringCut(ULong64_t val, std::string cutType, std::string cutStr);
  void fill();
  void write();
  void clean();
  void printBranchNames();
  void printBranchFullInfo();
};


generalTree::generalTree(TTree* inInTree_p, TFile* inOutFile_p, const std::string inDirPath = "", std::vector<std::string> inVarList = {}, std::vector<std::string> inCutListAND = {}, std::vector<std::string> inCutListOR = {}, const bool inDocCuts = true)
{
  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  inTree_p = inInTree_p;
  dirPath = inDirPath;
  varList = inVarList;
  cutListAND = inCutListAND;
  cutListOR = inCutListOR;
  docCuts = inDocCuts;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(unsigned int i = 0; i < cutListAND.size(); ++i){
    std::string temp = cutListAND.at(i);
    std::cout << "TEST CUTS: " << temp << std::endl;
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (temp==\'" << temp << "\')" << std::endl;

    temp.replace(0,1,"");
    temp.replace(temp.size()-1,1,"");

    std::vector<std::string> tempVect;

    while(temp.find(",") != std::string::npos){
      tempVect.push_back(temp.substr(0, temp.find(",")));
      temp.replace(0, temp.find(",")+1, "");
    }
    tempVect.push_back(temp);

    cutSubListAND.push_back(tempVect);
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(unsigned int i = 0; i < cutListOR.size(); ++i){
    std::string temp = cutListOR.at(i);
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (temp==\'" << temp << "\')" << std::endl;

    temp.replace(0,1,"");
    temp.replace(temp.size()-1,1,"");

    std::vector<std::string> tempVect;

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    while(temp.find(",") != std::string::npos){
      tempVect.push_back(temp.substr(0, temp.find(",")));
      temp.replace(0, temp.find(",")+1, "");
    }
    tempVect.push_back(temp);

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    cutSubListOR.push_back(tempVect);
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  if(dirPath.size() != 0)
    if(dirPath.substr(dirPath.size()-1,1).find("/") != std::string::npos) dirPath.replace(dirPath.size()-1,1,"");

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  for(Int_t dI = 0; dI < nDataTypes; ++dI){
    numPerDataType[dataTypes[dI]] = 0;
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  TObjArray* tempArr_p = (TObjArray*)inTree_p->GetListOfBranches();
  TObjArray* tempArrLeaf_p = (TObjArray*)inTree_p->GetListOfLeaves();
  const Int_t nEntries = tempArr_p->GetEntries();

  branchNames.reserve(nEntries);
  branchPos.reserve(nEntries);
  branchDT.reserve(nEntries);
  branchIsArr.reserve(nEntries);
  branchArrPeg.reserve(nEntries);
  branchIsVect.reserve(nEntries);
  branchIsValid.reserve(nEntries);

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  
  for(int j = 0; j < nEntries; ++j){
    std::string branchName = tempArr_p->At(j)->GetName();

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(varList.size() != 0){
      bool isGoodVal = false;
      for(unsigned int vI = 0; vI < varList.size(); ++vI){
	if(varList.at(vI).size() == branchName.size() && branchName.find(varList.at(vI)) != std::string::npos){
	  isGoodVal = true;
	  break;
	}
      }

      if(!isGoodVal) continue;
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    branchNames.push_back(branchName);
    branchPos.push_back(-999);
    std::string tempType = tempArr_p->At(j)->GetTitle();

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << j << ": " << branchNames.at(branchNames.size()-1) << std::endl;

    TLeaf* tempLeaf = (TLeaf*)inTree_p->GetLeaf(tempArrLeaf_p->At(j)->GetName());
    std::string tempClassType = tempLeaf->GetTypeName();

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    int nArr = 0;
    std::string tempType2 = tempType;
    while(tempType2.find("[") != std::string::npos){
      ++nArr;
      tempType2.replace(0, tempType2.find("[")+1, "");
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(tempType.find("[") != std::string::npos && nArr < 2){
      branchIsArr.push_back(true);
      //      branchIsValid.push_back(true);
      std::string pegVal = tempType.substr(tempType.find("[")+1, tempType.find("]") - tempType.find("[") - 1);
      branchArrPeg.push_back(pegVal);
      if(branchArrPegMax.find(pegVal) == branchArrPegMax.end()){
	bool isNum = true;
	for(unsigned int nI = 0; nI < pegVal.size(); ++nI){if(numStr.find(pegVal.substr(nI, 1)) == std::string::npos){isNum = false; break;}}
	if(!isNum) branchArrPegMax[pegVal] = (Int_t)inTree_p->GetMaximum(pegVal.c_str());
	else branchArrPegMax[pegVal] = std::stoi(pegVal);
      }
    }
    else{
      branchIsArr.push_back(false);
      branchArrPeg.push_back("");
    }

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

    if(tempType.find("/") != std::string::npos && nArr < 2){
      branchDT.push_back(tempType.substr(tempType.find("/")+1, 1));
      branchIsValid.push_back(true);
      branchIsVect.push_back(false);

      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

      if(branchIsArr.at(branchIsArr.size()-1)){
	if(branchDT.at(branchDT.size()-1).find("I") != std::string::npos) ++(numPerDataType["IntArr"]);
	else if(branchDT.at(branchDT.size()-1).find("F") != std::string::npos) ++(numPerDataType["FloatArr"]);
	else if(branchDT.at(branchDT.size()-1).find("D") != std::string::npos) ++(numPerDataType["DoubleArr"]);
	else if(branchDT.at(branchDT.size()-1).find("B") != std::string::npos) ++(numPerDataType["CharArr"]);
	else if(branchDT.at(branchDT.size()-1).find("S") != std::string::npos) ++(numPerDataType["ShortArr"]);
	else if(branchDT.at(branchDT.size()-1).find("L") != std::string::npos) ++(numPerDataType["Long64Arr"]);
	else if(branchDT.at(branchDT.size()-1).find("i") != std::string::npos) ++(numPerDataType["UIntArr"]);
	else if(branchDT.at(branchDT.size()-1).find("b") != std::string::npos) ++(numPerDataType["UCharArr"]);
	else if(branchDT.at(branchDT.size()-1).find("s") != std::string::npos) ++(numPerDataType["UShortArr"]);
	else if(branchDT.at(branchDT.size()-1).find("l") != std::string::npos) ++(numPerDataType["ULong64Arr"]);
	else if(branchDT.at(branchDT.size()-1).find("O") != std::string::npos) ++(numPerDataType["BoolArr"]);
      }
      else{
	if(branchDT.at(branchDT.size()-1).find("I") != std::string::npos) ++(numPerDataType["Int"]);
	else if(branchDT.at(branchDT.size()-1).find("F") != std::string::npos) ++(numPerDataType["Float"]);
	else if(branchDT.at(branchDT.size()-1).find("D") != std::string::npos) ++(numPerDataType["Double"]);
	else if(branchDT.at(branchDT.size()-1).find("B") != std::string::npos) ++(numPerDataType["Char"]);
	else if(branchDT.at(branchDT.size()-1).find("S") != std::string::npos) ++(numPerDataType["Short"]);
	else if(branchDT.at(branchDT.size()-1).find("L") != std::string::npos) ++(numPerDataType["Long64"]);
	else if(branchDT.at(branchDT.size()-1).find("i") != std::string::npos) ++(numPerDataType["UInt"]);
	else if(branchDT.at(branchDT.size()-1).find("b") != std::string::npos) ++(numPerDataType["UChar"]);
	else if(branchDT.at(branchDT.size()-1).find("s") != std::string::npos) ++(numPerDataType["UShort"]);
	else if(branchDT.at(branchDT.size()-1).find("l") != std::string::npos) ++(numPerDataType["ULong64"]);
	else if(branchDT.at(branchDT.size()-1).find("O") != std::string::npos) ++(numPerDataType["Bool"]);
      }
    }
    else if(tempClassType.find("vector") != std::string::npos && tempClassType.find("vector<vector") == std::string::npos){
      if(doGlobalDebug) std::cout << " " << __FILE__ << ", " << __LINE__ << ", " << j << " (vector): " << branchNames.at(branchNames.size()-1) << std::endl;
      branchIsVect.push_back(true);
      branchIsValid.push_back(true);
      tempClassType.replace(0,tempClassType.find("<")+1,"");
      tempClassType.replace(tempClassType.find(">"), tempClassType.size()-tempClassType.find(">"), "");
      branchDT.push_back(tempClassType);

      if(tempClassType.find("int") != std::string::npos) ++(numPerDataType["IntVect"]);
      else if(tempClassType.find("float") != std::string::npos) ++(numPerDataType["FloatVect"]);
      else if(tempClassType.find("double") != std::string::npos) ++(numPerDataType["DoubleVect"]);
    
      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", " << branchNames.at(branchNames.size()-1) << std::endl;
      TH1F* temp_h = new TH1F("temp_h", "", 100000, -0.5, 99999.5);
      inTree_p->Draw(("@" + branchNames.at(branchNames.size()-1) + ".size() >> temp_h").c_str());
      branchArrPegMax[branchNames.at(branchNames.size()-1)] = (Int_t)(temp_h->GetBinLowEdge(temp_h->GetNbinsX()+1) + 1);
      if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
      delete temp_h;
    }
    else{
      branchDT.push_back("NA");
      branchIsVect.push_back(false);
      branchIsValid.push_back(false);

      std::cout << "WARNING: Branch \'" << branchNames.at(branchNames.size()-1) << "\' from tree \'" << inTree_p->GetName() << "\' is not valid. Skipping..." << std::endl;
    }
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  branchVectInts.reserve(numPerDataType["IntVect"]);
  branchVectFloats.reserve(numPerDataType["FloatVect"]);
  branchVectDoubles.reserve(numPerDataType["DoubleVect"]);

  branchChars.reserve(numPerDataType["Char"]);
  branchShorts.reserve(numPerDataType["Short"]);
  branchInts.reserve(numPerDataType["Int"]);
  branchFloats.reserve(numPerDataType["Float"]);
  branchDoubles.reserve(numPerDataType["Double"]);
  branchLong64s.reserve(numPerDataType["Long64"]);
  branchUChars.reserve(numPerDataType["UChar"]);
  branchUShorts.reserve(numPerDataType["UShort"]);
  branchUInts.reserve(numPerDataType["UInt"]);
  branchULong64s.reserve(numPerDataType["ULong64"]);
  branchBools.reserve(numPerDataType["Bool"]);

  branchCharArrs.reserve(numPerDataType["CharArr"]);
  branchShortArrs.reserve(numPerDataType["ShortArr"]);
  branchIntArrs.reserve(numPerDataType["IntArr"]);
  branchFloatArrs.reserve(numPerDataType["FloatArr"]);
  branchDoubleArrs.reserve(numPerDataType["DoubleArr"]);
  branchLong64Arrs.reserve(numPerDataType["Long64Arr"]);
  branchUCharArrs.reserve(numPerDataType["UCharArr"]);
  branchUShortArrs.reserve(numPerDataType["UShortArr"]);
  branchUIntArrs.reserve(numPerDataType["UIntArr"]);
  branchULong64Arrs.reserve(numPerDataType["ULong64Arr"]);
  branchBoolArrs.reserve(numPerDataType["BoolArr"]);

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  outFile_p = inOutFile_p;
  outFile_p->cd();
  if(dirPath.size() != 0){
    outFile_p->mkdir(dirPath.c_str());
    outFile_p->cd(dirPath.c_str());
  }
  outTree_p = new TTree(inTree_p->GetName(), "");

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", running tree " << inTree_p->GetName() << std::endl;

  inTree_p->SetBranchStatus("*", 0);

  for(unsigned int bI = 0; bI < branchNames.size(); ++bI){
    if(!branchIsValid.at(bI)) continue;

    if(!branchIsArr.at(bI) && !branchIsVect.at(bI)){
      if(branchDT.at(bI).find("B") != std::string::npos){
	branchChars.push_back(0);
	branchChars.at(branchChars.size()-1) = new Char_t();
	branchPos.at(bI) = (branchChars.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchChars.at(branchChars.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchChars.at(branchChars.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("S") != std::string::npos){
	branchShorts.push_back(0);
	branchShorts.at(branchShorts.size()-1) = new Short_t();
	branchPos.at(bI) = (branchShorts.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchShorts.at(branchShorts.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchShorts.at(branchShorts.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("I") != std::string::npos){
	branchInts.push_back(0);
	branchInts.at(branchInts.size()-1) = new Int_t();
	branchPos.at(bI) = (branchInts.size()-1);

	if(doGlobalDebug) std::cout << "branchInts: " << __FILE__ << ", " << __LINE__ << ", " << branchNames.at(bI) << ", " << bI << ", " << branchNames.size() <<  std::endl;
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchInts.at(branchInts.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchInts.at(branchInts.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("F") != std::string::npos){
	branchFloats.push_back(0);
	branchFloats.at(branchFloats.size()-1) = new Float_t();
	branchPos.at(bI) = (branchFloats.size()-1);

	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchFloats.at(branchFloats.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchFloats.at(branchFloats.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("D") != std::string::npos){
	branchDoubles.push_back(0);
	branchDoubles.at(branchDoubles.size()-1) = new Double_t();
	branchPos.at(bI) = (branchDoubles.size()-1);

	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchDoubles.at(branchDoubles.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchDoubles.at(branchDoubles.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("L") != std::string::npos){
	branchLong64s.push_back(0);
	branchLong64s.at(branchLong64s.size()-1) = new Long64_t();
	branchPos.at(bI) = (branchLong64s.size()-1);

	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchLong64s.at(branchLong64s.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchLong64s.at(branchLong64s.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      if(branchDT.at(bI).find("b") != std::string::npos){
	branchUChars.push_back(0);
	branchUChars.at(branchUChars.size()-1) = new UChar_t();
	branchPos.at(bI) = (branchUChars.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUChars.at(branchUChars.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUChars.at(branchUChars.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("s") != std::string::npos){
	branchUShorts.push_back(0);
	branchUShorts.at(branchUShorts.size()-1) = new UShort_t();
	branchPos.at(bI) = (branchUShorts.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUShorts.at(branchUShorts.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUShorts.at(branchUShorts.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("i") != std::string::npos){
	branchUInts.push_back(0);
	branchUInts.at(branchUInts.size()-1) = new UInt_t();
	branchPos.at(bI) = (branchUInts.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUInts.at(branchUInts.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUInts.at(branchUInts.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("l") != std::string::npos){
	branchULong64s.push_back(0);
	branchULong64s.at(branchULong64s.size()-1) = new ULong64_t();
	branchPos.at(bI) = (branchULong64s.size()-1);

	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchULong64s.at(branchULong64s.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchULong64s.at(branchULong64s.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("O") != std::string::npos){
	branchBools.push_back(0);
	branchBools.at(branchBools.size()-1) = new Bool_t();
	branchPos.at(bI) = (branchBools.size()-1);

	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchBools.at(branchBools.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchBools.at(branchBools.size()-1)), (branchNames.at(bI) + "/" + branchDT.at(bI)).c_str());
      }
    }
    else if(branchIsArr.at(bI)){
      //      const unsigned int maxVal = branchArrPegMax[branchArrPeg.at(bI)];

      //      std::cout << "Branch, max: " << branchNames.at(bI) << ", " << maxVal << std::endl;

      if(doGlobalDebug) std::cout << "Branch: " << branchNames.at(bI) << std::endl;

      if(branchDT.at(bI).find("B") != std::string::npos){
	branchCharArrs.push_back(0);
	branchCharArrs.at(branchCharArrs.size()-1) = new Char_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchCharArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchCharArrs.at(branchCharArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchCharArrs.at(branchCharArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("S") != std::string::npos){
	branchShortArrs.push_back(0);
	branchShortArrs.at(branchShortArrs.size()-1) = new Short_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchShortArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchShortArrs.at(branchShortArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchShortArrs.at(branchShortArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("I") != std::string::npos){
	branchIntArrs.push_back(0);
	branchIntArrs.at(branchIntArrs.size()-1) = new Int_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchIntArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchIntArrs.at(branchIntArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchIntArrs.at(branchIntArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("F") != std::string::npos){
	branchFloatArrs.push_back(0);
	branchFloatArrs.at(branchFloatArrs.size()-1) = new Float_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchFloatArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchFloatArrs.at(branchFloatArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchFloatArrs.at(branchFloatArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("D") != std::string::npos){
	branchDoubleArrs.push_back(0);
	branchDoubleArrs.at(branchDoubleArrs.size()-1) = new Double_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchDoubleArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchDoubleArrs.at(branchDoubleArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchDoubleArrs.at(branchDoubleArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("L") != std::string::npos){
	branchLong64Arrs.push_back(0);
	branchLong64Arrs.at(branchLong64Arrs.size()-1) = new Long64_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchLong64Arrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchLong64Arrs.at(branchLong64Arrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchLong64Arrs.at(branchLong64Arrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      if(branchDT.at(bI).find("b") != std::string::npos){
	branchUCharArrs.push_back(0);
	branchUCharArrs.at(branchUCharArrs.size()-1) = new UChar_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchUCharArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUCharArrs.at(branchUCharArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUCharArrs.at(branchUCharArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("s") != std::string::npos){
	branchUShortArrs.push_back(0);
	branchUShortArrs.at(branchUShortArrs.size()-1) = new UShort_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchUShortArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUShortArrs.at(branchUShortArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUShortArrs.at(branchUShortArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("i") != std::string::npos){
	branchUIntArrs.push_back(0);
	branchUIntArrs.at(branchUIntArrs.size()-1) = new UInt_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchUIntArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchUIntArrs.at(branchUIntArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchUIntArrs.at(branchUIntArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("l") != std::string::npos){
	branchULong64Arrs.push_back(0);
	branchULong64Arrs.at(branchULong64Arrs.size()-1) = new ULong64_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchULong64Arrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchULong64Arrs.at(branchULong64Arrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchULong64Arrs.at(branchULong64Arrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
      else if(branchDT.at(bI).find("O") != std::string::npos){
	branchBoolArrs.push_back(0);
	branchBoolArrs.at(branchBoolArrs.size()-1) = new Bool_t[branchArrPegMax[branchArrPeg.at(bI)]];
	branchPos.at(bI) = (branchBoolArrs.size()-1);
	
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), (branchBoolArrs.at(branchBoolArrs.size()-1)));
	outTree_p->Branch(branchNames.at(bI).c_str(), (branchBoolArrs.at(branchBoolArrs.size()-1)), (branchNames.at(bI) + "[" + branchArrPeg.at(bI) + "]/" + branchDT.at(bI)).c_str());
      }
    }
    else if(branchIsVect.at(bI)){
      if(branchDT.at(bI).find("int") != std::string::npos){
	branchVectInts.push_back(0);
	branchVectInts.at(branchVectInts.size()-1) = new std::vector<int>;
	branchVectInts.at(branchVectInts.size()-1)->reserve(branchArrPegMax[branchNames.at(bI)]);
	branchPos.at(bI) = (branchVectInts.size()-1);

	outTree_p->Branch(branchNames.at(bI).c_str(), (branchVectInts.at(branchVectInts.size()-1)));
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), &(branchVectInts[branchVectInts.size()-1]));
      }
      else if(branchDT.at(bI).find("float") != std::string::npos){
	branchVectFloats.push_back(0);
	branchVectFloats.at(branchVectFloats.size()-1) = new std::vector<float>;
	branchVectFloats.at(branchVectFloats.size()-1)->reserve(branchArrPegMax[branchNames.at(bI)]);
	branchPos.at(bI) = (branchVectFloats.size()-1);

	outTree_p->Branch(branchNames.at(bI).c_str(), (branchVectFloats.at(branchVectFloats.size()-1)));
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), &(branchVectFloats[branchVectFloats.size()-1]));
      }
      else if(branchDT.at(bI).find("double") != std::string::npos){
	branchVectDoubles.push_back(0);
	branchVectDoubles.at(branchVectDoubles.size()-1) = new std::vector<double>;
	branchVectDoubles.at(branchVectDoubles.size()-1)->reserve(branchArrPegMax[branchNames.at(bI)]);
	branchPos.at(bI) = (branchVectDoubles.size()-1);

	outTree_p->Branch(branchNames.at(bI).c_str(), (branchVectDoubles.at(branchVectDoubles.size()-1)));
	inTree_p->SetBranchStatus(branchNames.at(bI).c_str(), 1);
	inTree_p->SetBranchAddress(branchNames.at(bI).c_str(), &(branchVectDoubles[branchVectDoubles.size()-1]));
      }
    }
    else{
      branchPos.at(bI) = (-999);
    }

  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  return;
}

bool generalTree::eventPasses(const Bool_t isOr = false)
{
  bool evtPass = !isOr;

  std::vector< std::vector<std::string> > cutSubList = cutSubListAND;
  if(isOr) cutSubList = cutSubListOR;

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  if(cutSubList.size() == 0){
    if(!isOr) evtPass = true;
    else evtPass = false;
  }

  for(unsigned int cI = 0; cI < cutSubList.size(); ++cI){

    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (size==" << cutSubList.at(cI).size() << ")" << std::endl;

    std::string varToCut = cutSubList.at(cI).at(0);
    if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << ", (var1==\'" << varToCut << "\')" << std::endl;
    std::string cutSign = cutSubList.at(cI).at(1);


    int bPos = -1;
      
    for(int bI = 0; bI < (int)branchNames.size(); ++bI){
      if(varToCut.size() == branchNames.at(bI).size() && varToCut.find(branchNames.at(bI)) != std::string::npos){
	bPos = bI;
	break;
      }
    }

    if(bPos < 0){
      std::cout << "Warning: in tree \'" << dirPath << "\\" << inTree_p->GetName() << "\' given cutVar \'" << varToCut << "\' is not found during eventPasses(). continue" << std::endl;
      continue;
    }

    if(branchIsValid[bPos] && !branchIsArr[bPos] && !branchIsVect[bPos]){
      if(branchDT[bPos].find("B") != std::string::npos){
	Char_t tempVarVal = *branchChars.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("S") != std::string::npos){
	Short_t tempVarVal = *branchShorts.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("I") != std::string::npos){
	Int_t tempVarVal = *branchInts.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("L") != std::string::npos){
	Long64_t tempVarVal = *branchLong64s.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("F") != std::string::npos){
	Float_t tempVarVal = *branchFloats.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));

	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("D") != std::string::npos){
	Double_t tempVarVal = *branchDoubles.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("b") != std::string::npos){
	UChar_t tempVarVal = *branchUChars.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("s") != std::string::npos){
	UShort_t tempVarVal = *branchUShorts.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("i") != std::string::npos){
	UInt_t tempVarVal = *branchUInts.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
      else if(branchDT[bPos].find("l") != std::string::npos){
	ULong64_t tempVarVal = *branchULong64s.at(branchPos.at(bPos));
	bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	if(!isOr && !tempCutBool){evtPass = false; break;}
	else if(isOr && tempCutBool){evtPass = true; break;}
      }
    }
    else if(branchIsVect[bPos]){
      bool doBreak = false;
      
      if(branchDT[bPos].find("I") != std::string::npos){
	for(unsigned int pI = 0; pI < branchVectInts.at(branchPos.at(bPos))->size(); ++pI){
	  Int_t tempVarVal = branchVectInts.at(branchPos.at(bPos))->at(pI);
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("F") != std::string::npos){
	for(unsigned int pI = 0; pI < branchVectFloats.at(branchPos.at(bPos))->size(); ++pI){
	  Float_t tempVarVal = branchVectFloats.at(branchPos.at(bPos))->at(pI);
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("D") != std::string::npos){
	for(unsigned int pI = 0; pI < branchVectDoubles.at(branchPos.at(bPos))->size(); ++pI){
	  Double_t tempVarVal = branchVectDoubles.at(branchPos.at(bPos))->at(pI);
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }

      if(doBreak) break;
    }
    else if(branchIsArr[bPos]){
      std::string pegStr = branchArrPeg[bPos];

      int pegPos = -1;      
      for(int bI = 0; bI < (int)branchNames.size(); ++bI){
	if(pegStr.size() == branchNames.at(bI).size() && pegStr.find(branchNames.at(bI)) != std::string::npos){
	  pegPos = bI;
	  break;
	}
      }

      Int_t pegVal = 0;

      if(branchDT[pegPos].find("I") != std::string::npos) pegVal = *(branchInts.at(branchPos.at(pegPos)));
      else if(branchDT[pegPos].find("i") != std::string::npos) pegVal = *(branchInts.at(branchPos.at(pegPos)));
      else std::cout << "Array event cuts not yet implemented for non integer peg vals" << std::endl;

      bool doBreak = false;

      if(branchDT[bPos].find("B") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Char_t tempVarVal = branchCharArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("S") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Short_t tempVarVal = branchShortArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("I") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Int_t tempVarVal = branchIntArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("L") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Long64_t tempVarVal = branchLong64Arrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("F") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Float_t tempVarVal = branchFloatArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("D") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  Double_t tempVarVal = branchDoubleArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("b") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  UChar_t tempVarVal = branchUCharArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("s") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  UShort_t tempVarVal = branchUShortArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("i") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  UInt_t tempVarVal = branchUIntArrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }
      else if(branchDT[bPos].find("l") != std::string::npos){
	for(Int_t pI = 0; pI < pegVal; ++pI){
	  ULong64_t tempVarVal = branchULong64Arrs.at(branchPos.at(bPos))[pI];
	  bool tempCutBool = evalStringCut(tempVarVal, cutSign, cutSubList.at(cI).at(2));
	  if(!isOr && !tempCutBool){evtPass = false; doBreak = true; break;}
	  else if(isOr && tempCutBool){evtPass = true; doBreak = true; break;}
	}
      }

      if(doBreak) break;

    }
  }

  return evtPass;
}

bool generalTree::evalStringCut(Char_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Char_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Char_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(Short_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Short_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Short_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(Int_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Int_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Int_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(Long64_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Long64_t cutVal = std::stol(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Long64_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(Float_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Float_t cutVal = std::stof(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Float_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(Double_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  Double_t cutVal = std::stod(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (Double_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}


bool generalTree::evalStringCut(UChar_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  UChar_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (UChar_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(UShort_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  UShort_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (UShort_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(UInt_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  UInt_t cutVal = std::stoi(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (UInt_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

bool generalTree::evalStringCut(ULong64_t val, std::string cutType, std::string cutStr)
{
  bool retVal = false;
  ULong64_t cutVal = std::stoul(cutStr);
  
  if(cutType.size() == 1 && cutType.find(">") != std::string::npos) retVal = (val > cutVal);
  else if(cutType.size() == 1 && cutType.find("<") != std::string::npos) retVal = (val < cutVal);
  else if(cutType.size() == 2 && cutType.find(">=") != std::string::npos) retVal = (val >= cutVal);
  else if(cutType.size() == 2 && cutType.find("<=") != std::string::npos) retVal = (val <= cutVal);
  else if(cutType.size() == 2 && cutType.find("==") != std::string::npos) retVal = (val == cutVal);
  else if(cutType.size() == 2 && cutType.find("!=") != std::string::npos) retVal = (val != cutVal);
  else std::cout << "Warning: in \'" << __FILE__ << "\', evalStringCut (ULong64_t), input \'" << cutType << "\' is invalid, returning false" << std::endl;

  return retVal;
}

void generalTree::fill()
{  
  outFile_p->cd();

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  if(doGlobalDebug){
    std::cout << "Dump branches before fill" << std::endl;
    std::cout << " ";
    for(unsigned int bI = 0; bI < branchNames.size(); ++bI){
      std::cout << branchNames.at(bI) << " (" << branchPos.at(bI) << ", " << branchIsArr.at(bI) << "), ";
    }
    std::cout << std::endl;
  }

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;
  outTree_p->Fill();

  if(doGlobalDebug) std::cout << __FILE__ << ", " << __LINE__ << std::endl;

  return;
}

void generalTree::write()
{
  outFile_p->cd();
  if(dirPath.size() != 0) outFile_p->cd(dirPath.c_str());
  std::cout << "  Writing \'" << outTree_p->GetName() << std::endl;
  std::cout << "   Final nEntries=" << outTree_p->GetEntries() << std::endl;
  outTree_p->Write("", TObject::kOverwrite);

  if(docCuts){
    if(dirPath.size() == 0){
      outFile_p->mkdir("cutDocDir");
      outFile_p->cd("cutDocDir");
    }
    else{
      outFile_p->mkdir((dirPath + "/cutDocDir").c_str());
      outFile_p->cd((dirPath + "/cutDocDir").c_str());
    }

    std::string andStr = "ANDCUTS_" + dirPath + "_" + std::string(outTree_p->GetName());
    TNamed andName(andStr.c_str(), "");
    andName.Write("", TObject::kOverwrite);

    for(unsigned int i = 0; i < cutSubListAND.size(); ++i){
      std::string cutStr = "";
      
      for(unsigned int j = 0; j < cutSubListAND.at(i).size(); ++j){
	cutStr = cutStr + cutSubListAND.at(i).at(j);
      }

      TNamed cutName((" AND Cut " + std::to_string(i)).c_str(), cutStr.c_str());
      cutName.Write("", TObject::kOverwrite);
    }
    if(cutSubListAND.size() == 0){
      TNamed cutName(" NONE", "");
      cutName.Write("", TObject::kOverwrite);
    }

    std::string orStr = "ORCUTS_" + dirPath + "_" + std::string(outTree_p->GetName());
    TNamed orName(orStr.c_str(), "");
    orName.Write("", TObject::kOverwrite);

    for(unsigned int i = 0; i < cutSubListOR.size(); ++i){
      std::string cutStr = "";
      
      for(unsigned int j = 0; j < cutSubListOR.at(i).size(); ++j){
	cutStr = cutStr + cutSubListOR.at(i).at(j);
      }

      TNamed cutName((" OR Cut " + std::to_string(i)).c_str(), cutStr.c_str());
      cutName.Write("", TObject::kOverwrite);
    }
    if(cutSubListOR.size() == 0){
      TNamed cutName(" NONE", "");
      cutName.Write("", TObject::kOverwrite);
    }
  }
  
  return;
}

void generalTree::clean()
{
  for(unsigned int i = 0; i < branchChars.size(); ++i){delete (branchChars.at(i));}
  for(unsigned int i = 0; i < branchShorts.size(); ++i){delete (branchShorts.at(i));}
  for(unsigned int i = 0; i < branchInts.size(); ++i){delete (branchInts.at(i));}
  for(unsigned int i = 0; i < branchFloats.size(); ++i){delete (branchFloats.at(i));}
  for(unsigned int i = 0; i < branchDoubles.size(); ++i){delete (branchDoubles.at(i));}
  for(unsigned int i = 0; i < branchLong64s.size(); ++i){delete (branchLong64s.at(i));}
  for(unsigned int i = 0; i < branchUChars.size(); ++i){delete (branchUChars.at(i));}
  for(unsigned int i = 0; i < branchUShorts.size(); ++i){delete (branchUShorts.at(i));}
  for(unsigned int i = 0; i < branchUInts.size(); ++i){delete (branchUInts.at(i));}
  for(unsigned int i = 0; i < branchULong64s.size(); ++i){delete (branchULong64s.at(i));}
  for(unsigned int i = 0; i < branchBools.size(); ++i){delete (branchBools.at(i));}

  for(unsigned int i = 0; i < branchCharArrs.size(); ++i){delete (branchCharArrs.at(i));}
  for(unsigned int i = 0; i < branchShortArrs.size(); ++i){delete (branchShortArrs.at(i));}
  for(unsigned int i = 0; i < branchIntArrs.size(); ++i){delete (branchIntArrs.at(i));}
  for(unsigned int i = 0; i < branchFloatArrs.size(); ++i){delete (branchFloatArrs.at(i));}
  for(unsigned int i = 0; i < branchDoubleArrs.size(); ++i){delete (branchDoubleArrs.at(i));}
  for(unsigned int i = 0; i < branchLong64Arrs.size(); ++i){delete (branchLong64Arrs.at(i));}
  for(unsigned int i = 0; i < branchUCharArrs.size(); ++i){delete (branchUCharArrs.at(i));}
  for(unsigned int i = 0; i < branchUShortArrs.size(); ++i){delete (branchUShortArrs.at(i));}
  for(unsigned int i = 0; i < branchUIntArrs.size(); ++i){delete (branchUIntArrs.at(i));}
  for(unsigned int i = 0; i < branchULong64Arrs.size(); ++i){delete (branchULong64Arrs.at(i));}
  for(unsigned int i = 0; i < branchBoolArrs.size(); ++i){delete (branchBoolArrs.at(i));}

  for(unsigned int i = 0; i < branchVectInts.size(); ++i){delete (branchVectInts.at(i));}
  for(unsigned int i = 0; i < branchVectFloats.size(); ++i){delete (branchVectFloats.at(i));}
  for(unsigned int i = 0; i < branchVectDoubles.size(); ++i){delete (branchVectDoubles.at(i));}

  delete outTree_p;
  outFile_p = 0;

  return;
}

void generalTree::printBranchNames()
{
  for(unsigned int i = 0; i < branchNames.size(); ++i){
    std::cout << "Branch " << i << ": " << branchNames.at(i) << std::endl;
  }

  return;
}

void generalTree::printBranchFullInfo()
{
  for(unsigned int i = 0; i < branchNames.size(); ++i){
    if(branchIsArr[i]) std::cout << "Branch " << i << ": " << branchNames.at(i) << ", " << branchDT.at(i) << ", " << branchIsArr.at(i) << " [" << branchArrPeg.at(i) << "==" << branchArrPegMax[branchArrPeg.at(i)] << "]" << std::endl;
    else std::cout << "Branch " << i << ": " << branchNames.at(i) << ", " << branchDT.at(i) << ", " << branchIsArr.at(i) << std::endl;
  }

  return;
}

#endif
