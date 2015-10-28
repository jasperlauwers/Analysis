
#ifndef Met_hpp
#define Met_hpp

#include "Particle.hpp"

class Met : public Particle {
    
public:
    Met();
    Met(float pt, float phi);
    Met(const TLorentzVector&);
    virtual ~Met();
    void set(float pt, float phi);    
};

#endif