#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "CutPlotter.hpp"

int main (int argc, char ** argv) {
    
    // check number of inpt parameters
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
    }
    catch(...)
    {
        cout << "exit program" << endl;
        if(cHandler) delete cHandler;
        return(-1);
    }
    
    EventContainer eventContainer;
    EventReader reader(eventContainer, cfgContainer);
//     EventCleaner cleaner( eventContainer );
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    CutPlotter plotter(eventContainer, cfgContainer);
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            { 
                while( reader.fillNextEvent() )
                {
    //                 cleaner.doCleaning();
                                    
                    plotter.fillTotal(iSample, iSubSample); // total # of events 
                    
                    for( unsigned int iCut = 0; iCut < cfgContainer.cutContainer.variableNames.size(); ++iCut ) 
                    {
                        if( selecter.passCut(iCut) ) {
                            plotter.fill(iSample, iSubSample, iCut);
                        }
                        else
                            break; // accumulate cuts
                    }               
                }
            }
        }
    }
    plotter.printEvents();
    plotter.writeHist("Cut_efficiency.root");
    plotter.writeEfficiency("png");
    plotter.writeStacked("png");
    
    delete cHandler;
}
