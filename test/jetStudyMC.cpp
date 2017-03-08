#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "BasePlotter.hpp"

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
    
    // Create framework objects
    EventContainer eventContainer;
    // FIXME Make reader without variables config and set correct branches
    EventReader reader(eventContainer, cfgContainer);
    EventCleaner cleaner(eventContainer);
    
    // Set parameters for jet study
    const int nPtBins = 3;
    const int nEtaBins = 4;
    int ptBins[nPtBins] = {30,100,1000};
    float etaBins[nEtaBins] = {0,2.5,3,10};
    Float_t minPt = 30;
        
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        BasePlotter plotter(eventContainer, cfgContainer);
            
        vector<HistogramContainer> histogramContainers;
        histogramContainer("jet_study_mc", (nEtaBins-1)*4+2);
        vector<vector<TH1F*>> denominatorHists;
        TString inReplace = ".", outReplace = "p";
        
        // Create histograms
        vector<TString> typeVector = {Efficiency, Purity};
        for( auto iString : typeVector );
        {
            for( int i=0; i < (nEtaBins-1); ++i ) {
            
                // vs Pt
                TString strEff = TString::Format(iString + "_vs_pt_eta_%.1f_%.1f", etaBins[iEta], etaBins[(iEta+1)]);
                strEff.ReplaceAll(inReplace, outReplace);
                histogramContainers.push_back( histogramContainer(strEff, 2) );    
                
                histogramContainer[histogramContainer.size()-1].add( new TH1F("CHS_num_"+strEff, "", 40, 0 ,400), "CHS", 3, false, true );      
                histogramContainer[histogramContainer.size()-1].add( new TH1F("Puppi_num_"+strEff, "", 40, 0 ,400), "PUPPI", 4, false, true );
                vector<TH1F*> denomH;
                denomH.push_back(new TH1F("CHS_den_"+strEff, "", 40, 0 ,400));
                denomH.push_back(new TH1F("Puppi_den_"+strEff, "", 40, 0 ,400));
                denominatorHists.push_back(denomH);
                
                // vs PU
                strEff = TString::Format(iString + "_vs_pu_eta_%.1f_%.1f", etaBins[iEta], etaBins[(iEta+1)]);
                strEff.ReplaceAll(inReplace, outReplace);
                histogramContainers.push_back( histogramContainer(strEff, 2) ); 
                
                histogramContainer[histogramContainer.size()-1].add( new TH1F("CHS_num_"+strEff, "", 50, 10, 60), "CHS", 3, false, true );
                histogramContainer[histogramContainer.size()-1].add( new TH1F("Puppi_num_"+strEff, "", 50, 10, 60), "PUPPI", 4, false, true );
                denomH.clear();
                denomH.push_back(new TH1F("CHS_den_"+strEff, "", 50, 10, 60));
                denomH.push_back(new TH1F("Puppi_den_"+strEff, "", 50, 10, 60));
                denominatorHists.push_back(denomH);
            }
            // vs Eta
            strEff = iString + "_vs_eta";
            histogramContainers.push_back( histogramContainer(strEff, 2) );
            histogramContainer[histogramContainer.size()-1].add( new TH1F("CHS_num_"+strEff, "", 60, -6 ,6), "CHS", 3, false, true );
            histogramContainer[histogramContainer.size()-1].add( new TH1F("Puppi_num_"+strEff, "", 60, -6 ,6), "PUPPI", 4, false, true );
            vector<TH1F*> denomH;
            denomH.push_back(new TH1F("CHS_den_"+strEff, "", 60, -6 ,6));
            denomH.push_back(new TH1F("Puppi_den_"+strEff, "", 60, -6 ,6));
            denominatorHists.push_back(denomH);
        }
    
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            reader.setSample(iSample, iSubSample);
            
            while( reader.fillNextEvent() )
            {
                cleaner.doCleaning();
                
                // Match GEN with RECO jets
                for( unsigned int iGen : eventContainer.goodGenJets ) {
                    
                    // Save all genJets with pt > minPt
                    if( eventContainer.genJets[iGen].pt() < minPt ) break;
                    for( int iType = 0; iType < 2; ++iType ) 
                    {
                        for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                            if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) denominatorHists[iEta*2][iType]->Fill(eventContainer.genJets[iGen].pt());
                            if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) denominatorHists[iEta*2+1][iType]->Fill(eventContainer.nvertices());
                        }
                        denominatorHists[(nEtaBins-1)*2][iType]->Fill(eventContainer.genJets[iGen].eta());
                    }
                    
                    // CHS
                    for( unsigned int iReco : eventContainer.goodJets ) {
                        Double_t dRchs = eventContainer.jets[iReco].dR(eventContainer.genJets[iGen]);
                        if( eventContainer.jets[iReco].pt() > minRecoEffPt && dRchs < 0.3 && eventContainer.jets[iReco].puId > puidSel) { // check jet puid
                            for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2].histograms[0]->Fill(eventContainer.genJets[iGen].pt());
                                if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2+1].histograms[0]->Fill(eventContainer.nvertices());
                            }
                            histogramContainers[(nEtaBins-1)*2].histograms[0]->Fill(eventContainer.genJets[iGen].eta());
                            break;
                        }
                    }
                    for( unsigned int iReco : eventContainer.goodPuppiJets ) {
                        Double_t dRpuppi = eventContainer.puppiJets[iReco].dR(eventContainer.genJets[iGen]);
                        if( eventContainer.puppiJets[iReco].pt() > minRecoEffPt && dRpuppi < 0.3 ) {
                            for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2].histograms[1]->Fill(eventContainer.genJets[iGen].pt());
                                if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2+1].histograms[1]->Fill(eventContainer.nvertices());
                            }
                            histogramContainers[(nEtaBins-1)*2].histograms[1]->Fill(eventContainer.genJets[iGen].eta());
                            break;
                        }                    
                    }            
                }// End gen loop
                
                // Match RECO with GEN jets
                // CHS
                for( unsigned int iReco : eventContainer.goodJets ) {
                    Double_t dRchs = eventContainer.jets[iReco].dR(eventContainer.genJets[iGen]);
                    if( eventContainer.jets[iReco].pt() > minRecoEffPt && dRchs < 0.3 && eventContainer.jets[iReco].puId > puidSel) { // check jet puid
                        for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                            if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2].histograms[0]->Fill(eventContainer.genJets[iGen].pt());
                            if( eventContainer.genJets[iGen].eta() > etaBins[iEta] && eventContainer.genJets[iGen].eta() < etaBins[iEta+1] ) histogramContainers[iEta*2+1].histograms[0]->Fill(eventContainer.nvertices());
                        }
                        histogramContainers[(nEtaBins-1)*2].histograms[0]->Fill(eventContainer.genJets[iGen].eta());
                        break;
                    }
                }
                
                
                for( int iRECO = 0; iRECO < matchN; ++iRECO ) {
                    bool firstChs = true, firstPuppi = true;
                    
                    // Save all chs jets with pt > minPt && passing jet puid
                    if( (*jet_pt)[iRECO] < minPt || (*jet_puid)[iRECO] < puidSel) firstChs = false;
                    else {
                        for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                            if( (*jet_eta)[iRECO] > etaBins[iEta] && (*jet_eta)[iRECO] < etaBins[iEta+1] ) jetReco[iEta]->Fill((*jet_pt)[iRECO]);
                            if( (*jet_eta)[iRECO] > etaBins[iEta] && (*jet_eta)[iRECO] < etaBins[iEta+1] ) jetReco[iEta+2*(nEtaBins-1)]->Fill(*nvtx);
                        }
                        jetReco[(nEtaBins-1)*4]->Fill((*jet_eta)[iRECO]);
                    }
                    
                    // Save all puppi jets with pt > minPt
                    if( (*puppiJet_pt)[iRECO] < minPt ) firstPuppi = false;
                    else {
                        for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                            if( (*puppiJet_eta)[iRECO] > etaBins[iEta] && (*puppiJet_eta)[iRECO] < etaBins[iEta+1] ) jetReco[iEta+(nEtaBins-1)]->Fill((*puppiJet_pt)[iRECO]);
                            if( (*puppiJet_eta)[iRECO] > etaBins[iEta] && (*puppiJet_eta)[iRECO] < etaBins[iEta+1] ) jetReco[iEta+3*(nEtaBins-1)]->Fill(*nvtx);
                        }
                        jetReco[(nEtaBins-1)*4+1]->Fill((*puppiJet_eta)[iRECO]);
                    }
                    
                    for(unsigned int iGen = 0; iGen < cleaned_genJet_pt.size(); ++iGen ) {
                        Double_t dRchs = TMath::Sqrt( TMath::Power((*jet_eta)[iRECO] - cleaned_genJet_eta[iGen], 2) + TMath::Power(abs(abs((*jet_phi)[iRECO] - cleaned_genJet_phi[iGen])-pi)-pi, 2) );
                        Double_t dRpuppi = TMath::Sqrt( TMath::Power((*puppiJet_eta)[iRECO] - cleaned_genJet_eta[iGen], 2) + TMath::Power(abs(abs((*puppiJet_phi)[iRECO] - cleaned_genJet_phi[iGen])-pi)-pi, 2) );
                        if( dRchs < 0.3 && firstChs ) {                       
                            for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                if( (*jet_eta)[iRECO] > etaBins[iEta] && (*jet_eta)[iRECO] < etaBins[iEta+1] ) genMatch[iEta]->Fill((*jet_pt)[iRECO]);
                                if( (*jet_eta)[iRECO] > etaBins[iEta] && (*jet_eta)[iRECO] < etaBins[iEta+1] ) genMatch[iEta+2*(nEtaBins-1)]->Fill(*nvtx);
                            }
                            genMatch[(nEtaBins-1)*4]->Fill((*jet_eta)[iRECO]);
                            
                            Float_t res = (*jet_pt)[iRECO] / cleaned_genJet_pt[iGen] -1;
                            for( int iPt=0; iPt<(nPtBins-1); ++iPt ) {
                                for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                    if( (*jet_pt)[iRECO] > ptBins[iPt] && (*jet_pt)[iRECO] < ptBins[iPt+1] && (*jet_eta)[iRECO] > etaBins[iEta] && (*jet_eta)[iRECO] < etaBins[iEta+1] )
                                        jetRes[iPt+iEta*(nPtBins-1)]->Fill(res);
                                }
                            }
                            firstChs = false;
                        }
                        if( dRpuppi < 0.3 && firstPuppi ) {
                            for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                if( (*puppiJet_eta)[iRECO] > etaBins[iEta] && (*puppiJet_eta)[iRECO] < etaBins[iEta+1] ) genMatch[iEta+(nEtaBins-1)]->Fill((*puppiJet_pt)[iRECO]);
                                if( (*puppiJet_eta)[iRECO] > etaBins[iEta] && (*puppiJet_eta)[iRECO] < etaBins[iEta+1] ) genMatch[iEta+3*(nEtaBins-1)]->Fill(*nvtx);
                            }
                            genMatch[(nEtaBins-1)*4+1]->Fill((*puppiJet_eta)[iRECO]);
                                                    
                            Float_t res = (*puppiJet_pt)[iRECO] / cleaned_genJet_pt[iGen] -1;
                            for( int iPt=0; iPt<(nPtBins-1); ++iPt ) {
                                for( int iEta=0; iEta<(nEtaBins-1); ++iEta ) {
                                    if( (*puppiJet_pt)[iRECO] > ptBins[iPt] && (*puppiJet_pt)[iRECO] < ptBins[iPt+1] && (*puppiJet_eta)[iRECO] > etaBins[iEta] && (*puppiJet_eta)[iRECO] < etaBins[iEta+1] )
                                        jetRes[iPt+iEta*(nPtBins-1)+nResHist/2]->Fill(res);
                                }
                            }                        
                            firstPuppi = false;
                        }                    
                    }   
                }
                
            }// End event loop
        }// End subsample loop
        
        string filename = "test.root";
        plotter.writeHist(filename);
        plotter.writePlots("png");    
        
    }// End sample loop

    delete cHandler;
}
