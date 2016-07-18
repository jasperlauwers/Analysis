#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "WeightCalc.hpp"
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
    WeightCalc weightCalc(eventContainer);

    
    int totTT = 0, totTL = 0, totLL =0;
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        // Init fake weights
        if( cfgContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON && cfgContainer.fakeContainer.fakeElectronFile != "" )
            weightCalc.initFakeWeight(&(cfgContainer.fakeContainer));
        
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            {
                // Init DY weights
                if( cfgContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos )
                    weightCalc.initDYWeight(reader);
                
                while( reader.fillNextEvent() )
                {
    //                 cleaner.doCleaning();
                    weightCalc.setWeight(cfgContainer.sampleContainer.sampleType[iSample], cfgContainer.sampleContainer.sampleNames[iSample][iSubSample]);
                                    
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
//                             weightCalc.setFakeWeight();
                            
                        int nTightLept = 0;
                        for( auto iLepton : eventContainer.goodLeptons ) {
                            if( eventContainer.leptons[iLepton].passesMedium() ) 
                                nTightLept++;
                        }
                        if( nTightLept  == 0 ) totLL++;
                        else if( nTightLept  == 1 ) totTL++;
                        else totTT++;
                        
//                         // Print soft mu
//                         if( nTightLept  > 0 )
//                         {
//                             cout << "Tight leptons: " << nTightLept << endl;
//                             for( int i = 0; i < eventContainer.goodJets.size(); ++i )
//                             {
//                                 if( eventContainer.jets[eventContainer.goodJets[i]].pt() > 0 )
//                                 {
//                                     cout << "jet pt: " << eventContainer.jets[eventContainer.goodJets[i]].pt() << endl;
//                                     cout << "soft muon pt: " << eventContainer.jets[eventContainer.goodJets[i]].softMuPt() << endl;
//                                     cout << "dR: " << eventContainer.jets[eventContainer.goodJets[i]].softMuDr() << endl;
//                                 }
//                             }
//                       
//                             for( int i = 0; i < eventContainer.goodLeptons.size(); ++i )
//                             {
//                                 if( eventContainer.leptons[eventContainer.goodLeptons[i]].pt() > 0 )
//                                 {
//                                     cout << "lepton pt: " << eventContainer.leptons[eventContainer.goodLeptons[i]].pt() << endl;
//                                 }
//                             }
//                             cout << endl;
//                         }
//                         if( eventContainer.looseleptoncorrectedpt(1) < 30 )
//                             cout << "Event nr:\t" << setprecision(10) << eventContainer.eventNo() << "\tfake weigth:\t" << eventContainer.weight() << endl;
//                             cout << "lepton 1 pt cor:\t" << eventContainer.looseleptoncorrectedpt(0) << "lepton 2 pt cor:\t" << eventContainer.looseleptoncorrectedpt(1) << endl;
                    }
                }
            }
        }
    }
    plotter.printEvents();
    plotter.writeEvents();
    plotter.writeHist("Cut_efficiency.root");
    plotter.writeEfficiency("png");
    plotter.writeStacked("png");
    cout /*<< "TT: " << totTT */<< "\tTL: " << totTL << "\tLL: " << totLL << endl;
    
    delete cHandler;
}
