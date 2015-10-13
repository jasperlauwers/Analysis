#ifndef ConfigReader_hpp
#define ConfigReader_hpp

#include "libconfig.h++"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
using namespace libconfig;

class ConfigReader {
public:
    ConfigReader(const string& fileName);
    virtual ~ConfigReader();
    
protected:
    Config cfg;
    
};
#endif