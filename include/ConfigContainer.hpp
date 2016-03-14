
#ifndef ConfigContainer_hpp
#define ConfigContainer_hpp

#include "VariableContainer.hpp"
#include "SampleContainer.hpp"
#include "CutContainer.hpp"

#include "string"

using namespace std;

struct ConfigContainer {
    SampleContainer sampleContainer;
    VariableContainer variableContainer;
    CutContainer cutContainer;
    
    string treeName, sampleDir, outputDir, plotString;
    float luminosity, maxEvents, minJetPt, minLeptonPt;
    bool plotRatio, plotSignificance, logY, signalStacked, addOverflow, addUnderflow, drawUncertainty, unblind;
};
#endif