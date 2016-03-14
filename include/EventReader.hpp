
#ifndef EventReader_hpp
#define EventReader_hpp

#include "ConfigContainer.hpp"
#include "EventContainer.hpp"
#include "TreeReader.hpp"
#include "libconfig.h++"
#include "TMath.h"
#include <dirent.h>
#include <regex>
// #include "TThread.h"

using namespace libconfig;
using namespace std;

typedef function<void ()> setFunction;

class EventReader {

public:
    EventReader(EventContainer&, const ConfigContainer&); 
    ~EventReader();
    void addReadBranches(const vector<string>&);
    bool setSample(unsigned int iSample, unsigned int iSubSample);
    bool fillNextEvent();
    void reweigh(unsigned int index);
        
private:
    EventContainer& eventContainer;
    const ConfigContainer& configContainer;
    TreeReader *treeReader;
    unsigned int nLeptons, nJets;  
    bool needJets, needGenJets, needPuppiJets, needGenLeptons, needElectronId, firstLooseLepton, needLooseLeptons, needTrackJets, triggerSelection;
    SampleType sampleType;
    vector<setFunction> functionVector;
    vector<string> branches, genBranches;
    float maxEventsWeight;
};


#endif