
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

enum class DataType { MuonEG, DoubleMuon, SingleMuon, DoubleEG, SingleElectron};

class EventReader {

public:
    EventReader(EventContainer&, const ConfigContainer&); 
    ~EventReader();
    void addReadBranches(const vector<string>&);
    bool setSample(unsigned int iSample, unsigned int iSubSample);
    bool fillNextEvent();
    void reweigh(unsigned int index);
    void getDYWeights(vector<float>&) const;
        
private:
    void findTrees(unsigned int iSample, unsigned int iSubSample, TTree *t) const;
    
    EventContainer& eventContainer;
    const ConfigContainer& configContainer;
    TreeReader *treeReader;
    unsigned int nLeptons, nJets;  
    bool needJets, needGenJets, needPuppiJets, needGenLeptons, needLHELeptons, needElectronId, firstLooseLepton, needLooseLeptons, needTrackJets, needSoftMuons, needTaus, triggerSelection, hasNegWeight, isDY, applybPogSF, genMatching;
    SampleType sampleType;
    DataType dataType;
    vector<setFunction> functionVector;
    vector<string> branches, genBranches;
    float maxEventsWeight;
};


#endif