
#ifndef FakeContainer_hpp
#define FakeContainer_hpp

#include <string>

using namespace std;

struct FakeContainer {
    string fakeElectronFile, fakeMuonFile, promptElectronFile, promptMuonFile;
    string fakeElectronHist, fakeMuonHist, promptElectronHist, promptMuonHist;
    float maxPtElectronPrompt, maxPtMuonPrompt, maxPtElectronFake, maxPtMuonFake;
};
#endif