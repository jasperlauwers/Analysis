
#ifndef CutPlotter_hpp
#define CutPlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "BasePlotter.hpp"
#include "HistogramContainer.hpp"
#include "LatinoStyle2.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <string>

using namespace std;

typedef function<float ()> eventContainerFunction;

class CutPlotter : BasePlotter {

public:
    CutPlotter(const EventContainer&, const ConfigContainer&);
    ~CutPlotter();
    void fill(unsigned int iSample, unsigned int iSubSample, unsigned int iCut);
    void writeStacked(string extension);
    void writeEfficiency(string extension);
    void writeHist(string filename) const;
        
private:
    unsigned int nSamples, nCuts;
    HistogramContainer histogramContainer;
};


#endif