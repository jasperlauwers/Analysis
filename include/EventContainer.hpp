
#ifndef EventContainer_hpp
#define EventContainer_hpp

#include "Jet.hpp"
#include "Lepton.hpp"
#include "Met.hpp"

#include <vector>

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
    
    // Gen jets
    float genjetpt(unsigned int) const;
    float genjeteta(unsigned int) const;
    float genjetphi(unsigned int) const;
    float genjetmass(unsigned int) const;
    
    // Puppi jets
    float puppijetpt(unsigned int) const;
    float puppijeteta(unsigned int) const;
    float puppijetphi(unsigned int) const;
    float puppijetmass(unsigned int) const;
    
    // Leptons
    float leptonpt(unsigned int) const;
    float leptoneta(unsigned int) const;
    float leptonphi(unsigned int) const;
    float leptoncharge(unsigned int) const;
    
    // Gen leptons
    float genleptonpt(unsigned int) const;
    float genleptoneta(unsigned int) const;
    float genleptonphi(unsigned int) const;
    
    // Di-particle varibles
    float mll() const;
    float dmll(float subtractMass) const;
    float dmllminpt(float subtractMass, float minPt) const;
    float mjj() const;
    float ptll() const;
    float ptjj() const;
    float detall() const;
    float detajj() const;
    float dphill() const;
    float dphijj() const;
    float channel() const;
    float productleptoncharge() const;
    float genmll() const;
    float genchannel() const;
    
    // Weight
    float weight() const;
    void setWeight(float);
    
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
    
    // Data members
    unsigned int iEvent;
    vector<Lepton> leptons, genLeptons;
    vector<unsigned int> goodLeptons, goodGenLeptons;
    vector<Jet> jets, genJets, puppiJets;
    vector<unsigned int> goodJets, goodGenJets, goodPuppiJets;
    Met met;
    
private:
    float _weight;
    float _nVertices;
};


#endif