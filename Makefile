#std-c++11
CXX = g++
CXXFLAGS = -Wall -O2 -Wextra -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif

ROOT = `root-config --cflags --glibs`


MKDIR_BIN = mkdir -p $(PWD)/bin
MKDIR_PDFDIR = mkdir -p $(PWD)/pdfDir
MKDIR_OUTPUT = mkdir -p $(PWD)/output

all: mkdirBin mkdirOutput mkdirPdfdir forestJetSkim testGeneralTrees quickMinMaxCheck

mkdirBin:
	$(MKDIR_BIN)

mkdirOutput:
	$(MKDIR_OUTPUT)

mkdirPdfdir:
	$(MKDIR_PDFDIR)

forestJetSkim: src/forestJetSkim.C
	$(CXX) $(CXXFLAGS) $(ROOT) -I $(PWD) -o bin/forestJetSkim.exe src/forestJetSkim.C

testGeneralTrees: src/testGeneralTrees.C
	$(CXX) $(CXXFLAGS) $(ROOT) -I $(PWD) -o bin/testGeneralTrees.exe src/testGeneralTrees.C

quickMinMaxCheck: src/quickMinMaxCheck.C
	$(CXX) $(CXXFLAGS) $(ROOT) -I $(PWD) -o bin/quickMinMaxCheck.exe src/quickMinMaxCheck.C

clean:
	rm *~ || true
	rm *# || true
	rm include/*~ || true
	rm include/#*# || true
	rm src/*~ || true
	rm src/#*# || true
	rm bin/*.exe || true
	rmdir bin || true