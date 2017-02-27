#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "EventPlotter.hpp"
#include <sstream>
#include <fstream>
#include "TF2.h"

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
    }
    catch(...)
    {
        cout << "exit program" << endl;
        if(cHandler) delete cHandler;
        return(-1);
    }
    

    EventContainer eventContainer;
    
    // Set EventReader and extra branches
    EventReader reader(eventContainer, cfgContainer);
    vector<string> readBranches = {"std_vector_LHE_weight"};
    reader.addReadBranches(readBranches);
    
//     EventCleaner cleaner(eventContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    
    // EventPlotter for each weight
    int nWeights = 99;  
    vector<EventPlotter*> plotterVector;
    string oldPlotString = cfgContainer.plotString;
    plotterVector.push_back( new EventPlotter(eventContainer, cfgContainer) );
    
    for( unsigned int iW = 0; iW < nWeights; ++iW )
    {
        if( oldPlotString == "" )
            cfgContainer.plotString = to_string(iW);
        else
            cfgContainer.plotString = oldPlotString + "_" + to_string(iW);
        
        plotterVector.push_back( new EventPlotter(eventContainer, cfgContainer) );
    }
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            { 
                while( reader.fillNextEvent() )
                {
    //                 cleaner.doCleaning();
                    if( selecter.passCuts() )
                    {
                        reader.reweigh(451);
                        plotterVector[0]->fill(iSample, iSubSample);
                        for( unsigned int iW = 0; iW < nWeights; ++iW )
                        {
                            reader.reweigh(446+iW);
                            plotterVector[iW+1]->fill(iSample, iSubSample);
                        }
                    }
                }
            }
        }
    }

    // Read operator values
    vector<vector<Float_t>> opValVec(9); //opVal1, opVal2, opVal3, opVal4, opVal9, opVal10, opVal11, opVal12, opVal13;
    const int Noperators = 9;
    const int Nscan = 11;
//     Float_t opVal[Noperators] = {0,0,0,0,0,0,0,0,0};
    int opNum[Noperators] = {1,2,3,4,9,10,11,12,13};
    TString opName[Noperators] = {"S0","S1","M0","M1","M6","M7","T0","T1","T2"};
    
    ifstream infile("aQGC/reweight.txt");
    string line;
    int iOp = 0, iVal =-1;
    while (std::getline(infile, line))
    {
        if( line == "launch" )
        {
            iVal++;
            continue;
        }
        if( iVal == Nscan )
        {
            iOp++;
            iVal = 0;
        }
        
        istringstream iss(line);       
        string a;
        int opval;
        float val;
        iss >> a >> a >> opval >> val;
//         cout << opval << "\t" << val << endl;
        if( opval == opNum[iOp] )
        {
            opValVec[iOp].push_back(val);
        }
        else if( opval == 12 &&  val == 0. )
            continue;
        else
            cerr << "Line not recognized" << endl;
    }
    
//     for( int iOp = 0; iOp < opValVec.size(); ++iOp ) {
//       for( int iVal = 0; iVal < opValVec[iOp].size(); ++iVal ) {
//         cout << opName[iOp] << "\t" << opValVec[iOp][iVal] << endl;;
//       }
//     }
//     
    
    // Write to file (also adds overflow!)
    for( unsigned int iW = 0; iW < nWeights; ++iW )
    {
        if( oldPlotString == "" )
            cfgContainer.plotString = to_string(iW);
        else
            cfgContainer.plotString = oldPlotString + "_" + to_string(iW);
        
        plotterVector[iW]->writePlots("png");
        plotterVector[iW]->writeHist("histograms.png");
    }
    
    // Fit TF1 for EFT framework
    vector<double> x,y,z; // size depends on operator
    // Set SM hist
    TH1* hSM = plotterVector[0]->getHistogramContainer(0)->histograms[0];

    // Loop over operators
    for( int iOp = 0; iOp < Noperators; ++iOp ) {
        
        // Loop over bins ( every bin is fitted as a function of the coupling parameters )
        for( int iBin = 0; iBin < cfgContainer.variableContainer.nBins[0]; ++iBin ) //Nbin+1: include overflow
        { 
            // 1D scans
            if( iOp != Noperators ) {
                x.clear();
                y.clear();
            
                // Loop over anomalous coupling grid
                for(size_t iEFT = 0; iEFT < Nscan; iEFT++){
                    TH1* histoEFT = plotterVector[iOp*Nscan+iEFT+1]->getHistogramContainer(0)->histograms[0];
                    if( opValVec[iOp][iEFT] != 0 ) {
                        x.push_back( opValVec[iOp][iEFT]*1e12 ); // factor 1e12 for fit convergence
                        y.push_back( histoEFT->GetBinContent(iBin+1)/hSM->GetBinContent(iBin+1) );
                    }
                    else
                        cout << "This should be 1: " << histoEFT->GetBinContent(iBin+1)/hSM->GetBinContent(iBin+1) << endl;
                }
                x.push_back(0);
                y.push_back(1);
            
                TCanvas *c = new TCanvas(TString::Format("c_opt_%s_bin_%d",opName[iOp].Data(),iBin),"");
                TGraph *graph = new TGraph(x.size(), &x[0], &y[0]);
                graph->SetMarkerStyle(20);
                graph->GetYaxis()->SetTitle(Form("m_{ll}/m_{ll}^{SM} bin %d",iBin));
                graph->GetYaxis()->SetTitleOffset(1.3);
                graph->GetXaxis()->SetTitle(Form("%s operator (x 10^{-12})",opName[iOp].Data()));
                graph->Draw("AP");
                TF1* func = new TF1(TString::Format("bin_function_%d",iBin),"pol2",-1e3,1e3);
                func->SetLineWidth(2);
                graph->Fit(func,"QRME");
                c->Write();
                func->Write();
                system(("mkdir -p " + cfgContainer.outputDir).c_str());   
                c->SaveAs((cfgContainer.outputDir+"/operator_"+string(opName[iOp])+"_bin_"+to_string(iBin)+".png").c_str(),"png");
            }
            // 2D grid
//                 else {
//                     x.clear();
//                     y.clear();
//                     z.clear();
//                     // Loop over anomalous coupling grid
//                     for(size_t iEFT = 0; iEFT < histoCont.histogramEFT.size(); iEFT++){
//                         TH1F* histoEFT = histoCont.histogramEFT.at(iEFT);
//                         if( opValVec[0][iEFT] != 0 || opValVec[1][iEFT] != 0 ) {                    
//                             x.push_back( opValVec[0][iEFT]*1e12 ); // factor 1e12 for fit convergence
//                             y.push_back( opValVec[1][iEFT]*1e12 );
//                             z.push_back( histoEFT->GetBinContent(iBin+1)/hSM->GetBinContent(iBin+1) );
//                         }
//                     }
//                     x.push_back(0);
//                     y.push_back(0);
//                     z.push_back(1);
//                     
//                     TCanvas *c = new TCanvas(TString::Format("c_opt_%s_bin_%d",opName[iOp].Data(),iBin),"");
//                     TGraph2D *graph = new TGraph2D(x.size(), &x[0], &y[0], &z[0]);
//                     graph->SetMarkerStyle(20);
//                     graph->GetZaxis()->SetTitle(Form("m_{ll}/m_{ll}^{SM} bin %d",iBin));
//                     graph->GetZaxis()->SetTitleOffset(1.5);
//                     graph->GetXaxis()->SetTitle("S0 operator (x 10^{-12})");
//                     graph->GetXaxis()->SetTitleOffset(1.5);
//                     graph->GetYaxis()->SetTitle("S1 operator (x 10^{-12})");
//                     graph->GetYaxis()->SetTitleOffset(1.5);
//                     graph->Draw("p0");
//                     TF2* func = new TF2(TString::Format("bin_function_%d",iBin),"[0]+[1]*x+[2]*y+[3]*x*y+[4]*x*x+[5]*y*y", -70, 70, -150, 150);
//                     func->SetParameter(0,1);
//                     graph->Fit(func,"QRME");
//                     func->Draw("surf1same");
//                     c->Write();
//                     func->Write();
//                     c->SaveAs(("output/"+outputPlotDirectory+"/operator_"+string(opName[iOp])+"_bin_"+to_string(iBin)+".png").c_str(),"png");
//                 }
        }// End loop over bins
    }// End loop over operators
    
    delete cHandler;
    for( unsigned int iW = 0; iW < nWeights; ++iW )
    {
        delete plotterVector[iW];
    }
    
    
}
