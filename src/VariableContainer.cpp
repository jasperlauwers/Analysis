
#ifndef VariableContainer_cpp
#define VariableContainer_cpp

#include <vector>
#include <string>

using namespace std;

struct VariableContainer {
    vector<string> variableNames;
    vector<int> nBins;
    vector<vector<double> > binning; // either (min,max) or all bins
};
#endif