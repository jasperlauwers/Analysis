#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "FakeCalc.hpp"
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
    FakeCalc fakeCalc(eventContainer, cfgContainer.fakeContainer);
    CutPlotter plotter(eventContainer, cfgContainer);
    
    int totTT = 0, totTL = 0, totLL =0;
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            { 
                while( reader.fillNextEvent() )
                {
    //                 cleaner.doCleaning();
                    if( cfgContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON && (&cfgContainer.fakeContainer) )
                            fakeCalc.setFakeWeight();
                                    
                    plotter.fillTotal(iSample, iSubSample); // total # of events 
                    
                    for( unsigned int iCut = 0; iCut < cfgContainer.cutContainer.variableNames.size(); ++iCut ) 
                    {
                        if( selecter.passCut(iCut) ) {
                            plotter.fill(iSample, iSubSample, iCut);
                        }
                        else
                            break; // accumulate cuts
                            
                    }
                    
                    // Temporary fake counter
                    if( cfgContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON && selecter.passCuts() )
                    {
//                         if( &cfgContainer.fakeContainer)
//                             fakeCalc.setFakeWeight();
                            
                        int nTightLept = 0;
                        for( auto iLepton : eventContainer.goodLeptons ) {
                            if( eventContainer.leptons[iLepton].passesMedium() ) 
                                nTightLept++;
                        }
                        cout << "Tight leptons: " << nTightLept << endl;
                        if( nTightLept  == 0 ) totLL++;
                        else if( nTightLept  == 1 ) totTL++;
                        else totTT++;
//                         if( eventContainer.looseleptoncorrectedpt(1) < 30 )
//                             cout << "Event nr:\t" << eventContainer.eventNo() << "\tfake weigth:\t" << eventContainer.weight() << endl;
//                             cout << "lepton 1 pt cor:\t" << eventContainer.looseleptoncorrectedpt(0) << "lepton 2 pt cor:\t" << eventContainer.looseleptoncorrectedpt(1) << endl;
                    }
                }
            }
        }
    }
    plotter.printEvents();
    plotter.writeHist("Cut_efficiency.root");
    plotter.writeEfficiency("png");
    plotter.writeStacked("png");
    cout /*<< "TT: " << totTT */<< "\tTL: " << totTL << "\tLL: " << totLL << endl;
    
    delete cHandler;
}
