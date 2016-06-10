
#include "Jet.hpp"

Jet::Jet() { }

Jet::Jet(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
    : Particle( pt, eta, phi, mass), _puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::Jet(const TLorentzVector& p4, float puId, float qgLikelihood, float CSV) 
: Particle( p4 ), _puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::~Jet() { }

void Jet::set(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
{
    Particle::set( pt, eta, phi, mass);
    _puId = puId;
    this->qgLikelihood = qgLikelihood;
    this->CSV = CSV;
}

void Jet::set(float pt, float eta, float phi, float mass) 
{
    Particle::set(pt, eta, phi, mass);
}

void Jet::setCsv( float csv )
{
    CSV = csv;
}
void Jet::setPuId( float puId )
{
    _puId = puId;
}
void Jet::setIsHardProcess( float isHardProcess )
{
    _isHardProcess = isHardProcess;
}
void Jet::setSoftMuPt( float softMuPt )
{
    _softMuPt = softMuPt;
}
float Jet::csv() const
{
    return CSV;
}
float Jet::puId() const
{
    return _puId;
}
bool Jet::isHardProcess() const
{
    return _isHardProcess;
}
float Jet::softMuPt() const
{
    return _softMuPt;
}