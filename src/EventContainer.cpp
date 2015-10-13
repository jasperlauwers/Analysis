
#include "EventContainer.hpp"
#include "TChain.h"

EventContainer::EventContainer()
: iEvent(0), _weight(1.) { }

EventContainer::EventContainer(const unsigned int leptonSize, const unsigned int jetSize)
: iEvent(0), leptons(leptonSize), jets(jetSize), _weight(1.) { }

EventContainer::~EventContainer() { }

void EventContainer::init(const unsigned int leptonSize, const unsigned int jetSize) 
{
    leptons.resize(leptonSize);
    jets.resize(jetSize);
}

void EventContainer::reset() 
{
    iEvent = 0;
    _weight = 1.;
    leptons.clear();
    goodLeptons.clear();
    jets.clear();
    goodJets.clear();
}

double EventContainer::jetpt(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].pt();
    else
        return -9999.9;
}
double EventContainer::jeteta(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].eta();
    else
        return -9999.9;
}
double EventContainer::jetphi(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].phi();
    else
        return -9999.9;
}
double EventContainer::jetmass(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].mass();
    else
        return -9999.9;
}
double EventContainer::leptonpt(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].pt();
    else
        return -9999.9;
}
double EventContainer::leptoneta(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].eta();
    else
        return -9999.9;
}
double EventContainer::leptonphi(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].phi();
    else
        return -9999.9;
}

double EventContainer::mll() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].mpp(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
double EventContainer::mjj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].mpp(jets[goodJets[1]]);
    else
        return -9999.9;
}
double EventContainer::ptll() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].ptpp(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
double EventContainer::ptjj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].ptpp(jets[goodJets[1]]);
    else
        return -9999.9;
}
double EventContainer::detall() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].dEta(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
double EventContainer::detajj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].dEta(jets[goodJets[1]]);
    else
        return -9999.9;  
}
double EventContainer::dphill() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].dPhi(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
double EventContainer::dphijj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].dPhi(jets[goodJets[1]]);
    else
        return -9999.9;
}
double EventContainer::channel() const
{
    if( goodLeptons.size() > 1 )
    {
        if( leptons[goodLeptons[0]].isElectron() )
        {
            if( leptons[goodLeptons[1]].isElectron() ) 
                return 1.; // ee
            else 
                return 2; // em
        }
        else
        {
            if( leptons[goodLeptons[1]].isElectron() ) 
                return 3.; // me
            else 
                return 0; // mm
        }
    }
    else
        return -9999.9;
}

float EventContainer::weight() const
{
    return _weight;
}
void EventContainer::setWeight(float weight)
{
    _weight = weight;    
}