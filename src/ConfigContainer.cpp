
#ifndef ConfigContainer_cpp
#define ConfigContainer_cpp

#include "VariableContainer.cpp"
#include "SampleContainer.cpp"
#include "CutContainer.cpp"

#include "string"

using namespace std;

struct ConfigContainer {
    SampleContainer sampleContainer;
    VariableContainer variableContainer;
    CutContainer cutContainer;
    
    string treeName, sampleDir, outputDir;
    double luminosity;
};
#endif