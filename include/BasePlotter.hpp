
#ifndef BasePlotter_hpp
#define BasePlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "HistogramContainer.hpp"
#include "LatinoStyle2.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include <iostream>
#include <string>
#include <functional>

using namespace std;

typedef function<float ()> eventContainerFunction;

class BasePlotter {

public:
    BasePlotter(const EventContainer&, const ConfigContainer&);
    virtual ~BasePlotter();
    void writeHist(string filename, const vector<TH1*>& histVector, string option) const;
    void writeStacked(string filename, const HistogramContainer& histContainer, string extension) const;
    void writeEfficiency(const HistogramContainer& numeratorContainer, const vector<TH1*>& denominatorHistograms, string extension) const;
    void writeEfficiency(const HistogramContainer& numeratorContainer, const vector<TH1*>& denominatorHistograms, string extension, const vector<string>& binNames) const;
        
protected:
    float getMaximumIncludingErrors(const TH1* h) const;
    void setCanvasAttributes( unsigned int nSamples, TLegend*& leg, vector<TLatex*>& latexVector) const;
    
    const EventContainer& eventContainer;
    const ConfigContainer& configContainer;
    eventContainerFunction eventWeightFunction;
    vector<vector<float>> globalWeight;   
};


#endif
