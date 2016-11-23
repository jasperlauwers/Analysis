
#ifndef FakeContainer_hpp
#define FakeContainer_hpp

#include <string>

using namespace std;

struct FakeContainer {
    string fakeElectronFile, fakeElectronFile2, fakeMuonFile, fakeMuonFile2, promptElectronFile, promptMuonFile;
    string fakeElectronHist, fakeElectronHist2, fakeMuonHist, fakeMuonHist2, promptElectronHist, promptMuonHist;
    float maxPtElectronPrompt, maxPtMuonPrompt, maxPtElectronFake, maxPtElectronFake2, maxPtMuonFake, maxPtMuonFake2;
};
#endif