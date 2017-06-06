#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "TH2F.h"
#include "TFile.h"

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

    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {            
            if( reader.setSample(iSample, iSubSample) )
            {
                if( cfgContainer.sampleContainer.sampleNames[iSample][iSubSample].find("WpWpJJ") != string::npos )
                {
//                     TH1::SetDefaultSumw2();  
//                     Double_t ptBins[5] = {20,40,60,100,200};
//                     TH2F *hNum = new TH2F("charge_flip_probability", "Charge flip probability", 4, ptBins, 5, 0, 2.5);
//                     TH2F *hDen = new TH2F("charge_flip_den", "Charge flip den", 4, ptBins, 5, 0, 2.5);
//                     TH1F *hNum1D = new TH1F("charge_flip_probability_eta", "Charge flip probability eta", 5, 0, 2.5);
//                     TH1F *hDen1D = new TH1F("charge_flip_den_eta", "Charge flip den eta", 5, 0, 2.5);
                    
                    int matched=0, unmatched=0;
                    
                    while( reader.fillNextEvent() )
                    {
                        cleaner.doLeptonCleaning();
                        if( selecter.passCuts() )
                        {
                            vector<int> genIndex = {-1,-1};
                            bool notFound = false, doubleMatch = false;

                            int counter=0;
                            for( auto iJet : eventContainer.goodJets ) {
                                if( ++counter > 2 )
                                    break;

                                for( auto iGenJet : eventContainer.goodGenJets ) {

                                    if( eventContainer.jets[iJet].dR(eventContainer.genJets[iGenJet]) < 0.3 )
                                    {
                                        if( genIndex[iJet] > -1 )
                                        {
                                            doubleMatch = true;
                                            cout << "Double match found" << endl;
                                        }
                                            
                                        genIndex[iJet] = iGenJet;
                                    }
                                }
                                if( genIndex[iJet] < 0 )
                                    notFound = true;
                            }
                            
                            if( doubleMatch || genIndex[0] == genIndex[1] )
                            {
                                cout << "double match or matched to same genjet" << endl;
                                continue;
                            }                               
                            
                            if( notFound )
                                unmatched++;
                            else
                                matched++;
                        }

                    }
                    
                    cout << "Matched: " << matched << " Unmatched: " << unmatched << "\nefficiency: " << ((double)matched)/(unmatched+matched) << endl;
                }
            }
        }
    }
    
    delete cHandler;
}