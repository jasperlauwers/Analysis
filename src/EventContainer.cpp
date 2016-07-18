
#include "EventContainer.hpp"
#include "TChain.h"

EventContainer::EventContainer()
: iEvent(0), _weight(1.), _reweight(1), _upWeight(1), _downWeight(1) { }

EventContainer::EventContainer(const unsigned int leptonSize, const unsigned int jetSize)
: iEvent(0), _weight(1.), _reweight(1), _upWeight(1), _downWeight(1)
{
    init( leptonSize, jetSize );
}

EventContainer::~EventContainer() { }

void EventContainer::init(const unsigned int leptonSize, const unsigned int jetSize) 
{
    leptons.resize(leptonSize);
    genLeptons.resize(leptonSize);
    looseLeptons.resize(leptonSize);
    jets.resize(jetSize);
    genJets.resize(jetSize);
    puppiJets.resize(jetSize);
    trackJets.resize(50);
}

void EventContainer::reset() 
{
    iEvent = 0;
    _weight = 1.;
    _reweight = 1;
    _upWeight = 1;
    _downWeight = 1;
    met.set(0,0);
    leptons.clear();
    goodLeptons.clear();
    jets.clear();
    goodJets.clear();
    genLeptons.clear();
    goodGenLeptons.clear();
    looseLeptons.clear();
    genJets.clear();
    goodGenJets.clear();
    puppiJets.clear();
    goodPuppiJets.clear();
    trackJets.clear();
    goodTrackJets.clear();
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
float EventContainer::jetmaxcsv( float minPt ) const
{
    float maxCSV = -1.;
    for( unsigned int i = 0; i < goodJets.size(); ++i )
    {
        if( jets[goodJets[i]].pt() < minPt )
            break;
        if( jets[goodJets[i]].csv() > maxCSV )
            maxCSV = jets[goodJets[i]].csv();
    }
    return maxCSV;
}
float EventContainer::jetmaxSoftMuonPt( float minPt ) const
{
    float maxPt = -9999.9;
    for( unsigned int i = 0; i < goodJets.size(); ++i )
    {
        if( jets[goodJets[i]].pt() < minPt )
            break;
        if( jets[goodJets[i]].softMuPt() > maxPt && jets[goodJets[i]].softMuDr() < 0.3 )
            maxPt = jets[goodJets[i]].softMuPt();
    }
    return maxPt;
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
float EventContainer::genjetnhardprocess() const
{
    float n = 0;
    for( unsigned int i = 0; i < goodGenJets.size(); ++i )
        if( genJets[goodGenJets[i]].isHardProcess() ) 
            n++;
    return n;
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

// Track jets
float EventContainer::trackjetht() const
{
    float ht=0;
    vector<float> etaBounds = {trackJets[goodTrackJets[0]].eta(), trackJets[goodTrackJets[1]].eta()};
    sort(etaBounds.begin(), etaBounds.end());
    
    for(size_t i=0;i<goodTrackJets.size();++i) {
        if( trackJets[goodTrackJets[i]].eta() < (etaBounds[1]-0.4) && trackJets[goodTrackJets[i]].eta() > (etaBounds[0]+0.4) )
            ht += trackJets[goodTrackJets[i]].pt();
    }
//     ht -= (leptons[goodLeptons[0]].pt() + leptons[goodLeptons[1]].pt());
    return ht;
}
float EventContainer::trackjethtdensity() const
{
    vector<float> etaBounds = {trackJets[goodTrackJets[0]].eta(), trackJets[goodTrackJets[1]].eta()};
    sort(etaBounds.begin(), etaBounds.end());
    
    // Calculate eta range within tracker
    if( etaBounds[0] >= 2.1 || etaBounds[1] <= -2.1 ) return 0;
    if( etaBounds[0] < -2.9 ) etaBounds[0] = -2.9;
    if( etaBounds[1] > 2.9 ) etaBounds[1] = 2.9;
    float etaRange = abs(trackJets[goodTrackJets[0]].eta() - trackJets[goodTrackJets[1]].eta()) -0.8;
    if( etaRange > 0 )
        return (trackjetht()/etaRange);
    else
        return 0;
}
unsigned int EventContainer::trackjetn() const
{
    unsigned int n=0;
    vector<float> etaBounds = {trackJets[goodTrackJets[0]].eta(), trackJets[goodTrackJets[1]].eta()};
    sort(etaBounds.begin(), etaBounds.end());
    
    for(size_t i=0;i<goodTrackJets.size();++i) {
        if( trackJets[goodTrackJets[i]].eta() < (etaBounds[1]-0.4) && trackJets[goodTrackJets[i]].eta() > (etaBounds[0]+0.4) )
            n++;
    }
    return n;
}
float EventContainer::trackjetndensity() const
{
    vector<float> etaBounds = {trackJets[goodTrackJets[0]].eta(), trackJets[goodTrackJets[1]].eta()};
    sort(etaBounds.begin(), etaBounds.end());
    
    // Calculate eta range within tracker
    if( etaBounds[0] >= 2.1 || etaBounds[1] <= -2.1 ) return 0;
    if( etaBounds[0] < -2.9 ) etaBounds[0] = -2.9;
    if( etaBounds[1] > 2.9 ) etaBounds[1] = 2.9;
    float etaRange = abs(trackJets[goodTrackJets[0]].eta() - trackJets[goodTrackJets[1]].eta()) -0.8;
    if( etaRange > 0 )
        return (trackjetn()/etaRange);
    else
        return 0;
}

// Isolated jets
float EventContainer::isolatedjetpt() const
{
    for( auto iJet : goodJets )
    {
        bool farFromLepton = true;
        for( unsigned int iLepton=0; iLepton < looseLeptons.size(); ++iLepton )
        {
            if( looseLeptons[iLepton].pt() > 0. )
            {
                if( jets[iJet].dR(looseLeptons[iLepton]) < 1. )
                {
                    farFromLepton = false;
                    break;
                }
            }
            else
                break;
        }
        if( farFromLepton ) 
            return jets[iJet].pt();
    }
    return -9999.9;    
}
float EventContainer::isolatedbjetpt(float btag) const
{
    for( auto iJet : goodJets )
    {
        // Skip non-b jets
        if( jets[iJet].csv() < btag )
            continue;
        bool farFromLepton = true;
        for( unsigned int iLepton=0; iLepton < looseLeptons.size(); ++iLepton )
        {
            if( looseLeptons[iLepton].pt() > 0. )
            {
                if( jets[iJet].dR(looseLeptons[iLepton]) < 1. )
                {
                    farFromLepton = false;
                    break;
                }
            }
            else
                break;
        }
        if( farFromLepton ) 
            return jets[iJet].pt();
    }
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
float EventContainer::leptonabseta(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return abs(leptons[goodLeptons[i]].eta());
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
float EventContainer::leptoncharge(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].charge();
    else
        return -9999.9;
}
unsigned int EventContainer::nLeptons( float minPt ) const
{
    for( unsigned int i = 0; i < goodLeptons.size(); ++i )
    {
        if( leptons[goodLeptons[i]].pt() < minPt )
            return i;
    }
    return goodLeptons.size();
}
float EventContainer::leptonclosestjetpt(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].closestJetPt();
    else
        return -9999.9;
}
float EventContainer::leptonclosestjetdr(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].closestJetDr();
    else
        return -9999.9;
}
float EventContainer::leptonclosestjetpartonflavour(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].closestJetPartonFlavour();
    else
        return -9999.9;
}
float EventContainer::flavourleptonpt(float flavour) const
{
    for( unsigned int i = 0; i < goodLeptons.size(); ++i )
    {
        if( looseLeptons[i].pt() > 0 && ((flavour == 11 && looseLeptons[i].isElectron()) || (flavour == 13 && looseLeptons[i].isMuon()))  )
            return looseLeptons[i].pt();           
    }
    return -9999.9;
}
float EventContainer::leptoncorrectedpt(unsigned int i) const
{
    if( i < goodLeptons.size() )
    {
        if( leptons[goodLeptons[i]].isElectron() )
        {
            double isoCut = 0.0354;
            if( abs(leptons[goodLeptons[i]].eta()) > 1.479 )
                isoCut = 0.0646;
            return leptons[goodLeptons[i]].pt() * ( 1. + max(0., leptons[goodLeptons[i]].isolation() - isoCut) );
        }
        else if( leptons[goodLeptons[i]].isMuon() )
            return leptons[goodLeptons[i]].pt() * ( 1. + max(0., leptons[goodLeptons[i]].isolation() - 0.15) );
        else
            return -9999.9;
    }            
    else
        return -9999.9;
}
float EventContainer::leptonisolation(unsigned int i) const
{
    if( i < goodLeptons.size() )
        return leptons[goodLeptons[i]].isolation();
    else
        return -9999.9;
}

// Loose leptons
float EventContainer::looseleptonpt(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].pt();
    else
        return -9999.9;
}
float EventContainer::looseleptoneta(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].eta();
    else
        return -9999.9;
}
float EventContainer::looseleptonabseta(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return abs(looseLeptons[i].eta());
    else
        return -9999.9;
}
float EventContainer::looseleptonphi(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].phi();
    else
        return -9999.9;
}
float EventContainer::looseleptonflavour(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return abs(looseLeptons[i].pId());
    else
        return -9999.9;
}
float EventContainer::loosemllminpt(float subtractMass, float minPt) const
{
    float dmll = -9999.9;
    for( unsigned int iLep1=0; iLep1 < looseLeptons.size(); ++iLep1 )
    {
        for( unsigned int iLep2=iLep1+1; iLep2 < looseLeptons.size(); ++iLep2 )
        {
            if( looseLeptons[iLep2].pt() < minPt )
                break;
            
            // check opposite charge and same flavour
            if( looseLeptons[iLep1].charge() * looseLeptons[iLep2].charge() < 0 && abs(looseLeptons[iLep1].pId()) == abs(looseLeptons[iLep2].pId()) )
            {
                float dmll_temp = looseLeptons[iLep1].mpp(looseLeptons[iLep2]) - subtractMass;
                if( abs(dmll_temp) < abs(dmll) )
                    dmll = dmll_temp;
            }
        }
    }
    return dmll;
}
float EventContainer::nlooseleptons(float minPt) const
{
    for( unsigned int i = 0; i < looseLeptons.size(); ++i )
    {
        if( looseLeptons[i].pt() <= minPt )
            return i;
    }
    return looseLeptons.size();
}
float EventContainer::looseleptonclosestjetpt(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].closestJetPt();
    else
        return -9999.9;
}
float EventContainer::looseleptonclosestjetdr(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].closestJetDr();
    else
        return -9999.9;
}
float EventContainer::looseleptonclosestjetpartonflavour(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].closestJetPartonFlavour();
    else
        return -9999.9;
}
float EventContainer::looseflavourleptonpt(float flavour) const
{
    for( unsigned int i = 0; i < looseLeptons.size(); ++i )
    {
        if( looseLeptons[i].pt() > 0 && ((flavour == 11 && looseLeptons[i].isElectron()) || (flavour == 13 && looseLeptons[i].isMuon()))  )
            return looseLeptons[i].pt();
    }
    return -9999.9;
}
float EventContainer::looseleptoncorrectedpt(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
    {
        if( looseLeptons[i].isElectron() )
        {
            double isoCut = 0.0354;
            if( abs(looseLeptons[i].eta()) > 1.479 )
                isoCut = 0.0646;
            return looseLeptons[i].pt() * ( 1. + max(0., looseLeptons[i].isolation() - isoCut) );
        }
        else if( looseLeptons[i].isMuon() )
            return looseLeptons[i].pt() * ( 1. + max(0., looseLeptons[i].isolation() - 0.15) );
        else
            return -9999.9;
    }            
    else
        return -9999.9;
}
float EventContainer::looseleptonisolation(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].isolation();
    else
        return -9999.9;
}
float EventContainer::looseleptonpasstight(unsigned int i) const
{
    if( looseLeptons[i].pt() > 0 )
        return looseLeptons[i].passesMedium();
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
float EventContainer::dmll(float subtractMass) const
{    
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].mpp(leptons[goodLeptons[1]]) - subtractMass;
    else
        return -9999.9;
}
float EventContainer::dmee(float subtractMass) const
{    
    if( goodLeptons.size() > 1 && leptons[goodLeptons[0]].isElectron() && leptons[goodLeptons[1]].isElectron())
        return leptons[goodLeptons[0]].mpp(leptons[goodLeptons[1]]) - subtractMass;
    else
        return -9999.9;
}
float EventContainer::dmllminpt(float subtractMass, float minPt) const
{
    float dmll = -9999.9;
    for( auto iLep1 : goodLeptons )
    {
        for( auto iLep2 : goodLeptons )
        {
            if( iLep1 >= iLep2) 
                continue;
            if( leptons[iLep2].pt() < minPt )
                break;
            
            // check opposite charge and same flavour
            if( leptons[iLep1].charge() * leptons[iLep2].charge() < 0 && abs(leptons[iLep1].pId()) == abs(leptons[iLep2].pId()) )
            {
                float dmll_temp = leptons[iLep1].mpp(leptons[iLep2]) - subtractMass;
                if( abs(dmll_temp) < abs(dmll) )
                    dmll = dmll_temp;
            }
        }
    }
    return dmll;
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
        return abs(leptons[goodLeptons[0]].dEta(leptons[goodLeptons[1]]));
    else
        return -9999.9;
}
float EventContainer::drll() const
{
    if( goodLeptons.size() > 1 )
        return leptons[goodLeptons[0]].dR(leptons[goodLeptons[1]]);
    else
        return -9999.9;
}
float EventContainer::detajj() const
{
    if( goodJets.size() > 1 )
        return abs(jets[goodJets[0]].dEta(jets[goodJets[1]]));
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
float EventContainer::productleptoncharge() const
{
    if( goodLeptons.size() > 1 )
    {
        return leptons[goodLeptons[0]].charge() * leptons[goodLeptons[1]].charge();
    }
    else
        return -9999.9;
}
float EventContainer::genmll() const
{
//     if( goodGenLeptons.size() > 1 )
//         return genLeptons[goodGenLeptons[0]].mpp(genLeptons[goodGenLeptons[1]]);
//     else
//         return -9999.9;
    for( auto iLep1 : goodGenLeptons )
    {
        for( auto iLep2 : goodGenLeptons )
        {
            if( iLep1 >= iLep2) 
                continue;
            
            if( /*( abs(genLeptons[iLep1].pId()) == abs(genLeptons[iLep2].pId()) ) && (genLeptons[iLep1].charge()*genLeptons[iLep2].charge() == -1 ) &&*/ genLeptons[iLep1].isPrompt() && genLeptons[iLep2].isPrompt() )
            { 
                return genLeptons[iLep1].mpp(genLeptons[iLep2]);
                break;
            }           
        }
    }
    return -9999.9;
}
float EventContainer::genchannel() const
{
    for( auto iLep1 : goodGenLeptons )
    {
        for( auto iLep2 : goodGenLeptons )
        {
            if( iLep1 >= iLep2) 
                continue;
            
            if( /*( abs(genLeptons[iLep1].pId()) == abs(genLeptons[iLep2].pId()) ) && (genLeptons[iLep1].charge() != genLeptons[iLep2].charge() ) &&*/ genLeptons[iLep1].isPrompt() && genLeptons[iLep2].isPrompt() )
            { 
                if( genLeptons[iLep1].isElectron() )
                {
                    if( genLeptons[iLep2].isElectron() ) 
                        return 1.; // ee
                    else 
                        return 2; // em
                }
                else
                {
                    if( genLeptons[iLep2].isElectron() ) 
                        return 3.; // me
                    else 
                        return 0; // mm
                }
            }
        }
    }
    return -9999.9;
}
float EventContainer::drjjll() const
{
    if( goodLeptons.size() > 1 && goodJets.size() > 1 )
    {
        TLorentzVector jetV = jets[goodJets[0]].getLorentzVector() + jets[goodJets[1]].getLorentzVector();
        TLorentzVector leptonV = leptons[goodLeptons[0]].getLorentzVector() + leptons[goodLeptons[0]].getLorentzVector();
        return jetV.DeltaR( leptonV );
    }
    else
        return -9999.9;
}

float EventContainer::zeppenfeldlep(unsigned int index) const
{
    if( goodLeptons.size() > index && goodJets.size() > 1 )
    {
        return ( leptons[goodLeptons[index]].eta()-(jets[goodJets[0]].eta()+jets[goodJets[1]].eta())/2 ) / abs(jets[goodJets[0]].dEta(jets[goodJets[1]]));       
    }
    else
        return -9999.9;
}

float EventContainer::mt() const
{
    return sqrt( pow(looseLeptons[0].pt()+met.pt(),2) - pow(looseLeptons[0].px()+met.px(),2) - pow(looseLeptons[0].py()+met.py(),2) );
}

bool EventContainer::passZjetFR(float flavour) const
{
    const Lepton *lZ1 = nullptr, *lZ2 = nullptr, *l = nullptr;
    for( unsigned int iLepton=0; iLepton < looseLeptons.size(); ++iLepton ) {   
        if( looseLeptons[iLepton].pt() <= 0 )
            break;
        if( (flavour == 13. && looseLeptons[iLepton].isElectron()) || (flavour == 11. && looseLeptons[iLepton].isMuon()) )
        {
            if( lZ1 )
                lZ2 = &(looseLeptons[iLepton]);
            else
                lZ1 = &(looseLeptons[iLepton]);
        }
        else
            l = &(looseLeptons[iLepton]);
    }
    if( !lZ1 || !lZ2 || !l )
        return false;
    if( lZ1->charge() * lZ2->charge() != -1 )
        return false;
    if( abs(lZ1->mpp(*lZ2) -91) > 15 ) 
        return false;
    
//     leptons[goodLeptons[2]] = *l;
//     looseLeptons[2] = *l;
    return true;
}
float EventContainer::loosechannel() const
{
    if( looseLeptons[1].pt() > 0 )
    {
        if( looseLeptons[0].isElectron() )
        {
            if(  looseLeptons[1].isElectron() ) 
                return 1.; // ee
            else 
                return 2; // em
        }
        else
        {
            if(  looseLeptons[1].isElectron() ) 
                return 3.; // me
            else 
                return 0; // mm
        }
    }
    else
        return -9999.9;
}
float EventContainer::looseproductleptoncharge() const
{
    if( looseLeptons[1].pt() > 0 )
    {
        return looseLeptons[0].charge() * looseLeptons[1].charge();
    }
    else
        return -9999.9;
}
float EventContainer::loosemll() const
{
    if( looseLeptons[1].pt() > 0 )
    {
        return looseLeptons[0].mpp(looseLeptons[1]);
    }
    else
        return -9999.9;    
}
float EventContainer::detametl() const
{
    float mindeta = 9999.9;
    for( auto iLep : goodLeptons )
    {
        mindeta = min( abs(leptons[goodLeptons[iLep]].dEta(met)), mindeta ); 
    }
    return mindeta;   
}
float EventContainer::mmetl() const
{
    int closestLepIndex = -1;
    float mindeta = 9999.9;
    for( auto iLep : goodLeptons )
    {
        float dEta = abs(leptons[goodLeptons[iLep]].dEta(met));
        if( dEta < mindeta )
        {
            closestLepIndex = iLep;
            mindeta = dEta; 
        } 
    }
    if( closestLepIndex > -1 )
        return leptons[closestLepIndex].mpp(met);
    else 
        return 9999.9;
}

float EventContainer::weight() const
{
    return _weight*_reweight;
}
void EventContainer::setWeight(float weight)
{
    _weight = weight;    
}
float EventContainer::reweight() const
{
    return _reweight;
}
void EventContainer::setReweight(float weight)
{
    _reweight = weight;    
}
void EventContainer::setFakeWeights(float upWeight, float downWeight)
{
    _upWeight = upWeight;  
    _downWeight = downWeight;  
}
float EventContainer::getUpWeight(float) const
{
    return _upWeight;
}
float EventContainer::getDownWeight(float) const
{
    return _downWeight;
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

// Event number
float EventContainer::eventNo() const
{
    return _eventNo;
}
void EventContainer::setEventNo(float eventNo)
{
    _eventNo = eventNo;
}