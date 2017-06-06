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
                    
                    int matched=0, ptMatched=0, etaMatched=0, unmatched=0, total=0, sameGenJet=0;
                    
                    while( reader.fillNextEvent() )
                    {
                        cleaner.doLeptonCleaning();
                        if( selecter.passCuts() )
                        {
                            vector<int> genIndex(10,-1);
                            int eventMatched=0;
                            bool notFound = false, doubleMatch = false;
                            float minEta=10, maxEta=-10;
                            int minEtaIndex=-1, maxEtaIndex=-1;

                            int counter=0;
                            for( auto iJet : eventContainer.goodJets ) {
//                                 if( ++counter > 20 )
//                                     break;
                                if( eventContainer.jets[iJet].pt() < 30 )
                                    break;
                                
                                if( eventContainer.jets[iJet].eta() < minEta )
                                {
                                    minEta = eventContainer.jets[iJet].eta();
                                    minEtaIndex = iJet;
                                }
                                if( eventContainer.jets[iJet].eta() > maxEta )
                                {
                                    maxEta = eventContainer.jets[iJet].eta();
                                    maxEtaIndex = iJet;
                                }

//                                 cout << "jet " << iJet << endl;
                                for( auto iGenJet : eventContainer.goodGenJets ) {
//                                     cout << "gen jet " << iGenJet << endl;
//                                     cout << eventContainer.jets[iJet].pt() << " \t" <<eventContainer.jets[iJet].eta() << "\t" << eventContainer.jets[iJet].phi() << endl;
//                                     cout << eventContainer.genJets[iGenJet].pt() << " \t" <<eventContainer.genJets[iGenJet].eta() << "\t" << eventContainer.genJets[iGenJet].phi() << endl;

                                    if( eventContainer.jets[iJet].dR(eventContainer.genJets[iGenJet]) < 0.6 )
                                    {
//                                         cout << "jet matched" << endl;
                                        if( genIndex[iJet] > -1 )
                                        {
                                            doubleMatch = true;
//                                             cout << "Double match found" << endl;
                                        }
                                        else
                                          eventMatched++;
                                            
                                        genIndex[iJet] = iGenJet;
                                    }
                                }
                            }
                            
                            if( genIndex[0] < 0 || genIndex[1] < 0 )
                                notFound = true;
                            
                            if( doubleMatch || (genIndex[0] == genIndex[1] && genIndex[0] > -1) )
                            {
//                                 cout << "double match or matched to same genjet " << genIndex[0] << " " <<  genIndex[1] << endl;
                                sameGenJet++;
//                                 continue;
                            }                               
                            
                            if( notFound )
                                unmatched++;
                            else
                                ptMatched++;
                            
                            if( eventMatched > 1 )
                                matched++;
                            
//                             cout << minEtaIndex << " " << maxEtaIndex << " " << genIndex[minEtaIndex] << " " << genIndex[maxEtaIndex]  << endl;
                            if( notFound && genIndex[minEtaIndex] > -1 && genIndex[maxEtaIndex] > -1 && (genIndex[minEtaIndex] != genIndex[maxEtaIndex]) )
                                etaMatched++;
                            
                            total++;
                        }
//                         cout << endl;

                    }
                    
                    cout << "pt matched: " << ptMatched-sameGenJet << " Unmatched: " << unmatched << "\nSame gen jet: " << sameGenJet << "\nefficiency: " << ((double)(ptMatched-sameGenJet))/total << endl;
                    cout << "Matched: " << matched << " Unmatched: " << unmatched << "\nSame gen jet: " << sameGenJet << "\nefficiency: " << ((double)matched)/total << endl;
                    cout << "eta matched: " << etaMatched << " Unmatched: " << unmatched << "\nSame gen jet: " << sameGenJet << "\nefficiency: " << ((double)etaMatched)/total << endl;
                }
            }
        }
    }
    
    delete cHandler;
}