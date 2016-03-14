
#ifndef SampleContainer_hpp
#define SampleContainer_hpp

#include <vector>
#include <string>

using namespace std;

enum class SampleType { DATA, MC, SIGNAL, FAKELEPTON, FAKELEPTON_UP, FAKELEPTON_DOWN};

struct SampleContainer {
    vector<vector<string>> sampleNames;
    vector<string> reducedNames;
    vector<vector<float>> luminosity;
    vector<vector<float>> weight;
    vector<int> color;
    vector<SampleType> sampleType;
};
#endif