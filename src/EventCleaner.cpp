
#include "EventCleaner.hpp"

EventCleaner::EventCleaner(EventContainer& evContainer)
: eventContainer(evContainer) { }

EventCleaner::~EventCleaner() { }

void EventCleaner::doCleaning() 
{
    // RECO cleaning
    unsigned int nGoodLeptons = eventContainer.goodLeptons.size();
    for( unsigned int iLepton=0; iLepton < nGoodLeptons; ++iLepton ) {
        if( ! eventContainer.leptons[eventContainer.goodLeptons[iLepton]].passesMedium() )
        {    
            eventContainer.goodLeptons.erase(eventContainer.goodLeptons.begin()+iLepton);
            iLepton--;
            nGoodLeptons--;
        }
    }
    
    // CHS
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
    
    //PUPPI
    nGoodJets = eventContainer.goodPuppiJets.size();
    for( unsigned int iJet=0; iJet < nGoodJets; ++iJet ) {
        
        bool goodJet = true;
        for( auto iLepton : eventContainer.goodLeptons ) {
            if( eventContainer.leptons[iLepton].pt() < 10. )
                break;
            if( eventContainer.puppiJets[eventContainer.goodPuppiJets[iJet]].dR(eventContainer.leptons[iLepton]) < 0.3 ) 
                goodJet = false;
        }
        if( ! goodJet )
        {
            eventContainer.goodPuppiJets.erase(eventContainer.goodPuppiJets.begin()+iJet); 
            iJet--;
            nGoodJets--;
        }
    }
    
    // GEN cleaning  
    nGoodJets = eventContainer.goodGenJets.size();
    for( unsigned int iJet=0; iJet < nGoodJets; ++iJet ) {
        
        bool goodJet = true;
        for( auto iLepton : eventContainer.goodGenLeptons ) {
            if( eventContainer.genLeptons[iLepton].pt() < 10. )
                break;
            if( eventContainer.genJets[eventContainer.goodGenJets[iJet]].dR(eventContainer.genLeptons[iLepton]) < 0.3 ) 
                goodJet = false;
        }
        if( ! goodJet )
        {
            eventContainer.goodGenJets.erase(eventContainer.goodGenJets.begin()+iJet); 
            iJet--;
            nGoodJets--;
        }
    }
}