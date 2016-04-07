
#ifndef VariableContainer_hpp
#define VariableContainer_hpp

#include <vector>
#include <map>
#include <string>

using namespace std;

struct VariableContainer {
    vector<string> variableNames;
    vector<int> nBins;
    vector<vector<double> > binning; // either (min,max) or all bins (double needed for TH2)
    map<unsigned int, vector<double>> axisRanges;
    vector<bool> is2D;
};
#endif