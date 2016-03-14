
#ifndef EventPlotter_hpp
#define EventPlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "VariableDictionary.hpp"
#include "HistogramContainer.hpp"
#include "BasePlotter.hpp"
#include "LatinoStyle2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>
#include <algorithm>

using namespace std;

typedef function<float ()> eventContainerFunction;

class EventPlotter : BasePlotter {

public:
    EventPlotter(const EventContainer&, const ConfigContainer&);
    ~EventPlotter();
    void fill(unsigned int iSample, unsigned int iSubSample);
    void writePlots(string extension);
    void writeHist(string filename);
    
    // parallel running
    EventPlotter(const ConfigContainer&);
    void init(const EventContainer& evContainer, unsigned int iSample);
    void parallelFill(unsigned int iSample, unsigned int iSubSample);
        
private:
    vector<eventContainerFunction> functionVector;
    vector<HistogramContainer> histogramContainers;
    unsigned int nSamples, nVariables;
    
    // parallel running
//     vector<const EventContainer*> eventContainerParallel;
//     vector<vector<eventContainerFunction>> functionVectorParallel;
//     vector<eventContainerFunction> eventWeightFunctionParallel;
//     vector<vector<TH1F*>> histograms;
};


#endif