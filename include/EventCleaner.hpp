#ifndef EventCleaner_hpp
#define EventCleaner_hpp

#include <iostream>
#include <string>
#include <algorithm>
#include "EventContainer.hpp"
#include "VariableContainer.hpp"

using namespace std;

class EventCleaner {
public:
    EventCleaner(EventContainer& evContainer);
    ~EventCleaner();
    void doCleaning();
    
private:
    EventContainer& eventContainer;
    unsigned int nLeptons, nJets;
    
};
#endif