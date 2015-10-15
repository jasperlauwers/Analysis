#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "EventPlotter.hpp"

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
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    EventPlotter plotter(eventContainer, cfgContainer);
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            reader.setSample(iSample, iSubSample);
            EventCleaner cleaner( eventContainer ); // has to be done after setSample! FIXME
            
            while( reader.fillNextEvent() /*&& eventContainer.iEvent < 1e6 */)
            {
                cleaner.doCleaning();
                if( selecter.passCuts() )
                    plotter.fill(iSample, iSubSample);
//                 for( int iCut = 0; iCut < cfgContainer.cutContainer.variableNames.size(); ++iCut ) 
//                 {
//                     if( selecter.passCut(iCut) ) 
//                         cout << "Passing cut " << iCut << endl;
//                     else
//                         cout << "Not passing cut " << iCut << endl;                    
//                 }
            }
        }
    }
    string filename = "test.root";
    plotter.writeHist(filename);
//     string ext = "png";
    plotter.writePlots("png");
    
    delete cHandler;
}
