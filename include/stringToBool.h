#ifndef STRINGTOBOOL_H
#define STRINGTOBOOL_H

#include <string>

bool stringToBool(const std::string inStr)
{
  std::string boolStr = inStr;
  const std::string upperStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const std::string lowerStr = "abcdefghijklmnopqrstuvwxyz";

  while(boolStr.find(" ") != std::string::npos) boolStr.replace(boolStr.find(" "), 1, "");

  for(unsigned int i = 0; i < boolStr.size(); ++i){
    if(upperStr.find(boolStr.substr(i,1)) != std::string::npos){
      boolStr.replace(i,1,lowerStr.substr(upperStr.find(boolStr.substr(i,1)),1));
    }
  }
 
  bool retBool = false;
  if(boolStr.find("false") != std::string::npos && boolStr.size() == 5) retBool = false;
  else if(boolStr.find("0") != std::string::npos && boolStr.size() == 1) retBool = false;
  else if(boolStr.find("true") != std::string::npos && boolStr.size() == 4) retBool = true;
  else if(boolStr.find("1") != std::string::npos && boolStr.size() == 1) retBool = true;
  else std::cout << "Warning: In \'" << __FILE__ << "\', input \'" << inStr << "\' is invalid. returning false" << std::endl;

  return retBool;
}

#endif
