 
#include "FakeLeptonPlotter.hpp"

FakeLeptonPlotter::FakeLeptonPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: EventPlotter(evContainer, cfgContainer)
{ }

FakeLeptonPlotter::~FakeLeptonPlotter(){ }

void FakeLeptonPlotter::fill(unsigned int iSample, unsigned int iSubSample) 
{
    if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() )
    {
//         if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA )
//         {
//             for( unsigned int iVar = 0, iHist = 0; iVar < nVariables; ++iVar, ++iHist )
//             {
//                 if( configContainer.variableContainer.is2D[iVar] )
//                 {
//                     TH2D* h = (TH2D*) histogramContainers[iHist].histograms[iSample];
//                     h->Fill( functionVector[iVar](), functionVector[iVar+1](), globalWeight[iSample][iSubSample] );  // apply global weight for trigger luminosity
//                     iVar++;
//                 }
//                 else
//                     histogramContainers[iHist].histograms[iSample]->Fill( functionVector[iVar](), globalWeight[iSample][iSubSample] );
//             }
//         }
//         else
//         {
            for( unsigned int iVar = 0, iHist = 0; iVar < nVariables; ++iVar, ++iHist )
            {
                if( configContainer.variableContainer.is2D[iVar] )
                {
                    TH2D* h = (TH2D*) histogramContainers[iHist].histograms[iSample];
                    h->Fill( functionVector[iVar](), functionVector[iVar+1](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
                    iVar++;
                }
                else
                    histogramContainers[iHist].histograms[iSample]->Fill( functionVector[iVar](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
            }
//         }
    }
    else
    {
        cerr << "Indices for sample or subsample out of range in the FakeLeptonPlotter::fill function." << endl;
        throw 1;
    }
}

void FakeLeptonPlotter::writeFakeRate(string extension)
{        
    for( unsigned int iVar = 0; iVar < nHistograms; ++iVar )
    {           
        if( configContainer.addOverflow ) 
        {
            histogramContainers[iVar].addOverflow();
        }
        if( configContainer.addUnderflow ) 
        {
            histogramContainers[iVar].addUnderflow();
        }
        
        
        HistogramContainer fakeHistCont("fake_rate_" + histogramContainers[iVar].containerName, 1);
        vector<TH1*> hMC;
        bool hasEwkCorrection = false;
        
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( histogramContainers[iVar].sampleType[iSample] == SampleType::DATA )
            {
                fakeHistCont.add(histogramContainers[iVar], iSample);
            }
            else if( configContainer.sampleContainer.sampleType[iSample] == SampleType::MC  )
            {
                hMC.push_back(histogramContainers[iVar].histograms[iSample]);
                hasEwkCorrection = true;
            }
        }
        
        // Add QCD signal
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( histogramContainers[iVar].sampleType[iSample] == SampleType::SIGNAL )
            {
                fakeHistCont.add(histogramContainers[iVar], iSample);
            }
        }
        
        // EKW MC correction
        if( hasEwkCorrection )
        {
            // Add data without ewk correction
            fakeHistCont.add(fakeHistCont, 0);
            fakeHistCont.color.back() += 3;
            fakeHistCont.reducedNames.back() += "_before_ewk_correction";
            fakeHistCont.histograms.back() = (TH1*) fakeHistCont.histograms[0]->Clone();
        }
        for( TH1* h : hMC )
        {
            fakeHistCont.histograms[0]->Add(h, -1.);           
        }
        
        // Seperate Numerator and Denominator
        if( histogramContainers[iVar].containerName.find("lepton") != string::npos ) 
        {
            if( histogramContainers[iVar].containerName.find("loose") != string::npos ) 
            {
                hDenomVector.push_back(fakeHistCont.histograms);
            }
            else
            {
                fakeHistogramContainers.push_back(fakeHistCont);
            }
        }
    }
        
    for( unsigned int iFake = 0; iFake < fakeHistogramContainers.size(); ++iFake )
    {
        for( unsigned int iHist = 0; iHist < fakeHistogramContainers[iFake].histograms.size(); ++iHist )
        {
            // remove underflow in numerator, important for TGraphAsymmErrors to work!
            fakeHistogramContainers[iFake].histograms[iHist]->ClearUnderflowAndOverflow();
            
            // fill empty/negative bins
            unsigned int nBins = fakeHistogramContainers[iFake].histograms[iHist]->GetNcells();
            for( unsigned int i = 0; i < (nBins+2); ++i )
            {
                if( hDenomVector[iFake][iHist]->GetBinContent(i) <= 0)
                {
                    hDenomVector[iFake][iHist]->SetBinContent(i,1.);
                    fakeHistogramContainers[iFake].histograms[iHist]->SetBinContent(i,0.);
                }
                if( fakeHistogramContainers[iFake].histograms[iHist]->GetBinContent(i) < 0.)
                {
                    fakeHistogramContainers[iFake].histograms[iHist]->SetBinContent(i,0);
                }
                if( fakeHistogramContainers[iFake].histograms[iHist]->GetBinContent(i) > hDenomVector[iFake][iHist]->GetBinContent(i) )
                {
                    fakeHistogramContainers[iFake].histograms[iHist]->SetBinContent(i,hDenomVector[iFake][iHist]->GetBinContent(i));
                }
            }
        }
        // Make plots
        BasePlotter::writeEfficiency(fakeHistogramContainers[iFake], hDenomVector[iFake], extension);
    }
    
    // Write histrograms
    system(("mkdir -p " + configContainer.outputDir).c_str());   
    TFile* f = new TFile((configContainer.outputDir + "Fakerate.root").c_str(), "RECREATE");
        
    for( unsigned int iFake = 0; iFake < fakeHistogramContainers.size(); ++iFake )
    {
        if( fakeHistogramContainers[iFake].histograms[0]->GetDimension() == 1 )
        {
            // Write Data FR
            TGraphAsymmErrors* tgraph = new TGraphAsymmErrors(fakeHistogramContainers[iFake].histograms[0], hDenomVector[iFake][0], "cl=0.683 b(1,1) mode");
            tgraph->Write();
            
            // Write MC signal FR
            for( unsigned int iHist = 0; iHist < fakeHistogramContainers[iFake].histograms.size(); ++iHist )
            {
                if( fakeHistogramContainers[iFake].sampleType[iHist] == SampleType::SIGNAL )
                {
                    TGraphAsymmErrors* tgraph = new TGraphAsymmErrors(fakeHistogramContainers[iFake].histograms[iHist], hDenomVector[iFake][iHist], "cl=0.683 b(1,1) mode");
                    tgraph->Write();;
                }
            }
        }
        else
        {
            // Write Data FR
            TH1* hTemp = (TH1*) fakeHistogramContainers[iFake].histograms[0]->Clone();
            hTemp->Divide(hDenomVector[iFake][0]);
            hTemp->Write();
            
            // Write MC signal FR
            for( unsigned int iHist = 0; iHist < fakeHistogramContainers[iFake].histograms.size(); ++iHist )
            {
                if( fakeHistogramContainers[iFake].sampleType[iHist] == SampleType::SIGNAL )
                {
                    TH1* hTemp = (TH1*) fakeHistogramContainers[iFake].histograms[iHist]->Clone();
                    hTemp->Divide(hDenomVector[iFake][iHist]);
                    hTemp->Write();
                }
            }
        }
    }  
    f->Close();
    
    // verbose output
    for( unsigned int iVar = 0; iVar < fakeHistogramContainers.size(); ++iVar )
    {  
        
        if( iVar == 0 )
            BasePlotter::writeHist("fake_lepton_intermediate.root", fakeHistogramContainers[iVar].histograms, "RECREATE");
        else
            BasePlotter::writeHist("fake_lepton_intermediate.root", fakeHistogramContainers[iVar].histograms, "UPDATE");
        
        BasePlotter::writeHist("fake_lepton_intermediate.root", hDenomVector[iVar], "UPDATE");
    }   
}
