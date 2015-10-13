
#ifndef CutReader_hpp
#define CutReader_hpp

#include "CutContainer.cpp"
#include "ConfigReader.hpp"

#include <vector>
#include <iostream>

class CutReader : public ConfigReader {
public:
    CutReader(const string& fileName, CutContainer& cutContainer);
    virtual ~CutReader();
    
private:
    CutContainer& cutContainer;
    
};
#endif