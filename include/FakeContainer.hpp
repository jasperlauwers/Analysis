
#ifndef FakeContainer_hpp
#define FakeContainer_hpp

#include <string>

using namespace std;

struct FakeContainer {
    string fakeElectronFile, fakeMuonFile, fakeMuonFile2, promptElectronFile, promptMuonFile;
    string fakeElectronHist, fakeMuonHist, fakeMuonHist2, promptElectronHist, promptMuonHist;
    float maxPtElectronPrompt, maxPtMuonPrompt, maxPtElectronFake, maxPtMuonFake, maxPtMuonFake2;
};
#endif