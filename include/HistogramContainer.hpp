
#ifndef HistogramContainer_hpp
#define HistogramContainer_hpp

#include <vector>
#include <string>
#include <iostream>
#include "TH1.h"

using namespace std;

struct HistogramContainer {
    HistogramContainer(string name);
    HistogramContainer(string name, unsigned int nTotal);
    ~HistogramContainer();
    void add(TH1* h, string histName, int color_ = 1, bool isData_ = false, bool isMc_ = true);
    bool check() const;

    string containerName; // plotted variable
    vector<TH1*> histograms;
    vector<string> reducedNames; // sample name
    vector<int> color;
    vector<bool> isData;
    vector<bool> isMC;
};
#endif