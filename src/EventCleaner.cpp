
#include "EventCleaner.hpp"

EventCleaner::EventCleaner(EventContainer& evContainer)
: eventContainer(evContainer), nLeptons( eventContainer.leptons.size() ), nJets( eventContainer.jets.size() ) { }

EventCleaner::~EventCleaner() { }

void EventCleaner::doCleaning() 
{
    for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {
        if( eventContainer.leptons[iLepton].pt() <= 0 ) 
            break;
        if( eventContainer.leptons[iLepton].passesMedium() )
        {    
            eventContainer.goodLeptons.push_back(iLepton);
        }
    }
    
    unsigned int nGoodLeptons = eventContainer.goodLeptons.size();
    for( unsigned int iJet=0; iJet < nJets; ++iJet ) {
        if( eventContainer.jets[iJet].pt() <= 0 ) 
            break;
        
        bool goodJet = true;
        for( unsigned int iLepton=0; iLepton < nGoodLeptons; ++iLepton ) {
            if( eventContainer.leptons[eventContainer.goodLeptons[iLepton]].pt() < 10. )
                break;
            if( eventContainer.jets[iJet].dR(eventContainer.leptons[eventContainer.goodLeptons[iLepton]]) < 0.3 ) 
                goodJet = false;
        }
        if( goodJet )
            eventContainer.goodJets.push_back(iJet); 
    }
}