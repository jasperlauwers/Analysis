#include "Lepton.hpp"

Lepton::Lepton() { }

Lepton::Lepton(double pt, double eta, double phi, double pId) 
: Particle( pt, eta, phi, 0), pId(pId) { }

Lepton::Lepton(const TLorentzVector&  p4, double pId) 
: Particle( p4 ), pId(pId) { }

Lepton::~Lepton() { }

void Lepton::set(double pt, double eta, double phi, double pId) 
{
    Particle::set( pt, eta, phi);
    this->pId = pId;
}

void Lepton::set(double pt, double eta, double phi, double pId, bool passMedium) 
{
    Particle::set( pt, eta, phi);
    this->pId = pId;
    this->passMedium = passMedium;
}

bool Lepton::isElectron() const {
    if( abs(pId) == 11 ) return true;
    else return false;
}

bool Lepton::isMuon() const {
    if( abs(pId) == 13 ) return true;
    else return false;    
}

bool Lepton::passesMedium() const {
    return passMedium;   
}

double Lepton::charge() const {
    return (pId < 0) - (pId > 0);
}