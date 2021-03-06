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
    EventCleaner cleaner( eventContainer );
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    
    TH1F *hCuts = new TH1F("Cut_Efficiency","",cfgContainer.cutContainer.variableNames.size()+1,0,cfgContainer.cutContainer.variableNames.size()+1);
    for( int iCut = 0; iCut < cfgContainer.cutContainer.variableNames.size(); ++iCut ) 
    {
        hCuts->GetXaxis()->SetBinLabel(iCut+2, (cfgContainer.cutContainer.variableNames[iCut]).c_str());
    }
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            reader.setSample(iSample, iSubSample);
            
            while( reader.fillNextEvent() )
            {
                cleaner.doCleaning();
                for( int iCut = 0; iCut < cfgContainer.cutContainer.variableNames.size(); ++iCut ) 
                {
                    if( selecter.passCut(iCut) ) 
                        hCuts->Fill(iCut+1.5);
                }
            }
        }
    }
    system(("mkdir -p " + cfgContainer.outputDir).c_str());   
    TFile* f = new TFile((cfgContainer.outputDir + "cutEff.root").c_str(),"RECREATE");
    hCuts->Write();
    f->Close();
    
    delete cHandler;
}
