
#ifndef VariableContainer_hpp
#define VariableContainer_hpp

#include <vector>
#include <string>

using namespace std;

struct VariableContainer {
    vector<string> variableNames;
    vector<int> nBins;
    vector<vector<float> > binning; // either (min,max) or all bins
};
#endif