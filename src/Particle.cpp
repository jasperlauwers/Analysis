/*
 * Particle class 
 * 
 */

#include "Particle.hpp"

Particle::Particle() { }

Particle::Particle(float pt, float eta, float phi, float mass)
{
    p4.SetPtEtaPhiM(pt, eta, phi, mass);
}

Particle::Particle(const TLorentzVector& p4Vec) : p4( p4Vec ) { }

Particle::~Particle() { }
    
void Particle::set(float pt, float eta, float phi, float mass) 
{
    p4.SetPtEtaPhiM(pt, eta, phi, mass);
}  

const TLorentzVector& Particle::getLorentzVector() const
{
    return p4;
}

float Particle::pt() const
{
    return p4.Pt();
}
float Particle::eta() const
{
    return p4.Eta();
}
float Particle::phi() const
{
    return p4.Phi();
}
float Particle::mass() const
{
    return p4.M();
}
    
float Particle::dR(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.DeltaR( lorentzV );
}

float Particle::dPhi(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.DeltaPhi( lorentzV );
}

float Particle::dEta(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.Eta() - lorentzV.Eta();
}

float Particle::mpp(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return (p4+lorentzV).M() ;
}

float Particle::ptpp(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return (p4+lorentzV).Pt() ;
}