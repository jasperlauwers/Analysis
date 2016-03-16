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
    Particle(float pt, float eta, float phi, float mass = 0);
    Particle(const TLorentzVector&);
    virtual ~Particle();
    void set(float pt, float eta, float phi, float mass = 0);
    const TLorentzVector& getLorentzVector() const;
    float px() const;
    float py() const;
    float pt() const;
    float eta() const;
    float phi() const;
    float mass() const;
    float dR(const Particle&) const;
    float dPhi(const Particle&) const;
    float dEta(const Particle&) const;
    float mpp(const Particle&) const;
    float ptpp(const Particle&) const;
    
protected:
    TLorentzVector p4;    
};

#endif