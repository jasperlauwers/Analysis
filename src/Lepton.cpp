#include "Lepton.hpp"

Lepton::Lepton() { }

Lepton::Lepton(float pt, float eta, float phi, float pId) 
: Particle( pt, eta, phi, 0), _pId(pId) { }

Lepton::Lepton(const TLorentzVector&  p4, float pId) 
: Particle( p4 ), _pId(pId) { }

Lepton::~Lepton() { }

void Lepton::set(float pt, float eta, float phi, float pId) 
{
    Particle::set( pt, eta, phi);
    _pId = pId;
}

void Lepton::set(float pt, float eta, float phi, float pId, bool passMedium) 
{
    Particle::set( pt, eta, phi);
    _pId = pId;
    this->passMedium = passMedium;
}
bool Lepton::isElectron() const 
{
    if( abs(abs(_pId) - 11.) < 0.001  ) return true;
    else return false;
}
bool Lepton::isMuon() const 
{
    if( abs(abs(_pId) - 13.) < 0.001 ) return true;
    else return false;    
}
bool Lepton::isTau() const 
{
    if( abs(abs(_pId) - 15.) < 0.001 ) return true;
    else return false;    
}
float Lepton::pId() const 
{
    return _pId;    
}


bool Lepton::passesMedium() const 
{
    return passMedium;   
}

float Lepton::charge() const 
{
    return (_pId < 0) - (_pId > 0);
}

void Lepton::setGenFlags(bool isPrompt, bool fromTau)
{
    _isPrompt = isPrompt;
    _fromTau = fromTau;    
}
bool Lepton::isPrompt() const
{
    return _isPrompt;    
}
bool Lepton::fromTau() const
{
    return _fromTau;    
}

void Lepton::setd0( float d0 ) 
{
    _d0 = d0;
}
void Lepton::setdEtaIn( float dEtaIn ) 
{
    _dEtaIn = dEtaIn;
}
void Lepton::setdPhiIn( float dPhiIn ) 
{
    _dPhiIn = dPhiIn;
}
void Lepton::setdz( float dz ) 
{
    _dz = dz;
}
void Lepton::seteffectiveArea( float effArea ) 
{
    _effArea = effArea;
}
void Lepton::setmissingHits( float missHits ) 
{
    _missHits = missHits;
}
void Lepton::setsigmaIetaIeta( float sIetaIeta ) 
{
    _sIetaIeta = sIetaIeta;
}
void Lepton::sethOverE( float hOverE ) 
{
    _hOverE = hOverE;
}
void Lepton::setooEmoop( float ooEmoop ) 
{
    _ooEmoop = ooEmoop;
}
void Lepton::setconversionVeto( float convVeto ) 
{
    _convVeto = convVeto;
}
void Lepton::setscEta( float scEta ) 
{
    _scEta = scEta;
}
void Lepton::setIsolation( float isolation ) 
{
    _isolation = isolation;
}

float Lepton::d0() const
{
    return _d0;
}
float Lepton::dEtaIn() const
{
    return _dEtaIn;
}
float Lepton::dPhiIn() const
{
    return _dPhiIn;
}
float Lepton::dz() const
{
    return _dz;
}
float Lepton::effectiveArea() const
{
    return _effArea;
}
float Lepton::missingHits() const
{
    return _missHits;
}
float Lepton::sigmaIetaIeta() const
{
    return _sIetaIeta;
}
float Lepton::hOverE() const
{
    return _hOverE;
}
float Lepton::ooEmoop() const
{
    return _ooEmoop;
}
float Lepton::conversionVeto() const
{
    return _convVeto;
}
float Lepton::scEta() const
{
    return _scEta;
}
float Lepton::isolation() const
{
    return _isolation;
}
