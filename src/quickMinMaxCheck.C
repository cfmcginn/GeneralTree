#include <iostream>

#include "TChain.h"

int quickMinMaxCheck()
{
  TChain* hi = new TChain("hiEvtAnalyzer/HiTree");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias/HIMinimumBias/crab_HIMinimumBias_XeXeForest_ReRecoed/180110_234002/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias1/HIMinimumBias1/crab_HIMinimumBias1_XeXeForest_ReRecoed/180110_233112/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias10/HIMinimumBias10/crab_HIMinimumBias10_XeXeForest_ReRecoed/180110_233557/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias13/HIMinimumBias13/crab_HIMinimumBias13_XeXeForest_ReRecoed/180110_233705/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias14/HIMinimumBias14/crab_HIMinimumBias14_XeXeForest_ReRecoed/180110_233729/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias16/HIMinimumBias16/crab_HIMinimumBias16_XeXeForest_ReRecoed/180110_233819/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias19/HIMinimumBias19/crab_HIMinimumBias19_XeXeForest_ReRecoed/180110_233940/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias3/HIMinimumBias3/crab_HIMinimumBias3_XeXeForest_ReRecoed/180110_233157/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias4/HIMinimumBias4/crab_HIMinimumBias4_XeXeForest_ReRecoed/180110_233225/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias5/HIMinimumBias5/crab_HIMinimumBias5_XeXeForest_ReRecoed/180110_233246/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias6/HIMinimumBias6/crab_HIMinimumBias6_XeXeForest_ReRecoed/180110_233422/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias7/HIMinimumBias7/crab_HIMinimumBias7_XeXeForest_ReRecoed/180110_233447/0000/Hi*.root");
  hi->Add("/eos/cms/store/group/phys_heavyions/kjung/XeXe_ReRecoForest_v1/HIMinimumBias9/HIMinimumBias9/crab_HIMinimumBias9_XeXeForest_ReRecoed/180110_233534/0000/Hi*.root");

  std::cout << hi->GetMinimum("run") << std::endl;;
  std::cout << hi->GetMaximum("run") << std::endl;;

  return 0;
}

int main()
{
  int retVal = 0;
  retVal += quickMinMaxCheck();
  return retVal;
}
