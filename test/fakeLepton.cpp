#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "FakeLeptonPlotter.hpp"

int main (int argc, char ** argv) {
    
    // check number of input parameters
    if(argc < 2){
        cerr<<"Needs the cfg file as argument --> exit "<<endl;
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
    EventCleaner cleaner(eventContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    FakeLeptonPlotter plotter(eventContainer, cfgContainer);
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            {   
                while( reader.fillNextEvent() )
                {
//                     cleaner.doCleaning();
                    cleaner.doFakeLeptonCleaning();
                    if( selecter.passCuts() )
                        plotter.fill(iSample, iSubSample);
                }
            }
        }
    }
    
    string filename = "Histograms.root";
    plotter.writeHist(filename);
    plotter.writePlots("png");
    plotter.writeFakeRate("png");
    
    
    TFile *f = new TFile((cfgContainer.outputDir + filename).c_str(),"UPDATE");
    TH1F *hPtNum = (TH1F*) f->Get(("data_leptonpt1_" + cfgContainer.plotString).c_str());
    TH1F *hPtDen = (TH1F*) f->Get(("data_looseleptonpt1_" + cfgContainer.plotString).c_str());
    TH1F *hFRpt = (TH1F*) hPtNum->Clone("Fake_rate_pt");
    TH1F *hEtaNum = (TH1F*) f->Get(("data_leptoneta1_" + cfgContainer.plotString).c_str());
    TH1F *hEtaDen = (TH1F*) f->Get(("data_looseleptoneta1_" + cfgContainer.plotString).c_str());
    TH1F *hFReta = (TH1F*) hEtaNum->Clone("Fake_rate_eta");
    
    hFRpt->Divide(hPtDen);
    hFReta->Divide(hEtaDen);
    
    hFRpt->SetTitle("Fake_rate_pt");
    hFReta->SetTitle("Fake_rate_eta");
    
    hFRpt->Write();
    hFReta->Write();
    
    TCanvas *c = new TCanvas("c","", 600, 600);
    hFRpt->Draw();
    c->Print((cfgContainer.outputDir + hFRpt->GetTitle() + ".png").c_str(),"png");
    hFReta->Draw();
    c->Print((cfgContainer.outputDir + hFReta->GetTitle() + ".png").c_str(),"png");
    
    f->Close();
    
    delete cHandler;
}
