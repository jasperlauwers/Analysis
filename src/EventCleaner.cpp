
#include "EventCleaner.hpp"

EventCleaner::EventCleaner(EventContainer& evContainer)
: eventContainer(evContainer) { }

EventCleaner::~EventCleaner() { }

void EventCleaner::doCleaning() 
{
    unsigned int nGoodLeptons = eventContainer.goodLeptons.size();
    for( unsigned int iLepton=0; iLepton < nGoodLeptons; ++iLepton ) {
        if( ! eventContainer.leptons[eventContainer.goodLeptons[iLepton]].passesMedium() )
        {    
            eventContainer.goodLeptons.erase(eventContainer.goodLeptons.begin()+iLepton);
            iLepton--;
            nGoodLeptons--;
        }
    }
    
    nGoodLeptons = eventContainer.goodLeptons.size();
    unsigned int nGoodJets = eventContainer.goodJets.size();
    for( unsigned int iJet=0; iJet < nGoodJets; ++iJet ) {
        
        bool goodJet = true;
        for( auto iLepton : eventContainer.goodLeptons ) {
            if( eventContainer.leptons[iLepton].pt() < 10. )
                break;
            if( eventContainer.jets[eventContainer.goodJets[iJet]].dR(eventContainer.leptons[iLepton]) < 0.3 ) 
                goodJet = false;
        }
        if( ! goodJet )
        {
            eventContainer.goodJets.erase(eventContainer.goodJets.begin()+iJet); 
            iJet--;
            nGoodJets--;
        }
    }
}