
#ifndef HistogramContainer_hpp
#define HistogramContainer_hpp

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "TH1.h"
#include "SampleContainer.hpp"

using namespace std;

struct HistogramContainer {
    HistogramContainer(string name);
    HistogramContainer(string name, unsigned int nTotal);
    ~HistogramContainer();
    void add(TH1* h, string histName, int color_ = 1, SampleType sampleType = SampleType::MC, const vector<double>& axisRange = vector<double>());
    void add(const HistogramContainer& histContainer, int index);
    void pop_back();
    bool check() const;
    void addOverflow();
    void addUnderflow();

    string containerName; // plotted variable
    vector<TH1*> histograms;
    vector<string> reducedNames; // sample name
    vector<int> color;
    vector<SampleType> sampleType;
    vector<double> axisRanges;
};
#endif