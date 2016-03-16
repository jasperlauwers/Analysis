
#include "EventCleaner.hpp"

EventCleaner::EventCleaner(EventContainer& evContainer)
: eventContainer(evContainer) { }

EventCleaner::~EventCleaner() { }

void EventCleaner::doCleaning() 
{
    // Lepton 
    doLeptonCleaning();
    
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

void EventCleaner::doLeptonCleaning() 
{
    // Lepton cleaning
    unsigned int nGoodLeptons = eventContainer.goodLeptons.size();
    for( unsigned int iLepton=0; iLepton < nGoodLeptons; ++iLepton ) {
        bool tightLepton = false;
        
        if( eventContainer.leptons[eventContainer.goodLeptons[iLepton]].isElectron() )
        {
            if( eventContainer.leptons[eventContainer.goodLeptons[iLepton]].passesMedium() )
                tightLepton = true;
        }
        else if( eventContainer.leptons[eventContainer.goodLeptons[iLepton]].isMuon() ) 
        {
            if( /*eventContainer.leptons[eventContainer.goodLeptons[iLepton]].passesMedium() 
                && abs(eventContainer.leptons[eventContainer.goodLeptons[iLepton]].dz()) < 0.1 
                && abs(eventContainer.leptons[eventContainer.goodLeptons[iLepton]].d0()) < (eventContainer.leptons[eventContainer.goodLeptons[iLepton]].pt()<20? 0.01:0.02 )
                && */abs(eventContainer.leptons[eventContainer.goodLeptons[iLepton]].isolation()) < 0.15 )
                tightLepton = true;
        }           
        if( ! tightLepton )
        {    
            eventContainer.goodLeptons.erase(eventContainer.goodLeptons.begin()+iLepton);
            iLepton--;
            nGoodLeptons--;
        }
    }
}

void EventCleaner::doTrackJetsCleaning()
{
    unsigned int nGoodJets = eventContainer.goodTrackJets.size();
    for( unsigned int iJet=0; iJet < nGoodJets; ++iJet ) {
        
        bool goodJet = true;
        for( auto iLepton : eventContainer.goodLeptons ) {
            if( eventContainer.trackJets[eventContainer.goodTrackJets[iJet]].dR(eventContainer.leptons[iLepton]) < 0.3 ) 
                goodJet = false;
        }
        if( ! goodJet )
        {
            eventContainer.goodTrackJets.erase(eventContainer.goodTrackJets.begin()+iJet); 
            iJet--;
            nGoodJets--;
        }
    }
}