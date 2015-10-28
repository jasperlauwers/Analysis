
#ifndef SampleContainer_hpp
#define SampleContainer_hpp

#include <vector>
#include <string>

using namespace std;

struct SampleContainer {
    vector<vector<string>> sampleNames;
    vector<string> reducedNames;
    vector<vector<float>> luminosity;
    vector<vector<float>> weight;
    vector<int> color;
    vector<bool> isData;
    vector<bool> isMC;
};
#endif