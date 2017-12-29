# GeneralTree
General skimming format for TTree of basic datatypes, array of basic datatypes, and vector of basic datatypes

Note that the following code requires a c++ compiler and ROOT, although if you've downloaded a ROOT TTree skimmer you presumably already have both.

To build, run "make"
To clean, run "make clean"

To test, run ./bin/testGeneralTrees.exe <yourFileWithNTuples> <yourConfigFile> <outFileName-optional>

A file in input is available, input/HiForest_10_SKIM_20171229.root, compatible with config file config/treeTest.txt

DO:
	./bin/testGeneralTrees.exe input/HiForest_10_SKIM_20171229.root test/treeTest.txt 

To write your own interface, see skeleton example testGeneralTrees.C and follow the basic outline
To write your own configFile, see skeleton example config/treeTest.txt