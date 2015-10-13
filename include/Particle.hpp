/*
 * Particle class 
 * 
 */

#ifndef Particle_hpp
#define Particle_hpp

#include "TLorentzVector.h"

class Particle {
    
public:
    Particle();
    Particle(double pt, double eta, double phi, double mass = 0);
    Particle(const TLorentzVector&);
    virtual ~Particle();
    void set(double pt, double eta, double phi, double mass = 0);
    const TLorentzVector& getLorentzVector() const;
    double pt() const;
    double eta() const;
    double phi() const;
    double mass() const;
    double dR(const Particle&) const;
    double dPhi(const Particle&) const;
    double dEta(const Particle&) const;
    double mpp(const Particle&) const;
    double ptpp(const Particle&) const;
    
protected:
    TLorentzVector p4;    
};

#endif