
#ifndef EventContainer_hpp
#define EventContainer_hpp

#include "Jet.hpp"
#include "Lepton.hpp"

#include <vector>

using namespace std;

class EventContainer {

public:
    EventContainer();
    EventContainer(const unsigned int leptonSize, const unsigned int jetSize);
    ~EventContainer();
    void init(const unsigned int leptonSize, const unsigned int jetSize);
    void reset(); 
    double jetpt(unsigned int) const;
    double jeteta(unsigned int) const;
    double jetphi(unsigned int) const;
    double jetmass(unsigned int) const;
    double leptonpt(unsigned int) const;
    double leptoneta(unsigned int) const;
    double leptonphi(unsigned int) const;
    double mll() const;
    double mjj() const;
    double ptll() const;
    double ptjj() const;
    double detall() const;
    double detajj() const;
    double dphill() const;
    double dphijj() const;
    double channel() const;
    
    float weight() const;
    void setWeight(float);
    
    unsigned int iEvent;
    vector<Lepton> leptons;
    vector<unsigned int> goodLeptons;
    vector<Jet> jets;
    vector<unsigned int> goodJets;
    
private:
    float _weight;
};


#endif