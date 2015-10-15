
#ifndef EventReader_hpp
#define EventReader_hpp

#include "ConfigContainer.hpp"
#include "EventContainer.hpp"
#include "TreeReader.hpp"
#include "libconfig.h++"

using namespace libconfig;
using namespace std;

class EventReader {

public:
    EventReader(EventContainer&, const ConfigContainer&); 
    ~EventReader();
    void setSample(unsigned int iSample, unsigned int iSubSample);
    bool fillNextEvent();
        
private:
    EventContainer& eventContainer;
    const ConfigContainer& configContainer;
    TreeReader *treeReader;
    unsigned int nLeptons, nJets;  
    bool isData;
    vector<pair<float&,float&>> extraBranches;
};


#endif