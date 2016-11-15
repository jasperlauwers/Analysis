#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "WeightCalc.hpp"
#include "EventPlotter.hpp"

int main (int argc, char ** argv) {
    
    // check number of input parameters
    if(argc < 2){
        cerr<<"Needs the cfg file as agument --> exit "<<endl;
        return -1;
    }
        
    string cfgName(argv[1]);
    ConfigContainer cfgContainer;
    ConfigHandler* cHandler = nullptr;
    
    try
    {
        cHandler = new ConfigHandler(cfgName, cfgContainer);
        cHandler->readConfig();
        cHandler->writeConfig();
    }
    catch(...)
    {
        cout << "exit program" << endl;
        if(cHandler) delete cHandler;
        return(-1);
    }
    

    EventContainer eventContainer;
    EventReader reader(eventContainer, cfgContainer);
    EventCleaner cleaner(eventContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    WeightCalc weightCalc(eventContainer);
    EventPlotter plotter(eventContainer, cfgContainer);
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        // Init fake weights
        if( (cfgContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON || cfgContainer.sampleContainer.sampleType[iSample] == SampleType::MCFAKELEPTON) && cfgContainer.fakeContainer.fakeElectronFile != "" )
            weightCalc.initFakeWeight(&(cfgContainer.fakeContainer));
        
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            // Init DY weights
            if( cfgContainer.flipChargeDY && (cfgContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos || cfgContainer.sampleContainer.sampleNames[iSample][iSubSample].find("TTTo2L2Nu") != string::npos) )
                weightCalc.initDYWeight(reader);
            
            if( reader.setSample(iSample, iSubSample) )
            {   
                while( reader.fillNextEvent() )
                {
//                     cleaner.doCleaning();
                    cleaner.doTrackJetsCleaning();
                    if( selecter.passCuts() )
                    {
                        weightCalc.setWeight(cfgContainer.sampleContainer.sampleType[iSample], cfgContainer.sampleContainer.sampleNames[iSample][iSubSample]);
//                         cout << "Event nr:\t" << eventContainer.eventNo() << "\tfake weigth:\t" << eventContainer.weight() << endl;
//                         cout << "lepton 1 pt cor:\t" << eventContainer.looseleptoncorrectedpt(0) << "lepton 2 pt cor:\t" << eventContainer.looseleptoncorrectedpt(1) << endl;
                        plotter.fill(iSample, iSubSample);
                    }
                }
            }
        }
    }
    string filename = "test.root";
    plotter.writeHist(filename);
    plotter.writePlots("png");
    
    delete cHandler;
}
