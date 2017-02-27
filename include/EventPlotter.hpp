
#ifndef EventPlotter_hpp
#define EventPlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "VariableDictionary.hpp"
#include "HistogramContainer.hpp"
#include "BasePlotter.hpp"
#include "LatinoStyle2.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>
#include <algorithm>

using namespace std;

typedef function<float ()> eventContainerFunction;

class EventPlotter : protected BasePlotter {

public:
    EventPlotter(const EventContainer&, const ConfigContainer&);
    virtual ~EventPlotter();
    virtual void fill(unsigned int iSample, unsigned int iSubSample);
    void writePlots(string extension);
    void writeHist(string filename);
    HistogramContainer* getHistogramContainer(unsigned int i);
    
    // parallel running
    EventPlotter(const ConfigContainer&);
    void init(const EventContainer& evContainer, unsigned int iSample);
    void parallelFill(unsigned int iSample, unsigned int iSubSample);
        
protected:
    vector<eventContainerFunction> functionVector;
    vector<HistogramContainer> histogramContainers;
    unsigned int nSamples, nVariables, nHistograms;
    
    // parallel running
//     vector<const EventContainer*> eventContainerParallel;
//     vector<vector<eventContainerFunction>> functionVectorParallel;
//     vector<eventContainerFunction> eventWeightFunctionParallel;
//     vector<vector<TH1D*>> histograms;
};


#endif