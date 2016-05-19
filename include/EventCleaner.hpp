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
    void doFakeLeptonCleaning();
    void doTrackJetsCleaning();
    void doLooseLeptonIsoCleaning();
    
private:
    EventContainer& eventContainer;    
};
#endif
