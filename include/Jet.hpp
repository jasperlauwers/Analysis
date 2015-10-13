
#ifndef Jet_hpp
#define Jet_hpp

#include "Particle.hpp"

class Jet : public Particle {
    
public:
    Jet();
    Jet(double pt, double eta, double phi, double mass = 0, double puId = 0, double qgLikelihood = 0, double CSV = 0);
    Jet(const TLorentzVector&, double puId = 0, double qgLikelihood = 0, double CSV = 0);
    virtual ~Jet();
    void set(double pt, double eta, double phi, double mass, double puId, double qgLikelihood, double CSV);
    void set(double pt, double eta, double phi, double mass = 0);
    
private:
    double puId, qgLikelihood, CSV; 
    
};

#endif