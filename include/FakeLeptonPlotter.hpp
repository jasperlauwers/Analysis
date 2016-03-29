
#ifndef FakeLeptonPlotter_hpp
#define FakeLeptonPlotter_hpp

#include "EventContainer.hpp"
#include "ConfigContainer.hpp"
#include "VariableDictionary.hpp"
#include "HistogramContainer.hpp"
#include "EventPlotter.hpp"
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

class FakeLeptonPlotter : public EventPlotter {

public:
    FakeLeptonPlotter(const EventContainer&, const ConfigContainer&);
    ~FakeLeptonPlotter();
    void fill(unsigned int iSample, unsigned int iSubSample);
    void writeFakeRate(string extension);
        
private:
    vector<HistogramContainer> fakeHistogramContainers;
    vector<vector<TH1*>> hDenomVector;
};


#endif