#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "EventPlotter.hpp"
// #include <thread>
#include "TThread.h"
#include "TMemFile.h"

void init(const ConfigContainer&); // Needed to read trees in a thread

typedef struct {
   unsigned int iSample;
   ConfigContainer* cfgContainer;
   EventPlotter* plotter;
} args_t;

void *fillPlots(void *ptr) //fillPlots( unsigned int iSample,  const ConfigContainer& cfgContainer, EventPlotter& plotter ) 
{
    args_t *args = (args_t *)ptr;
    
    EventContainer eventContainer;
    EventReader reader(eventContainer, *(args->cfgContainer) );
    EventSelecter selecter(eventContainer, args->cfgContainer->cutContainer);
//     EventCleaner cleaner(eventContainer);
    args->plotter->init(eventContainer, args->iSample);
    cout << "segmentation violation before?" << endl;
    
    for( unsigned int iSubSample = 0; iSubSample < args->cfgContainer->sampleContainer.sampleNames[args->iSample].size(); ++iSubSample) 
    {
        reader.setSample(args->iSample, iSubSample);
        cout << "segmentation violation before this one?" << endl;
        
        while( reader.fillNextEvent() )
        {
//             cleaner.doCleaning();
            if( selecter.passCuts() )
                args->plotter->parallelFill(args->iSample, iSubSample);
        }
    }
    return 0;        
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
    
    TThread *threads[cfgContainer.sampleContainer.reducedNames.size()];
    args_t args[cfgContainer.sampleContainer.reducedNames.size()];
    init(cfgContainer);
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        TString name = "tr"; name += iSample;
        
        args[iSample].iSample = iSample;
        args[iSample].cfgContainer = &cfgContainer;
        args[iSample].plotter = &plotter;
        threads[iSample] = new TThread(name, fillPlots, (void*) &(args[iSample]));
        threads[iSample]->Run();
        TThread::Ps();
        // TThread(name, fillPlots, iSample, ref(cfgContainer), ref(plotter));
    }
    
    cout << "Threads created" << endl;
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
//         if (threads[iSample].joinable())
            cout << "Before threads joined" << endl;
            threads[iSample]->Join();
            cout << "Threads joined" << endl;
            TThread::Ps();
    }    
    
    string filename = "test.root";
//     plotter.writeHist(filename);
//     string ext = "png";
//     plotter.writePlots("png");
    
    delete cHandler;
}

void init(const ConfigContainer& cfgContainer)
{
    
    EventContainer eventContainer;
    EventReader reader(eventContainer, cfgContainer );
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            reader.setSample(iSample, iSubSample);
            cout << "segmentation violation before this one?" << endl;
            
            reader.fillNextEvent();
        }
    }
}