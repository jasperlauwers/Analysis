#ifndef WeightCalc_hpp
#define WeightCalc_hpp

#include <iostream>
#include <string>
#include <algorithm>
#include "TH2D.h"
#include "TFile.h"
#include "EventContainer.hpp"
#include "FakeContainer.hpp"
#include "SampleContainer.hpp"
#include "EventReader.hpp"

using namespace std;

class WeightCalc {
public:
    WeightCalc(EventContainer& evContainer, bool latinoFakes);
    ~WeightCalc();
    void initFakeWeight(FakeContainer* fContainer);
    void initDYWeight(const EventReader& reader);
    void setWeight(SampleType sampleType, const string& sampleName);
    
private:
    EventContainer& eventContainer;  
    FakeContainer *fakeContainer;
    TH2D *hFakeElectron, *hFakeElectron2, *hFakeMuon, *hFakeMuon2, *hPromptElectron, *hPromptMuon;
    vector<float> DYweights;
    TH2D *hDYshapeWeight;
    float maxVal;
    bool applyDYWeight, applyFakeWeight, useElectronCorrectedPt, useMuonCorrectedPt, useTwoMuonFR, useTwoElectronFR, latinoFakes;
};
#endif