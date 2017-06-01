#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "TH2F.h"

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
//     EventCleaner cleaner(eventContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);

    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {            
            if( reader.setSample(iSample, iSubSample) )
            {
                if( cfgContainer.sampleContainer.sampleNames[iSample][iSubSample].find("DY") != string::npos )
                {
                    TH1::SetDefaultSumw2();
                    TFile *f = new TFile("
                    
                    TH2F *hNum = new TH2F("charge_flip_probability", "Charge flip probability", 15, 0 , 150, 5, 0, 2.5);
                    TH2F *hDen = new TH2F("charge_flip_den", "Charge flip den", 15, 0 , 150, 5, 0, 2.5);
                    TH1F *hNum1D = new TH1F("charge_flip_probability_eta", "Charge flip probability eta", 5, 0, 2.5);
                    TH1F *hDen1D = new TH1F("charge_flip_den_eta", "Charge flip den eta", 5, 0, 2.5);
                    
                    while( reader.fillNextEvent() )
                    {
                        if( selecter.passCuts() )
                        {
                            vector<size_t> genIndex = {0,0};
                            bool notFound = false, doubleMatch = false;
                            
                            for( auto iLep : eventContainer.goodLeptons ) {
                                for( auto iLHELep : eventContainer.goodLHELeptons ) {
                                    if( eventContainer.leptons[iLep].dR(eventContainer.lheLeptons[iLHELep]) ) < 0.1
                                    {
                                        if( genIndex[iLep] > 0 )
                                            doubleMatch = true;
                                            
                                        genIndex[iLep] = iLHELep;
                                    }
                                    notFound = true;
                                }
                            }
                            
                            if( notFound || doubleMatch || genIndex[0] == genIndex[1] )
                                continue;
                            
                            for( auto iLep : eventContainer.goodLeptons ) {
                                if( eventContainer.leptons[iLep].charge == eventContainer.lheLeptons[genIndex[iLep]].charge )
                                {
                                    hDenom->Fill(eventContainer.leptons[iLep].pt(), abs(eventContainer.leptons[iLep].eta()));
                                    hDen1D->Fill(abs(eventContainer.leptons[iLep].eta()));
                                }
                                else
                                {
                                    hNum->Fill(eventContainer.leptons[iLep].pt(), abs(eventContainer.leptons[iLep].eta()));
                                    hNum1D->Fill(abs(eventContainer.leptons[iLep].eta()));
                                }
                            }
                        }
                    }
                    hNum->Divide(hDenom);
                    hNum1D->Divide(hDen1D);
                    
                    system(("mkdir -p " + cfgContainer.outputDir).c_str());   
                    TFile* f = new TFile((cfgContainer.outputDir + "chFlip.root").c_str(),"RECREATE");
                    hNum->Write();
                    hNum1D->Write()
                    f->Close();
                }
            }
        }
    }
    
    delete cHandler;
}
