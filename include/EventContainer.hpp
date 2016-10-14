
#ifndef EventContainer_hpp
#define EventContainer_hpp

#include "Jet.hpp"
#include "Lepton.hpp"
#include "Met.hpp"

#include <vector>
#include <iostream>

using namespace std;

class EventContainer {

public:
    EventContainer();
    EventContainer(const unsigned int leptonSize, const unsigned int jetSize);
    ~EventContainer();
    void init(const unsigned int leptonSize, const unsigned int jetSize);
    void reset(); 
    
    // Jets
    float jetpt(unsigned int) const;
    float jeteta(unsigned int) const;
    float jetphi(unsigned int) const;
    float jetmass(unsigned int) const;
    float jetcsv(unsigned int) const;
    unsigned int nJets( float minPt ) const;
    float jetmaxcsv( float minPt ) const;
    float jetmaxSoftMuonPt( float minPt ) const;
    
    // Gen jets
    float genjetpt(unsigned int) const;
    float genjeteta(unsigned int) const;
    float genjetphi(unsigned int) const;
    float genjetmass(unsigned int) const;
    float genjetnhardprocess() const;
    
    // Puppi jets
    float puppijetpt(unsigned int) const;
    float puppijeteta(unsigned int) const;
    float puppijetphi(unsigned int) const;
    float puppijetmass(unsigned int) const;
    
    // Track jets
    float trackjetht() const;
    float trackjethtdensity() const;
    unsigned int trackjetn() const;
    float trackjetndensity() const;
    
    // Isolated jets
    float isolatedjetpt() const;
    float isolatedbjetpt(float btag) const;
    
    // Leptons
    float leptonpt(unsigned int) const;
    float leptoneta(unsigned int) const;
    float leptonabseta(unsigned int) const;
    float leptonphi(unsigned int) const;
    float leptoncharge(unsigned int) const;
    unsigned int nLeptons( float minPt ) const;
    float leptonclosestjetpt(unsigned int) const;
    float leptonclosestjetdr(unsigned int) const;
    float leptonclosestjetpartonflavour(unsigned int) const;
    float flavourleptonpt(float) const;
    float leptoncorrectedpt(unsigned int) const;
    float leptonisolation(unsigned int) const;
    bool leptontriplechargeagreement(unsigned int) const;
    
    // Loose leptons
    float looseleptonpt(unsigned int) const;
    float looseleptoneta(unsigned int) const;
    float looseleptonabseta(unsigned int) const;
    float looseleptonphi(unsigned int) const;
    float looseleptonflavour(unsigned int) const;
    float loosemllminpt(float subtractMass, float minPt) const;
    float nlooseleptons(float minPt) const;
    float looseleptonclosestjetpt(unsigned int) const;
    float looseleptonclosestjetdr(unsigned int) const;
    float looseleptonclosestjetpartonflavour(unsigned int) const;
    float looseflavourleptonpt(float) const;
    float looseleptoncorrectedpt(unsigned int) const;
    float looseleptonisolation(unsigned int) const;
    float looseleptonpasstight(unsigned int) const;
    
    // Gen leptons
    float genleptonpt(unsigned int) const;
    float genleptoneta(unsigned int) const;
    float genleptonphi(unsigned int) const;
    
    // Di-particle varibles
    float mll() const;
    float dmll(float subtractMass) const;
    float dmee(float subtractMass) const;
    float dmllminpt(float subtractMass, float minPt) const;
    float mjj() const;
    float ptll() const;
    float ptjj() const;
    float detall() const;
    float drll() const;
    float detajj() const;
    float dphill() const;
    float dphijj() const;
    float channel() const;
    float productleptoncharge() const;
    float genmll() const;
    float genchannel() const;
    float drjjll() const;
    float zeppenfeldlep(unsigned int index) const;
    float mt() const; //  Transversal Mass ( loose lepton, MET) 
    bool passZjetFR(float flavour) const; 
    float loosechannel() const;
    float looseproductleptoncharge() const;
    float loosemll() const;
    float detametl() const;
    float mmetl() const;
    
    // Weight
    float weight() const;
    float reweight() const;
    void setWeight(float);
    void setReweight(float);
    void setFakeWeights(float, float);
    float getUpWeight(float) const;
    float getDownWeight(float) const;
    
    // Number of vertices
    float nvertices() const;
    void setNvertices(float);
    
    // Electron id parameters
    float electrond0(unsigned int) const;
    float electrondEtaIn(unsigned int) const;
    float electrondPhiIn(unsigned int) const;
    float electrondz(unsigned int) const;
    float electroneffectiveArea(unsigned int) const;
    float electronmissingHits(unsigned int) const;
    float electronsigmaIetaIeta(unsigned int) const;
    float electronhOverE(unsigned int) const;
    float electronooEmoop(unsigned int) const;
    float electronconversionVeto(unsigned int) const;
    float electronscEta(unsigned int) const;
    float electronisolation(unsigned int) const;
    
    // Event number
    float eventNo() const;
    void setEventNo(float); 
    
    // Data members
    unsigned int iEvent;
    vector<Lepton> leptons, genLeptons, looseLeptons;
    vector<unsigned int> goodLeptons, goodGenLeptons;
    vector<Jet> jets, genJets, puppiJets, trackJets;
    vector<unsigned int> goodJets, goodGenJets, goodPuppiJets, goodTrackJets;
    Met met;
    
private:
    float _weight, _reweight, _upWeight, _downWeight;
    float _nVertices;
    unsigned int _eventNo;
};


#endif