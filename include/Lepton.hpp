
#ifndef Lepton_hpp
#define Lepton_hpp

#include "Particle.hpp"

class Lepton : public Particle {
    
public:
    Lepton();
    Lepton(double pt, double eta, double phi, double pId);
    Lepton(const TLorentzVector&, double pId);
    virtual ~Lepton();
    void set(double pt, double eta, double phi, double pId);
    void set(double pt, double eta, double phi, double pId, bool passMedium);
    bool isElectron() const;
    bool isMuon() const;
    bool passesMedium() const;
    double charge() const;
    
private:
    double pId, passMedium; 
    
};

#endif