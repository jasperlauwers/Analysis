#ifndef WeightCalc_hpp
#define WeightCalc_hpp

#include <iostream>
#include <string>
#include <algorithm>
#include "TH2F.h"
#include "TFile.h"
#include "EventContainer.hpp"
#include "FakeContainer.hpp"
#include "SampleContainer.hpp"
#include "EventReader.hpp"

using namespace std;

class WeightCalc {
public:
    WeightCalc(EventContainer& evContainer);
    ~WeightCalc();
    void initFakeWeight(FakeContainer* fContainer);
    void initDYWeight(const EventReader& reader);
    void setWeight(SampleType sampleType, const string& sampleName);
    
private:
    EventContainer& eventContainer;  
    FakeContainer *fakeContainer;
    TH2F *hFakeElectron, *hFakeMuon, *hPromptElectron, *hPromptMuon;
    vector<float> DYweights;
    TH1F *hDYshapeWeight;
    float maxVal;
    bool applyDYWeight, applyFakeWeight;
};
#endif