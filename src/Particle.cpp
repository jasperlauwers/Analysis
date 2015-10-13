/*
 * Particle class 
 * 
 */

#include "Particle.hpp"

Particle::Particle() { }

Particle::Particle(double pt, double eta, double phi, double mass)
{
    p4.SetPtEtaPhiM(pt, eta, phi, mass);
}

Particle::Particle(const TLorentzVector& p4Vec) : p4( p4Vec ) { }

Particle::~Particle() { }
    
void Particle::set(double pt, double eta, double phi, double mass) 
{
    p4.SetPtEtaPhiM(pt, eta, phi, mass);
}  

const TLorentzVector& Particle::getLorentzVector() const
{
    return p4;
}

double Particle::pt() const
{
    return p4.Pt();
}
double Particle::eta() const
{
    return p4.Eta();
}
double Particle::phi() const
{
    return p4.Phi();
}
double Particle::mass() const
{
    return p4.M();
}
    
double Particle::dR(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.DeltaR( lorentzV );
}

double Particle::dPhi(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.DeltaPhi( lorentzV );
}

double Particle::dEta(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return p4.Eta() - lorentzV.Eta();
}

double Particle::mpp(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return (p4+lorentzV).M() ;
}

double Particle::ptpp(const Particle& particle) const 
{
    const TLorentzVector& lorentzV = particle.getLorentzVector();
    return (p4+lorentzV).Pt() ;
}