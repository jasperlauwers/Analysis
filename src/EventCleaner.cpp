
#include "EventCleaner.hpp"

EventCleaner::EventCleaner(EventContainer& evContainer)
: eventContainer(evContainer) { }

EventCleaner::~EventCleaner() { }

void EventCleaner::doCleaning() 
{
    // Lepton 
//     doLeptonCleaning();
    
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

void EventCleaner::doFakeLeptonCleaning() 
{
    // Lepton cleaning
    unsigned int nGoodLeptons = eventContainer.goodLeptons.size();
    for( unsigned int iLepton=0; iLepton < nGoodLeptons; ++iLepton ) {        
        if( ! eventContainer.leptons[eventContainer.goodLeptons[iLepton]].passesMedium() )
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

void EventCleaner::doLooseLeptonIsoCleaning()
{
    // Lepton cleaning
    unsigned int nLeptons = eventContainer.looseLeptons.size();
    for( unsigned int iLepton=0; iLepton < nLeptons; ++iLepton ) {        
        if( eventContainer.looseLeptons[iLepton].pt() <= 0. )
	    break;
        
        float trackIsoCut = 0.2;
        if( eventContainer.looseLeptons[iLepton].isMuon() )
            trackIsoCut = 0.4;
            
	if( eventContainer.looseLeptons[iLepton].isolation() >= trackIsoCut )
        {
	    for( unsigned int iLepton2=iLepton; iLepton2 < nLeptons-1; ++iLepton2 ) { 
  		eventContainer.looseLeptons[iLepton2] = eventContainer.looseLeptons[iLepton2+1];
	    }
            eventContainer.looseLeptons[nLeptons-1].set(0,0,0,0);
	    iLepton--;
        }
    }
}

