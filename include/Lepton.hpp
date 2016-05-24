
#ifndef Lepton_hpp
#define Lepton_hpp

#include "Particle.hpp"

class Lepton : public Particle {
    
public:
    Lepton();
    Lepton(float pt, float eta, float phi, float pId);
    Lepton(const TLorentzVector&, float pId);
    virtual ~Lepton();
    void set(float pt, float eta, float phi, float pId);
    void set(float pt, float eta, float phi, float pId, bool passMedium);
    bool isElectron() const;
    bool isMuon() const;
    bool isTau() const;
    float pId() const; 
    bool passesMedium() const;
    float charge() const;
    
    // gen parameters
    void setGenFlags(bool isPrompt, bool fromTau);
    bool isPrompt() const;
    bool fromTau() const;
    
    // electron id parameters
    void setd0(float);
    void setdEtaIn(float); 
    void setdPhiIn(float); 
    void setdz(float); 
    void seteffectiveArea(float); 
    void setmissingHits(float); 
    void setsigmaIetaIeta(float); 
    void sethOverE(float); 
    void setooEmoop(float); 
    void setconversionVeto(float); 
    void setscEta(float); 
    void setIsolation(float);
    void setTrackIso(float);
    float d0() const;
    float dEtaIn() const;
    float dPhiIn() const;
    float dz() const;
    float effectiveArea() const;
    float missingHits() const;
    float sigmaIetaIeta() const;
    float hOverE() const;
    float ooEmoop() const;
    float conversionVeto() const;
    float scEta() const;
    float isolation() const;
    float trackIso() const;
    
    // fake rate parameters
    void setClosestJetPt(float);
    float closestJetPt() const;
    void setClosestJetDr(float);
    float closestJetDr() const;
    void setClosestJetPartonFlavour(float);
    float closestJetPartonFlavour() const;
    
private:
    float _pId;
    bool passMedium, _isPrompt, _fromTau; 
    float _d0, _dEtaIn, _dPhiIn, _dz, _effArea, _missHits, _sIetaIeta, _hOverE, _ooEmoop, _convVeto, _scEta, _isolation, _trackIso;
    float _closestJetPt, _closestJetDr, _closestJetPartonFlavour;
};

#endif