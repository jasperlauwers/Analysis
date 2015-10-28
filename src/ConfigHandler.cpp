
#include "ConfigHandler.hpp"


ConfigHandler::ConfigHandler(const string& fileName, ConfigContainer& cContainer) 
: ConfigReader(fileName), hasSamples(false), hasVariables(false), hasCuts(false), cfgContainer(cContainer)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    // Read the subconfig file names
    try
    {
        ////////// Config files ////////// 
        try
        {
            variablesFile = cfg.lookup("VariablesFile").c_str();
            hasVariables = true;
        }
        catch(const SettingNotFoundException &nfex) { }     
        
        try
        {
            samplesFile = cfg.lookup("SamplesFile").c_str();
            hasSamples = true;
        }
        catch(const SettingNotFoundException &nfex) { } 

        try
        {
            cutsFile = cfg.lookup("CutsFile").c_str();
            hasCuts = true;
        }
        catch(const SettingNotFoundException &nfex) { } 
        
        ////////// General settings ////////// 
        // TreeName
        try
        {
            cfgContainer.treeName = cfg.lookup("TreeName").c_str();
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "TreeName setting not found. Using default: 'latino'" << endl;
            cfgContainer.treeName = "latino";
        }
        
        // SamplesDir
        try
        {
            cfgContainer.sampleDir = cfg.lookup("SamplesDir").c_str();
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "SamplesDir setting not found. Using default: './'" << endl;
            cfgContainer.sampleDir = "./";
        }
        if( cfgContainer.sampleDir.size() != 0  && cfgContainer.sampleDir[cfgContainer.sampleDir.size()-1] != '/' )
        {
            cfgContainer.sampleDir.append("/");
        }
        
        // OutputDir
        try
        {
            cfgContainer.outputDir = cfg.lookup("OutputDir").c_str();
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "OutputDir setting not found. Using default: './'" << endl;
            cfgContainer.outputDir = "./";
        }
        if( cfgContainer.outputDir.size() != 0  && cfgContainer.outputDir[cfgContainer.outputDir.size()-1] != '/' )
        {
            cfgContainer.outputDir.append("/");
        }
        
        // Luminosity
        try
        {
            cfgContainer.luminosity = cfg.lookup("Luminosity");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "Luminosity setting not found. Using default: 1000" << endl;
            cfgContainer.luminosity = 1000.;
        }
        
        ////////// Plot Settings ////////// 
        // MaxEvents
        try
        {
            cfgContainer.maxEvents = cfg.lookup("MaxEvents");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "MaxEvents setting not found. Using default: -1" << endl;
            cfgContainer.maxEvents = -1.;
        }
    }
    catch(const SettingTypeException &tex)
    {
        cerr << "Incorrect type for setting: " << tex.getPath() << endl;
        throw;
    }
    
    // Set sub-Containers
    setSampleContainer( cfgContainer.sampleContainer );
    setVariableContainer( cfgContainer.variableContainer );
    setCutContainer( cfgContainer.cutContainer );
}

ConfigHandler::~ConfigHandler() { }

void ConfigHandler::setSampleContainer(SampleContainer& sContainer)
{
    if( hasSamples )
    {
        sampleContainer = &sContainer;
    }
    else
    {
        cerr << "Config file doesn't contain 'SamplesFile' setting." << endl;
        throw SettingNotFoundException("SamplesFile");
    }
}

void ConfigHandler::setVariableContainer(VariableContainer& vContainer)
{
    if( hasVariables )
    {
        variableContainer = &vContainer;
    }
    else
    {
        cerr << "Config file doesn't contain 'VariablesFile' setting." << endl;
        throw SettingNotFoundException("VariablesFile");
    }
}

void ConfigHandler::setCutContainer(CutContainer& cContainer)
{
    if( hasCuts )
    {
        cutContainer = &cContainer;
    }
    else
    {
        cout << "Config file doesn't contain 'CutsFile' setting. \n No cuts applied." << endl;
//         throw SettingNotFoundException("CutsFile");
    }
}

const Config& ConfigHandler::getConfig() const
{
    return cfg;   
}

void ConfigHandler::readConfig()
{
    try
    {
        if( sampleContainer )
        {
            SampleReader sReader(samplesFile, *sampleContainer);
            cout << "Read out samples file successfully" << endl;
        }
        
        if( variableContainer )
        {
            VariableReader vReader(variablesFile, *variableContainer);
            cout << "Read out variables file successfully" << endl;
        }
        
        if( cutContainer )
        {
            CutReader cReader(cutsFile, *cutContainer);
            cout << "Read out cuts file successfully" << endl;
        }
    }
    catch(const SettingNotFoundException &nfex)
    {
        cerr << "Setting not found in configuration file: " << nfex.getPath() << endl;
        throw;
    }
    catch(const SettingTypeException &tex)
    {
        cerr << "Incorrect type or structure in the configuration file: " << tex.getPath() << endl;
        throw;
    }
}