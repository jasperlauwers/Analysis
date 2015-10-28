#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "EventPlotter.hpp"
// #include <thread>
#include "TThread.h"

void fillPlots( unsigned int iSample,  const ConfigContainer& cfgContainer, EventPlotter& plotter ) 
{
    EventContainer eventContainer;
    EventReader reader(eventContainer, cfgContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    EventCleaner cleaner(eventContainer);
    plotter.init(eventContainer, iSample);
    cout << "segmentation violation before?" << endl;
    
    for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
    {
        reader.setSample(iSample, iSubSample);
        cout << "segmentation violation before this one?" << endl;
        
        while( reader.fillNextEvent() && eventContainer.iEvent != cfgContainer.maxEvents )
        {
            cleaner.doCleaning();
            if( selecter.passCuts() )
                plotter.parallelFill(iSample, iSubSample);
        }
    }
            
}

int main(int argc, char ** argv) {
    
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

    EventPlotter plotter(cfgContainer);
    
    TThread threads[cfgContainer.sampleContainer.reducedNames.size()];
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        TString name = "tr"; name += iSample;
        threads[iSample] = TThread(name, fillPlots, iSample, ref(cfgContainer), ref(plotter));
    }
    
    cout << "Threads created" << endl;
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
//         if (threads[iSample].joinable())
            threads[iSample].Join();
    }    
    
    string filename = "test.root";
    plotter.writeHist(filename);
//     string ext = "png";
    plotter.writePlots("png");
    
    delete cHandler;
}
