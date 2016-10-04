#ifndef ConfigHandler_hpp
#define ConfigHander_hpp

#include "libconfig.h++"
#include <iostream>
#include <string>
#include <fstream>
#include "VariableReader.hpp"
#include "SampleReader.hpp"
#include "CutReader.hpp"
#include "FakeReader.hpp"
#include "ConfigContainer.hpp"

using namespace std;
using namespace libconfig;

class ConfigHandler : public ConfigReader {
public:
    ConfigHandler(const string& fileName, ConfigContainer& cContainer);
    virtual ~ConfigHandler();
    void setSampleContainer(SampleContainer&);
    void setVariableContainer(VariableContainer&);
    void setCutContainer(CutContainer&);
    void setFakeContainer(FakeContainer&);
    const Config& getConfig() const;
    void readConfig();
    // Makes a copy of the config files to the output directory (log file)
    void writeConfig();
    
protected:
    bool hasSamples, hasVariables, hasCuts, hasFakes;
    string samplesFile, variablesFile, cutsFile, fakesFile;
    ConfigContainer& cfgContainer;
    SampleContainer *sampleContainer;
    VariableContainer *variableContainer;
    CutContainer *cutContainer;
    FakeContainer *fakeContainer;
    const string& fileName;
};
#endif