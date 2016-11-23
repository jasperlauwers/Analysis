
#include "FakeReader.hpp"

FakeReader::FakeReader(const string& fileName, FakeContainer& fakeContainer)
: ConfigReader(fileName), fakeContainer(fakeContainer)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    const Setting& fakes = cfg.lookup("fakes");
    const unsigned int nFake = fakes.getLength();
    
    // Check # arguments
    if( nFake < 16 ) 
    {
        cerr << "Number of elements in fakes list not correct. Need at least 4 files to calculate Fake Weights." << endl;
        throw 1;
    }
    if((nFake%4) != 0 ) 
    {
        cerr << "Number of elements in fakes list not correct, as it is not divisible by 4" << endl;
        throw 1;
    }
    
    string fileTemp, histTemp;
    float maxPtTemp=0;
    for( unsigned int iFake=0; iFake < nFake; ++iFake ) 
    {
        int remainder = iFake%4;
        if( remainder == 0 ) 
        {
            fileTemp = (fakes[iFake]).c_str();
            if( fileTemp.find(".root") == string::npos )
            {
                fileTemp += ".root";
            }
        }
        else if( remainder == 1 ) 
        {
            histTemp = (fakes[iFake]).c_str();
        }
        else if( remainder == 2 ) 
        {
            maxPtTemp = fakes[iFake];
        }
        else if( remainder == 3 ) 
        {
            string type = (fakes[iFake]).c_str();
            transform(type.begin(), type.end(), type.begin(), ::tolower); // convert to lower case
            
            if( type == "efr" )
            {
                fakeContainer.fakeElectronFile = fileTemp;
                fakeContainer.fakeElectronHist = histTemp;
                fakeContainer.maxPtElectronFake = maxPtTemp;
            }
            else if( type == "efr2" )
            {
                fakeContainer.fakeElectronFile2 = fileTemp;
                fakeContainer.fakeElectronHist2 = histTemp;
                fakeContainer.maxPtElectronFake2 = maxPtTemp;
            }
            else if( type == "mfr" )
            {
                fakeContainer.fakeMuonFile = fileTemp;
                fakeContainer.fakeMuonHist = histTemp;
                fakeContainer.maxPtMuonFake = maxPtTemp;
            }
            else if( type == "mfr2" )
            {
                fakeContainer.fakeMuonFile2 = fileTemp;
                fakeContainer.fakeMuonHist2 = histTemp;
                fakeContainer.maxPtMuonFake2 = maxPtTemp;
            }
            else if( type == "epr" )
            {
                fakeContainer.promptElectronFile = fileTemp;
                fakeContainer.promptElectronHist = histTemp;
                fakeContainer.maxPtElectronPrompt = maxPtTemp;
            }
            else if( type == "mpr" )
            {
                fakeContainer.promptMuonFile = fileTemp;
                fakeContainer.promptMuonHist = histTemp;
                fakeContainer.maxPtMuonPrompt = maxPtTemp;
            }
            else
            {
                cerr << "Type unknown in fakes configuration file, options: \"eFR\"/\"eFR2\"/\"mFR\"/\"mFR2\"/\"ePR\"/\"mPR\"" << endl;
                throw 1;
            }
        }
    }
}

FakeReader::~FakeReader() { }