
#include "ConfigHandler.hpp"


ConfigHandler::ConfigHandler(const string& fileName, ConfigContainer& cContainer) 
: ConfigReader(fileName), hasSamples(false), hasVariables(false), hasCuts(false), hasFakes(false), cfgContainer(cContainer), sampleContainer(nullptr), variableContainer(nullptr), cutContainer(nullptr), fileName(fileName)
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
        
        try
        {
            fakesFile = cfg.lookup("FakesFile").c_str();
            hasFakes = true;
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
        
        // MinJetPt
        try
        {
            cfgContainer.minJetPt = cfg.lookup("MinJetPt");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "MinJetPt setting not found. Using default: 0" << endl;
            cfgContainer.minJetPt = 0.;
        }
        
        // MinLeptonPt
        try
        {
            cfgContainer.minLeptonPt = cfg.lookup("MinLeptonPt");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "MinLeptonPt setting not found. Using default: 0" << endl;
            cfgContainer.minLeptonPt = 0.;
        }
        
        // FlipChargeDY
        try
        {
            cfgContainer.flipChargeDY = cfg.lookup("FlipChargeDY");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "FlipChargeDY setting not found. Using default: false" << endl;
            cfgContainer.flipChargeDY = false;
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
        
        // PlotRatio
        try
        {
            cfgContainer.plotRatio = cfg.lookup("PlotRatio");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "PlotRatio setting not found. Using default: false" << endl;
            cfgContainer.plotRatio = false;
        }
        
        // PlotSignificance
        try
        {
            cfgContainer.plotSignificance = cfg.lookup("PlotSignificance");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "PlotSignificance setting not found. Using default: false" << endl;
            cfgContainer.plotSignificance = false;
        }
        
        // LogY
        try
        {
            cfgContainer.logY = cfg.lookup("LogY");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "LogY setting not found. Using default: false" << endl;
            cfgContainer.logY = false;
        }
        
        // SignalStacked
        try
        {
            cfgContainer.signalStacked = cfg.lookup("SignalStacked");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "SignalStacked setting not found. Using default: true" << endl;
            cfgContainer.signalStacked = true;
        }
        
        // AddOverflow
        try
        {
            cfgContainer.addOverflow = cfg.lookup("AddOverflow");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "AddOverflow setting not found. Using default: false" << endl;
            cfgContainer.addOverflow = false;
        }
        
        // AddUnderflow
        try
        {
            cfgContainer.addUnderflow = cfg.lookup("AddUnderflow");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "AddUnderflow setting not found. Using default: false" << endl;
            cfgContainer.addUnderflow = false;
        }
        
        // DrawUncertainty
        try
        {
            cfgContainer.drawUncertainty = cfg.lookup("DrawUncertainty");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "DrawUncertainty setting not found. Using default: true" << endl;
            cfgContainer.drawUncertainty = true;
        }
        
        // DrawNormalized
        try
        {
            cfgContainer.drawNormalized = cfg.lookup("DrawNormalized");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "DrawNormalized setting not found. Using default: false" << endl;
            cfgContainer.drawNormalized = false;
        }
        
        // PlotString
        try
        {
            cfgContainer.plotString = cfg.lookup("PlotString").c_str();
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "PlotString setting not found. Using default: \"\"" << endl;
            cfgContainer.plotString = "";
        }
        
        // Unblind
        try
        {
            cfgContainer.unblind = cfg.lookup("Unblind");
        }
        catch(const SettingNotFoundException &nfex) 
        {
            cout << "Unblind setting not found. Using default: false" << endl;
            cfgContainer.unblind = false;
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
    setFakeContainer( cfgContainer.fakeContainer );
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

void ConfigHandler::setFakeContainer(FakeContainer& fContainer)
{
    if( hasFakes )
    {
        fakeContainer = &fContainer;
    }
    else
    {
        cout << "Config file doesn't contain 'FakesFile' setting. \n No fake weights calculated." << endl;
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
        if( hasSamples )
        {
            cout << "Reading out samples file" << endl;
            SampleReader sReader(samplesFile, *sampleContainer);
            cout << "Read out samples file successfully" << endl;
        }
        
        if( hasVariables )
        {
            cout << "Reading out variables file" << endl;
            VariableReader vReader(variablesFile, *variableContainer);
            cout << "Read out variables file successfully" << endl;
        }
        if( hasCuts )
        {
            cout << "Reading out cuts file" << endl;
            CutReader cReader(cutsFile, *cutContainer);
            cout << "Read out cuts file successfully" << endl;
        }
        if( hasFakes )
        {
            cout << "Reading out fakes file" << endl;
            FakeReader fReader(fakesFile, *fakeContainer);
            cout << "Read out fakes file successfully" << endl;
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

void ConfigHandler::writeConfig()
{
    ifstream ifStream(fileName, ios::binary);
    
    system(("mkdir -p " + cfgContainer.outputDir).c_str());   
        
    string outConfigName = fileName;
    if( fileName.find('/') != std::string::npos )
    {
        outConfigName = fileName.substr(fileName.find_last_of("/")+1);
    }
    ofstream ofStream(cfgContainer.outputDir+"Config_"+outConfigName, ios::binary);

    ofStream << ifStream.rdbuf();
    
    if( hasSamples )
    {
        ifstream ifStream(samplesFile, ios::binary);
        ofStream << ifStream.rdbuf();
    }    
    if( hasVariables )
    {
        ifstream ifStream(variablesFile, ios::binary);
        ofStream << ifStream.rdbuf();
    }
    if( hasCuts )
    {
        ifstream ifStream(cutsFile, ios::binary);
        ofStream << ifStream.rdbuf();
    }
    if( hasFakes )
    {
        ifstream ifStream(fakesFile, ios::binary);
        ofStream << ifStream.rdbuf();
    }
}