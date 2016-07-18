
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
    
    void add(const SampleContainer& right)
    {
        sampleNames.insert( sampleNames.end(), right.sampleNames.begin(), right.sampleNames.end() );
        reducedNames.insert( reducedNames.end(), right.reducedNames.begin(), right.reducedNames.end() );
        luminosity.insert( luminosity.end(), right.luminosity.begin(), right.luminosity.end() );
        weight.insert( weight.end(), right.weight.begin(), right.weight.end() );
        color.insert( color.end(), right.color.begin(), right.color.end() );
        sampleType.insert( sampleType.end(), right.sampleType.begin(), right.sampleType.end() );
    }
};
    
#endif