
#include "EventContainer.hpp"
#include "TChain.h"

EventContainer::EventContainer()
: iEvent(0), _weight(1.) { }

EventContainer::EventContainer(const unsigned int leptonSize, const unsigned int jetSize)
: iEvent(0), _weight(1.) 
{
    init( leptonSize, jetSize );
}

EventContainer::~EventContainer() { }

void EventContainer::init(const unsigned int leptonSize, const unsigned int jetSize) 
{
    leptons.resize(leptonSize);
    genLeptons.resize(leptonSize);
    jets.resize(jetSize);
    genJets.resize(jetSize);
    puppiJets.resize(jetSize);
}

void EventContainer::reset() 
{
    iEvent = 0;
    _weight = 1.;
    met.set(0,0);
    leptons.clear();
    goodLeptons.clear();
    jets.clear();
    goodJets.clear();
    genLeptons.clear();
    goodGenLeptons.clear();
    genJets.clear();
    goodGenJets.clear();
    puppiJets.clear();
    goodPuppiJets.clear();
}

// Jets
float EventContainer::jetpt(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].pt();
    else
        return -9999.9;
}
float EventContainer::jeteta(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].eta();
    else
        return -9999.9;
}
float EventContainer::jetphi(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].phi();
    else
        return -9999.9;
}
float EventContainer::jetmass(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].mass();
    else
        return -9999.9;
}
float EventContainer::jetcsv(unsigned int i) const
{
    if( i < goodJets.size() )
        return jets[goodJets[i]].csv();
    else
        return -9999.9;
}
unsigned int EventContainer::nJets( float minPt ) const
{
    for( unsigned int i = 0; i < goodJets.size(); ++i )
    {
        if( jets[goodJets[i]].pt() < minPt )
            return i;
    }
    return goodJets.size();
}

// Gen jets 
float EventContainer::genjetpt(unsigned int i) const
{
    if( i < goodGenJets.size() )
        return genJets[goodGenJets[i]].pt();
    else
        return -9999.9;
}
float EventContainer::genjeteta(unsigned int i) const
{
    if( i < goodGenJets.size() )
        return genJets[goodGenJets[i]].eta();
    else
        return -9999.9;
}
float EventContainer::genjetphi(unsigned int i) const
{
    if( i < goodGenJets.size() )
        return genJets[goodGenJets[i]].phi();
    else
        return -9999.9;
}
float EventContainer::genjetmass(unsigned int i) const
{
    if( i < goodGenJets.size() )
        return genJets[goodGenJets[i]].mass();
    else
        return -9999.9;
}

// Puppi jets
float EventContainer::puppijetpt(unsigned int i) const
{
    if( i < goodPuppiJets.size() )
        return puppiJets[goodPuppiJets[i]].pt();
    else
        return -9999.9;
}
float EventContainer::puppijeteta(unsigned int i) const
{
    if( i < goodPuppiJets.size() )
        return puppiJets[goodPuppiJets[i]].eta();
    else
        return -9999.9;
}
float EventContainer::puppijetphi(unsigned int i) const
{
    if( i < goodPuppiJets.size() )
        return puppiJets[goodPuppiJets[i]].phi();
    else
        return -9999.9;
}
float EventContainer::puppijetmass(unsigned int i) const
{
    if( i < goodPuppiJets.size() )
        return puppiJets[goodPuppiJets[i]].mass();
    else
        return -9999.9;
}

// Leptons
float EventContainer::leptonpt(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].pt();
    else
        return -9999.9;
}
float EventContainer::leptoneta(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].eta();
    else
        return -9999.9;
}
float EventContainer::leptonphi(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].phi();
    else
        return -9999.9;
}

// Gen leptons
float EventContainer::genleptonpt(unsigned int i) const
{
    if( i < goodGenLeptons.size() )
        return genLeptons[goodGenLeptons[i]].pt();
    else
        return -9999.9;
}
float EventContainer::genleptoneta(unsigned int i) const
{
    if( i < goodGenLeptons.size() )
        return genLeptons[goodGenLeptons[i]].eta();
    else
        return -9999.9;
}
float EventContainer::genleptonphi(unsigned int i) const
{
    if( i < goodGenLeptons.size() )
        return genLeptons[goodGenLeptons[i]].phi();
    else
        return -9999.9;
}

float EventContainer::mll() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].mpp(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::mjj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].mpp(jets[goodJets[1]]);
    else
        return -9999.9;
}
float EventContainer::ptll() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].ptpp(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::ptjj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].ptpp(jets[goodJets[1]]);
    else
        return -9999.9;
}
float EventContainer::detall() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].dEta(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::detajj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].dEta(jets[goodJets[1]]);
    else
        return -9999.9;  
}
float EventContainer::dphill() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].dPhi(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::dphijj() const
{
    if( goodJets.size() > 1 )
        return jets[goodJets[0]].dPhi(jets[goodJets[1]]);
    else
        return -9999.9;
}
float EventContainer::channel() const
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
float EventContainer::genmll() const
{
    if( goodGenLeptons.size() > 1 )
        return genLeptons[goodGenLeptons[0]].mpp(genLeptons[goodGenLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::genchannel() const
{
    if( goodGenLeptons.size() > 1 )
    {
        if( genLeptons[goodGenLeptons[0]].isElectron() )
        {
            if( genLeptons[goodGenLeptons[1]].isElectron() ) 
                return 1.; // ee
            else 
                return 2; // em
        }
        else
        {
            if( genLeptons[goodGenLeptons[1]].isElectron() ) 
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

float EventContainer::nvertices() const
{
    return _nVertices;
}
void EventContainer::setNvertices(float nVertices)
{
    _nVertices = nVertices;
}

// electron id parameters
float EventContainer::electrond0(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].d0();
        i++;
    }
    return -9999.9;
}
float EventContainer::electrondEtaIn(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].dEtaIn();
        i++;
    }
    return -9999.9;
}
float EventContainer::electrondPhiIn(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].dPhiIn();
        i++;
    }
    return -9999.9;
}
float EventContainer::electrondz(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].dz();
        i++;
    }
    return -9999.9;
}
float EventContainer::electroneffectiveArea(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].effectiveArea();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronmissingHits(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].missingHits();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronsigmaIetaIeta(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].sigmaIetaIeta();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronhOverE(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].hOverE();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronooEmoop(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].ooEmoop();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronconversionVeto(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].conversionVeto();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronscEta(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].scEta();
        i++;
    }
    return -9999.9;
}
float EventContainer::electronisolation(unsigned int i) const
{
    while( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
            return leptons[goodLeptons[i]].isolation();
        i++;
    }
    return -9999.9;
}