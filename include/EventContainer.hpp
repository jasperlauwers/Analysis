
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
    double jetpt(unsigned int) const;
    double jeteta(unsigned int) const;
    double jetphi(unsigned int) const;
    double jetmass(unsigned int) const;
    double jetcsv(unsigned int) const;
    unsigned int nJets( float minPt ) const;
    
    // Gen jets
    double genjetpt(unsigned int) const;
    double genjeteta(unsigned int) const;
    double genjetphi(unsigned int) const;
    double genjetmass(unsigned int) const;
    
    // Puppi jets
    
    // Leptons
    double leptonpt(unsigned int) const;
    double leptoneta(unsigned int) const;
    double leptonphi(unsigned int) const;
    
    // Gen leptons
    double genleptonpt(unsigned int) const;
    double genleptoneta(unsigned int) const;
    double genleptonphi(unsigned int) const;
    
    // Di-particle varibles
    double mll() const;
    double mjj() const;
    double ptll() const;
    double ptjj() const;
    double detall() const;
    double detajj() const;
    double dphill() const;
    double dphijj() const;
    double channel() const;
    double genmll() const;
    double genchannel() const;
    
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