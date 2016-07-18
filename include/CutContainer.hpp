
#ifndef CutContainer_hpp
#define CutContainer_hpp

#include <vector>
#include <string>

using namespace std;

struct CutContainer {
    vector<string> variableNames;
    vector<float> cutValues;
    bool triggerSelection;
    vector<unsigned int> triggerVector;
    vector<float> triggerPtVector, triggerLumiVector;

    void add(const CutContainer& right)
    {
        variableNames.insert( variableNames.end(), right.variableNames.begin(), right.variableNames.end() );
        cutValues.insert( cutValues.end(), right.cutValues.begin(), right.cutValues.end() );
    }
};

#endif