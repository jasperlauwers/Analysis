#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "WeightCalc.hpp"
#include "EventPlotter.hpp"

/*
 * Plots using multiple config files
 * The first config file is the main one, 
 * from the next ones only the samples list
 * and cuts list will be used
 */

int main (int argc, char ** argv) {
    
    // check number of input parameters
    if(argc < 2){
        cerr<<"Needs at least one cfg file as agument --> exit "<<endl;
        return -1;
    }
        
    vector<string> cfgNames(argc-1);
    vector<ConfigContainer> cfgContainers(argc-1);
    vector<unique_ptr<ConfigHandler>> cHandlers;
    
    ConfigContainer mainCfgContainer;
    for( int iCfg = 0; iCfg < (argc-1); ++iCfg ) 
    {
        cfgNames[iCfg] = string(argv[iCfg+1]);
        
        try
        {
            unique_ptr<ConfigHandler> ptr(new ConfigHandler(cfgNames[iCfg], cfgContainers[iCfg]));
            cHandlers.push_back(move(ptr));
            cHandlers[iCfg]->readConfig();
            cHandlers[iCfg]->writeConfig();
        }
        catch(...)
        {
            return(-1);
        }
        
        // Add samples to main ConfigContainer
        if( iCfg == 0 )
            mainCfgContainer = cfgContainers[0];
        else
        {
            mainCfgContainer.sampleContainer.add(cfgContainers[iCfg].sampleContainer);   
            mainCfgContainer.cutContainer.add(cfgContainers[iCfg].cutContainer); 
        }
    }
    
    EventContainer eventContainer;
    EventPlotter plotter(eventContainer, mainCfgContainer);
    EventReader reader(eventContainer, mainCfgContainer);
    EventCleaner cleaner(eventContainer);
    WeightCalc weightCalc(eventContainer);
    
    unsigned int totSample = 0;
    for( int iCfg = 0; iCfg < (argc-1); ++iCfg ) 
    {
        EventSelecter selecter(eventContainer, cfgContainers[iCfg].cutContainer);
        
        for( unsigned int iSample = 0; iSample < cfgContainers[iCfg].sampleContainer.reducedNames.size(); ++iSample) 
        {
            // Init fake weights
            if( (cfgContainers[iCfg].sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON || cfgContainers[iCfg].sampleContainer.sampleType[iSample] == SampleType::MCFAKELEPTON) && mainCfgContainer.fakeContainer.fakeElectronFile != "" )
                weightCalc.initFakeWeight(&(mainCfgContainer.fakeContainer));
            
            for( unsigned int iSubSample = 0; iSubSample < cfgContainers[iCfg].sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
            {
                // Init DY weights
                if( cfgContainers[iCfg].flipChargeDY && (cfgContainers[iCfg].sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos || cfgContainers[iCfg].sampleContainer.sampleNames[iSample][iSubSample].find("TTTo2L2Nu") != string::npos) )
                    weightCalc.initDYWeight(reader);
                
                if( reader.setSample(totSample, iSubSample) )
                {   
                    while( reader.fillNextEvent() )
                    {
    //                     cleaner.doCleaning();
//                         cleaner.doTrackJetsCleaning();
                        cleaner.doLeptonCleaning();
                        if( selecter.passCuts() )
                        {
                            weightCalc.setWeight(cfgContainers[iCfg].sampleContainer.sampleType[iSample], cfgContainers[iCfg].sampleContainer.sampleNames[iSample][iSubSample]);
    //                         cout << "Event nr:\t" << eventContainer.eventNo() << "\tfake weigth:\t" << eventContainer.weight() << endl;
    //                         cout << "lepton 1 pt cor:\t" << eventContainer.looseleptoncorrectedpt(0) << "lepton 2 pt cor:\t" << eventContainer.looseleptoncorrectedpt(1) << endl;
                            plotter.fill(totSample, iSubSample);
                        }
                    }
                }
            }
            totSample++;
        }
    }
    string filename = "test.root";
    plotter.writeHist(filename);
    plotter.writePlots("png");
}
