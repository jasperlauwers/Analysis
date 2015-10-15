
#ifndef EventPlotter_hpp
#define EventPlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "VariableDictionary.hpp"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include <iostream>

using namespace std;

typedef function<double ()> eventContainerFunction;

class EventPlotter {

public:
    EventPlotter(const EventContainer&, const ConfigContainer&); // give it config parser object
    ~EventPlotter();
    void fill(unsigned int iSample, unsigned int iSubSample);
    void writePlots(string extension) const;
    void writeHist(string filename) const;
        
private:
    unsigned int getIndex(const string& indexString, const string& fullString, bool isJet);

    const EventContainer& eventContainer;
    const ConfigContainer& configContainer;
    vector<eventContainerFunction> functionVector;
    eventContainerFunction eventWeightFunction;
    vector<vector<TH1F*>> histograms;
    vector<vector<float>> globalWeight;
    unsigned int nSamples, nVariables;
};


#endif