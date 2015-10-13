#ifndef ConfigHandler_hpp
#define ConfigHander_hpp

#include "libconfig.h++"
#include <iostream>
#include <string>
#include "VariableReader.hpp"
#include "SampleReader.hpp"
#include "CutReader.hpp"
#include "ConfigContainer.cpp"

using namespace std;
using namespace libconfig;

class ConfigHandler : public ConfigReader {
public:
    ConfigHandler(const string& fileName, ConfigContainer& cContainer);
    virtual ~ConfigHandler();
    void setSampleContainer(SampleContainer&);
    void setVariableContainer(VariableContainer&);
    void setCutContainer(CutContainer&);
    const Config& getConfig() const;
    void readConfig();
    
protected:
    bool hasSamples, hasVariables, hasCuts;
    string samplesFile, variablesFile, cutsFile;
    ConfigContainer& cfgContainer;
    SampleContainer *sampleContainer;
    VariableContainer *variableContainer;
    CutContainer *cutContainer;
};
#endif